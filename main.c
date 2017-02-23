#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <math.h>
#include <poll.h>

#define SERVER "1337" //Server port
#define MAXPAY 0xff
#define CLIENTID 0x45
#define MAXBUFLEN 1276
#define STARTID 0xffff
#define ENDID 0xffff
#define DATA 0xfff1
#define ACK 0xfff2
#define REJECT 0xfff3

//Define packet structures
typedef struct datapack {
    unsigned short startid;
    unsigned char clientid;
    unsigned short data;
    unsigned char segnum;
    unsigned char len;
    unsigned char payload[MAXPAY];
    unsigned short endid;
    unsigned short numseg;
    struct datapack *next;
}datapack;

typedef struct ackpack {
    short startid;
    char clientid;
    short ack;
    char segnum;
    short endid;
}ackpack;

typedef struct rejpack {
    short startid;
    char clientid;
    short reject;
    short subc;
    char segnum;
    short endid;
}rejpack;

//create buffers
typedef struct databuf {
    void *data;
    int next;
    size_t size;
}databuf;


//Function definitions
datapack * fragment(char message[]);
void serialize_Data(datapack send, databuf *output);
void reserve(databuf *b, size_t bytes);
void serialize_short(short x, databuf *b);
void serialize_char(char x, databuf *b);
databuf *new_buffer();

//Takes in an ip address (localhost for client on same computer) and string to send.
int main(int argc, char *argv[])
{
    //variable initializations
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes, res;
    struct sockaddr_in their_addr;
    char mess[strlen(argv[2])];
    databuf *b = new_buffer();
    int count = 0;
    unsigned char buf1[MAXBUFLEN];
    struct pollfd fd;
    int count1 = 0;

    //fragment the input message
    strcpy(mess,argv[2]);
    datapack *send = fragment(mess);

    //Ensure proper number of arguments
    if(argc != 3)
    {
        fprintf(stderr,"Usage: Talker honesname message\n");
        exit(1);
    }

    //Setup hints for network
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    //Make sure your and server networks are the same and you have the correct address.
    if ((rv = getaddrinfo(argv[1], SERVER, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    //Create a socket with the required parameters.
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("talker: socket");
            continue;
        }
        break;
    }

    //Setup polling
    fd.fd = sockfd;
    fd.events = POLLIN;

    //error check the socket was created properly
    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }


    //LOOP START
    while(count < send->numseg)
    {
        //Serialize the first fragment
        serialize_Data(*send,b);
        //send the first fragment to the server
        numbytes = sendto(sockfd, b->data, b->size, 0, p->ai_addr, p->ai_addrlen);
        if (numbytes == -1)
        {
            perror("talker: sendto");
            exit(1);
        }

        //wait for ACK/REJ from server (This is ack_timer
        while(count1 < 2){
            res = poll(&fd,1,3000);
            if(res == 0) {
                //timeout
                //resend + increment count
                numbytes = sendto(sockfd, b->data, b->size, 0, p->ai_addr, p->ai_addrlen);
                if (numbytes == -1)
                {
                    perror("talker: sendto");
                    exit(1);
                }
                count1++;

            }
            else if(res == -1) {
                //error
                perror("poll error");
                return res;

            } else {
                //Receive the ACK/REJ
                socklen_t len = sizeof(their_addr);
                int numbytes = recvfrom(sockfd,buf1,MAXBUFLEN-1, 0,&their_addr, &len);
                if(numbytes == -1){
                    perror("ACK/REJ reception error");
                    exit(1);
                }
                ackpack *ack = malloc(sizeof(ackpack));
                rejpack *rej = malloc(sizeof(rejpack));
                deserialize(ack,rej,buf1);
                free(ack);
                free(rej);
                break;
            };
        }



        //next packet
        send = send->next;
        b->next = 0;
        count1 = 0;
        count++;
        printf("%d\n",count);
        //LOOP END

    }






    //free memory from server info after sending and close the socket and turn off client
    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n",numbytes, argv[1]);
    close(sockfd);
    free(send);
    free(b);

    return 0;
}

