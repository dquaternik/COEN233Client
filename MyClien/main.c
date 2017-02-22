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
    unsigned char buf1[MAXBUFLEN];
    struct pollfd fd;
    int count = 0;

    //fragment the input message
    strcpy(mess,argv[2]);
    datapack *send = fragment(mess);
    printf("send: %d\n",send);
    datapack *cursend = send;

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
        if(count>0){
            cursend = send->next;
        }

        //Serialize the first fragment
        serialize_Data(*cursend,b);
        printf("serial: %d\n",b->data);
        //send the first fragment to the server
        //ISSUE
        numbytes = sendto(sockfd, b->data, b->size, 0, p->ai_addr, p->ai_addrlen);
        printf("numbytes: %d\n",numbytes);
        if (numbytes == -1)
        {
            perror("talker: sendto");
            exit(1);
        }

        printf("sendto\n");
        //wait for ACK/REJ from server
        res = poll(&fd,1,1000);

        if(res == 0)
        {
            //timeout
        }
        else if(res == -1)
        {
            //error
            perror("poll error");
            return res;

        } else
        {
            //Receive the ACK/REJ
            socklen_t len = sizeof(their_addr);
            numbytes = recvfrom(sockfd,buf1,MAXBUFLEN-1, 0,&their_addr, &len);
            if(numbytes == -1)
            {
                perror("Ack error\n");
                exit(1);
            }
            else{
                printf("Ack received\n");
            }
        };
        printf("recvfrom\n");


        //If rejected, decipher the code

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

//Fragments message into maximum size
datapack * fragment(char message[])
{
    int count = 1;
    int length = strlen(message);
    int nsegs = ceil(((float) length)/((float) MAXPAY)); //find number of fragments to create
    if(length<MAXPAY){
        count = nsegs+1;
    }
    printf("nsegs = %d\n",nsegs);
    datapack *headpack = malloc(sizeof(datapack));        //create head packet and next pack
    datapack *curpack;        //used to create next pack

    //setup head node
    headpack->startid = STARTID;
    headpack->clientid = CLIENTID;
    headpack->data = DATA;
    headpack->segnum = 1;
    headpack->numseg = nsegs;
    if(count>nsegs){
        headpack->len = length;
    } else{
        headpack->len = MAXPAY;
    }
    int i = 0;
    while(i<headpack->len){
        headpack->payload[i] = message[i];
        i++;
    }

    headpack->endid = ENDID;
    curpack = headpack;

    while(count < nsegs)
    {
        curpack->next = malloc(sizeof(datapack));
        curpack->next = curpack;
        //initialize all the constant data
        curpack->startid = STARTID;
        curpack->clientid = CLIENTID;
        curpack->data = DATA;
        curpack->segnum = count +1;
        curpack->numseg = nsegs;
        curpack->len = MAXPAY;

        //insert portion of payload into the packet
        i = 0;
        while(i < MAXPAY){
            curpack->payload[i] = message[255*count+i];
            if(message[255*count+i] == '\0'){
                curpack->len = i; //Set length to i to error check server side. All extra will be null
            }
            i++;
        }

        //Insert endid
        curpack->endid = ENDID;

        count++;
    }

    printf("headpack %d\n",headpack);
    return headpack;
}

//Serialize each fragment
void serialize_Data(datapack send, databuf *output)
{
    serialize_short(send.startid,output);
    printf("serialize start\n");
    serialize_char(send.clientid,output);
    printf("serialize clien\n");
    serialize_short(send.data,output);
    printf("serialize dat\n");
    serialize_char(send.segnum,output);
    printf("serialize seg: %d\n",send.segnum);
    serialize_char(send.len,output);
    printf("serialize len\n");
    for(int i = 0; i < MAXPAY; i++)
    {
        serialize_char(send.payload[i],output);
        printf("serialize pay");
    }
    serialize_short(send.endid,output);
    printf("serialize end\n");
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

//databuffer initilization
databuf *new_buffer(){
    databuf *b = malloc(sizeof(datapack)*2);

    b->data = malloc(sizeof(datapack));
    b->size = sizeof(datapack);
    b->next = 0;

    return b;
};