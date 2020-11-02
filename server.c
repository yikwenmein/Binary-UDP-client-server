



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#define PORT 8080 
#define CLADDR_LEN 100


char clientAddr[CLADDR_LEN];

/* Include the calcLib header file, using <> as its a library and not just a object file we link.  */
#include "calcLib.h"



#include "calcLib.c"

void DieWithError(char *err) {
   perror(err);
   exit(1);
}


//char buff[MAXLINE];
//int clientlen;
    
struct  __attribute__((__packed__)) calcMessage {
  uint16_t type;    // See below, conversion needed (for practice)
  uint32_t message; // See below, conversion needed (for practice)
  
  // Protocol, UDP = 17, TCP = 6, other values are reserved. 
  uint16_t protocol; // conversion needed (for practice)
  uint16_t version; // 1, conversion needed (for practice)
  uint16_t minor_version; // 0 , conversion needed (for practice)

};
struct  __attribute__((__packed__)) calcProtocol {
  uint16_t type;  // What message is this, 1 = server to client, 2 client to server, 3... reserved , conversion needed (for practice)
  uint16_t major_version; // 1, conversion needed (for practice)
  uint16_t minor_version; // 0, conversion needed (for practice)
  uint32_t id; // Server side identification with operation. Client must return the same ID as it got from Server., conversion needed (for practice)
  uint32_t arith; // What operation to perform, see mapping below. 
  int32_t inValue1; // integer value 1, conversion needed (for practice)
  int32_t inValue2; // integer value 2, conversion needed (for practice)
  int32_t inResult; // integer result, conversion needed (for practice)
  double flValue1;  // float value 1,NO NEED TO do host to Network or Network to Host conversion here, we are using equivalent platforms        
  double flValue2;  // float value 2,NO NEED TO do host to Network or Network to Host conversion here, we are using equivalent platforms
  double flResult;  // float result,NO NEED TO do host to Network or Network to Host conversion here, we are using equivalent platforms
};


 struct calcMessage message;
 struct calcProtocol  protocol; 

int main()
{
     int sock;                        /* Socket */
     struct sockaddr_in echoServAddr; /* Local address */
     struct sockaddr_in echoClntAddr; /* Client address */
     unsigned int cliAddrLen;         /* Length of incoming message */
     unsigned short echoServPort;     /* Server port */
     int recvMsgSize;                 /* Size of received message */ 

  //struct TextMessage * temp = malloc(sizeof(struct TextMessage));

/*  if (!argv[1]) {*/
/*      fprintf(stderr,"no port number provided\n");*/
/*     exit(1);*/
/*  }*/

    //echoServPort = atoi(PORT);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */  
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    //DieWithError("socket() failed");
    printf("socket creation failed...\n"); 
   //exit(0); 
 }
  else
   printf("Socket successfully created..\n");
  /* Construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
  memset(&echoClntAddr, 0, sizeof(echoClntAddr)); 
  echoServAddr.sin_family = AF_INET;                /* Internet address family */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  echoServAddr.sin_port = htons(PORT);      /* Local port */

  /* Bind to the local address */ 
  if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
  
  printf("socket bind failed...\n"); 
		//exit(0); 
		}
    // DieWithError("bind() failed");
    
  else
  printf("Socket successfully binded..\n"); 
   
  cliAddrLen = sizeof(echoClntAddr);   
  inet_ntop(AF_INET, &(echoClntAddr.sin_addr), clientAddr, CLADDR_LEN);   
  
  for (;;) /* Run forever */
  {
        /* Set the size of the in-out parameter */
      
        
        if(recvfrom(sock, (struct calcMessage*)&message, sizeof(message),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
     
        printf("version: %d\n", ntohs(message.version));   /* Print the string in sent struct */
        printf("Protocol: %d\n", ntohs(message.protocol));
        printf("Message: %d\n", ntohs(message.message));
        printf("Type: %d\n", ntohs(message.type));
        memset(&message, 0, sizeof(message));
         
       printf("size of calcMessage: %d\n", sizeof(message)); 
         
      
           
    /* Initialize the library, this is needed for this library. */
  initCalcLib();
  char *ptr;
  ptr=randomType(); // Get a random arithemtic operator. 
  int success, ans, send;
  double results;
  double f1,f2,fresult;
  int i1,i2,iresult;
  /*
  printf("ptr = %p, \t", ptr );
  printf("string = %s, \n", ptr );
  */

  /* Act differently depending on what operator you got, judge type by first char in string. If 'f' then a float */
  
  if(ptr[0]=='f'){
   // printf("Float\t");
    f1=randomFloat();
    f2=randomFloat();
    protocol.flValue1 =f1;
    protocol.flValue2 =f2;

    /* At this point, ptr holds operator, f1 and f2 the operands. Now we work to determine the reference result. */
   
    if(strcmp(ptr,"fadd")==0){
      protocol.arith = htonl(5);
      fresult=f1+f2;
      protocol.flResult = fresult;
      success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 

    } else if (strcmp(ptr, "fsub")==0){
      fresult=f1-f2;
      protocol.arith = htonl(6);
      protocol.flResult = fresult;
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      
    } else if (strcmp(ptr, "fmul")==0){
      fresult=f1*f2;
      protocol.arith = htonl(7);
      protocol.flResult = fresult;
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
    } else if (strcmp(ptr, "fdiv")==0){
      fresult=f1/f2;
      protocol.arith = htonl(8);
      protocol.flResult = fresult;
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
    }
  //  printf("%s %8.8g %8.8g = %8.8g\n",ptr,f1,f2,fresult);
  } else {
    //printf("Int\t");
    i1=randomInt();
    i2=randomInt();
    protocol.inValue1 =htonl(i1);
    protocol.inValue2 =htonl(i2);

    if(strcmp(ptr,"add")==0){
      iresult=i1+i2;
      protocol.arith = htonl(1);
      protocol.inResult = htonl(iresult);
      
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      
      
    } else if (strcmp(ptr, "sub")==0){
      iresult=i1-i2;
      protocol.arith = htonl(2);
      protocol.inResult = htonl(iresult);
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      
    } else if (strcmp(ptr, "mul")==0){
      iresult=i1*i2;
      protocol.arith = htonl(3);
      protocol.inResult = htonl(iresult);
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      
    } else if (strcmp(ptr, "div")==0){
      iresult=i1/i2;
      protocol.arith = htonl(4);
      protocol.inResult = htonl(iresult);
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        /* Block until receive message from  a client */
        //if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0,
           //(struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error sending calcMessage.message to client\n");
        }
        printf("Successfully sent calcMessage.message to client\n");
        memset(&message, 0, sizeof(message));
      } 
      
    }

  //  printf("%s %d %d = %d \n",ptr,i1,i2,iresult);
  }
/*  protocol.id = htonl(15);*/
/*  int success = 0;*/
/*  success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)*/
/*         &echoServAddr, sizeof(echoServAddr));*/
/*  if (success == -1 ) {*/

/*               printf("Sent struct size: %d\n", success);*/
/*               DieWithError("sendto() sent a different number of bytes than expected\n");*/
/*        }*/
/*     printf("size of calcProtocol: %d\n", sizeof(protocol)); */
   }
      /* NOT REACHED */

        close(sock);
        return 0;
  } 


