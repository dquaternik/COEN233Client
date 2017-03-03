CLIENTs for 233 programming assingment 1. 
Should be running server before client, but client will retransmit a few times if setup out of order.

Files:
main.c //A working version of the code
rejcode1.c //A version designed to throw out of sequence error
rejcode2.c //A version designed to throw length mismatch error
rejcode3.c //A version designed to throw end of packet ID missing error
rejcode4.c //A version designed to throw duplicate packet error

To run on linux, type 'gcc -o version version.c -lm' followed by './version'. replacing version with desired client. 


Test Message 5 packets in length: 

Quality of Service does a number of things in the network, with the overall goal of ensuring that all data reaches its destination with the maximum number of users connected. There are six issues that it generally deals with; low throughput, dropped packets, errors, latency, jitter, and out-of-order delivery. How you handle this takes a few forms, of which multiple can be used at once to ensure the best performance from end to end. Although it affects everything, QoS does not occur at every level of the network. QoS is an issue at the network layer. It has to ensure that heterogeneous networks are able to communicate seamlessly, bandwidth is not wasted, and maximum transmission rates are achieved. It is also focused on ensuring multiple users are able to connect to the network at once, their data is sent and received in the proper order, and collision detection and avoidance. Both require sophisticated algorithms to allow for maximum performance and extensibility. As with all things on a network, interoperability is one of the highest concerns. For this reason, many of the protocols refer to others and specify which can be mixed. 