int ackrej(int sockfd, struct sockaddr_in *their_addr){




}


//databuffer initilization
databuf *new_buffer(){
    databuf *b = malloc(sizeof(datapack)*2);

    b->data = malloc(sizeof(datapack));
    b->size = sizeof(datapack);
    b->next = 0;

    return b;
};

//Fragments message into maximum size
datapack *fragment(char message[])
{
    int count = 0,flag1 = 0,i = 0;
    int nsegs = ceil(((float) strlen(message))/((float) MAXPAY)); //find number of fragments to create
    printf("nsegs = %d\n",nsegs);
    datapack *sendpack = malloc(sizeof(datapack)*nsegs);        //create array with as many packets as needed
    while(count < nsegs)
    {
        //initialize all the constant data
        sendpack[count].startid = STARTID;
        sendpack[count].clientid = CLIENTID;
        sendpack[count].data = DATA;
        sendpack[count].segnum = count +1;
        sendpack[count].numseg = nsegs;
        sendpack[count].len = MAXPAY;

        //insert portion of payload into the packet
        i = 0;
        while(i < MAXPAY){
            sendpack[count].payload[i] = message[255*count+i];
            if(message[255*count+i] == '\0' && flag1 != 1){
                sendpack[count].len = i; //Set length to i to error check server side. All extra will be null
                flag1 = 1;
            }
            i++;
        }

        //Insert endid
        sendpack[count].endid = ENDID;
        if(count > 0)
        {
            sendpack[count-1].next = &sendpack[count];
        }
        count++;
        flag1 = 0;
    }

    return sendpack;
}

//Serialize each fragment
void serialize_Data(datapack send, databuf *output)
{
    serialize_short(send.startid,output);
    serialize_char(send.clientid,output);
    serialize_short(send.data,output);
    serialize_char(send.segnum,output);
    serialize_char(send.len,output);
    for(int i = 0; i < MAXPAY; i++)
    {
        serialize_char(send.payload[i],output);
    }
    serialize_short(send.endid,output);
}

//Space reservation. Can be useful when varying packet size, but this setup only has 1 so it goes unused.
void reserve(databuf *b, size_t bytes)
{
    if((b->next + bytes) > b->size)
    {
        b->data = realloc(b->data,b->size*2);
        b->size *= 2;
    }
};


//Serialize by data size
void serialize_short(short x, databuf *b)
{
    //reserve(b, sizeof(short));
    memcpy(((char *)b->data) + b->next, &x, sizeof(short));
    b->next += sizeof(short);
};


void serialize_char(char x, databuf *b)
{
    //reserve(b,sizeof(char));
    memcpy(((char *)b->data)+ b->next,&x,sizeof(char));
    b->next += sizeof(char);
};

//Deserialize ACK/REJ packs
int deserialize(ackpack *ack,rejpack *rej, char buffer[]){


    if(((u_char) buffer[3] == 0xf2 && (u_char) buffer[4] == 0xff)
       || ((u_char) buffer[3] == 0xff && (u_char) buffer[4] == 0xf2)){
        ack->ack = ACK;
        printf("ACK received\n");

    }else if(((u_char) buffer[3] == 0xf3 && (u_char) buffer[4] == 0xff)
            || ((u_char) buffer[3] == 0xff && (u_char) buffer[4] == 0xf3)){

        rej->reject = REJECT;
        rej->subc = buffer[5] + buffer[6];
        if(rej->subc == 1){
            perror("Error: Packet StartID incorrect\n");
            exit(1);
        } else if(rej->subc == 2){
            perror("Error: Packet client field incorrect\n");
            exit(2);
        } else if(rej->subc == 3){
            perror("Error: Packet DATA field incorrect\n");
            exit(3);
        } else if(rej->subc == 4){
            perror("Error: Packet Segment Out of Order\n");
            exit(4);
        } else if(rej->subc == 5){
            perror("Error: Payload does not match length\n");
            exit(5);
        } else if(rej->subc == 7){
            perror("Error: Missing End of Packet ID\n");
            exit(7);
        }

    } else{
        return 2;
    }

};