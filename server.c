



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
  uint16_t major_version; // 1, conversion needed (for practice)
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

int main(int argc, char *argv[])
{
     int sock, port;                        
     struct sockaddr_in echoServAddr; 
     struct sockaddr_in echoClntAddr; 
     unsigned int cliAddrLen;         
     unsigned short echoServPort;     
     char delim[]=":";
     char *Desthost=strtok(argv[1],delim);
     char *Destport=strtok(NULL,delim);
     port = atoi(Destport);
                

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
  echoServAddr.sin_addr.s_addr = inet_addr(Desthost);
  echoServAddr.sin_port = htons(port); 
  /* Bind to the local address */ 
  if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
  
  printf("socket bind failed...\n"); 
		//exit(0); 
		}
 
    
  else
  printf("Socket successfully binded..\n"); 
   
  cliAddrLen = sizeof(struct sockaddr_in);   
  //inet_ntop(AF_INET, &(echoClntAddr.sin_addr), clientAddr, CLADDR_LEN);   
  
  for (;;) /* Run forever */
  {
        /* Set the size of the in-out parameter */
       printf("\n\nListening for incoming messages...\n");
      printf("==========================================\n"); 
        if(recvfrom(sock, (struct calcMessage*)&message, sizeof(message),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

       
        DieWithError("recvfrom() failed");
        }
        char* ipString = inet_ntoa( echoServAddr.sin_addr);
        printf("Received message from IP: %s \n",ipString);
        printf("Successfully received calcMessage from client\n");
        printf("size of calcMessage: %d\n", sizeof(message));
        if (ntohs(message.message)==0){
        printf("message: %d <-> 0 OK\n", ntohs(message.message) );//printf("protocol: %d\n", ntohs(message.protocol));
        }
        else{
           printf("message: %d <-> 0 NOT OK\n", ntohs(message.message) );
        }
        //printf("message: %d\n", ntohs(message.message));
        if (ntohs(message.protocol)==17){
        printf("protocol: %d <-> 17 OK\n", ntohs(message.protocol) );//printf("protocol: %d\n", ntohs(message.protocol));
        }
        else{
           printf("Ppotocol: %d <-> 17 NOT OK\n", ntohs(message.protocol) );
        }
        //printf("type: %d\n", ntohs(message.type)); 
        if (ntohs(message.type)==22){
        printf("type: %d <-> 22 OK\n", ntohs(message.type) );//printf("protocol: %d\n", ntohs(message.protocol));
        }
        else{
           printf("type: %d <-> 22 NOT OK\n", ntohs(message.type) );
        }
        //printf("major_version: %d\n", ntohs(message.major_version)); 
        if (ntohs(message.major_version)==1){
        printf("major_version: %d <-> 1 OK\n", ntohs(message.major_version) );//printf("protocol: %d\n", ntohs(message.protocol));
        }
        else{
           printf("major_version: %d <-> 1 NOT OK\n", ntohs(message.major_version) );
        }
        if (ntohs(message.minor_version)==0){
        printf("minor_version: %d <-> 0 OK\n", ntohs(message.minor_version) );//printf("protocol: %d\n", ntohs(message.protocol));
        }
        else{
           printf("minor_version: %d <-> 0 NOT OK\n", ntohs(message.minor_version) );
        }
        if (ntohs(message.protocol)!=17){
        memset(&message, 0, sizeof(message)); 
        message.major_version = htons(1);
        message.minor_version =htons(0);
        //message.protocol =htons(17);
        message.message = htons(2);
        message.type = htons(2);
        int n = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (n == -1 ) {

               printf("Sent struct size: %d\n", n);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      memset(&protocol, 0, sizeof(protocol));
          //printf("%d <-> 17 NOT OK\n", ntohs(message.protocol) );
          printf("Server exiting...\n");
          close(sock);
          exit(1);
        }
      
         
     
  initCalcLib();
  char *ptr;
  ptr=randomType(); 
  int success, ans, send;
  double results;
  double f1,f2,fresult;
  int i1,i2,iresult;
  
  
  if(ptr[0]=='f'){
   // printf("Float\t");
    f1=randomFloat();
    f2=randomFloat();
    protocol.flValue1 =f1;
    protocol.flValue2 =f2;

 
   
    if(strcmp(ptr,"fadd")==0){
      protocol.arith = htonl(5);
      fresult=f1+f2;
      //protocol.flResult = fresult;
      success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%f + %f] to client \n", f1,f2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      printf("\nReceived %f as task results from client\n",protocol.flResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
      printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 

    } else if (strcmp(ptr, "fsub")==0){
      fresult=f1-f2;
      protocol.arith = htonl(6);
      //protocol.flResult = fresult;
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%f - %f] to client \n", f1,f2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

       
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      printf("\nReceived %f as task results from client\n",protocol.flResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
      printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      
    } else if (strcmp(ptr, "fmul")==0){
      fresult=f1*f2;
      protocol.arith = htonl(7);
      //protocol.flResult = fresult;
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%f * %f] to client \n", f1,f2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

       
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      printf("\nReceived %f as task results from client\n",protocol.flResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
       printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
    } else if (strcmp(ptr, "fdiv")==0){
      fresult=f1/f2;
      protocol.arith = htonl(8);
      //protocol.flResult = fresult;
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%f / %f] to client \n", f1,f2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        
        DieWithError("recvfrom() failed");
        }
      results = protocol.flResult;
      printf("\nReceived %f as task results from client\n",protocol.flResult);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(fresult- results) < 0.01){
      message.message = htonl(1);
       printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
    }
  
  } else {
    //printf("Int\t");
    i1=randomInt();
    i2=randomInt();
    protocol.inValue1 =htonl(i1);
    protocol.inValue2 =htonl(i2);

    if(strcmp(ptr,"add")==0){
      iresult=i1+i2;
      protocol.arith = htonl(1);
      //rotocol.inResult = htonl(iresult);
      
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%d + %d] to client \n", i1,i2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

        
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      printf("\nReceived %d as task results from client\n",ans);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
      printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      
      
    } else if (strcmp(ptr, "sub")==0){
      iresult=i1-i2;
      protocol.arith = htonl(2);
      //protocol.inResult = htonl(iresult);
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%d - %d] to client \n", i1,i2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

       
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      printf("\nReceived %d as task results from client\n",ans);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
       printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      
    } else if (strcmp(ptr, "mul")==0){
      iresult=i1*i2;
      protocol.arith = htonl(3);
     // protocol.inResult = htonl(iresult);
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
      printf("\n\nSent task [%d * %d] to client \n", i1,i2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

      
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      printf("\nReceived %d as task results from client\n",ans);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
       printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      
    } else if (strcmp(ptr, "div")==0){
      iresult=i1/i2;
      protocol.arith = htonl(4);
      //protocol.inResult = htonl(iresult);
       success = sendto(sock, (struct calcProtocol*)&protocol, sizeof(protocol), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
     if (success == -1 ) {

               printf("Sent struct size: %d\n", success);
               DieWithError("sendto() sent a different number of bytes than expected\n");
        }
       printf("\n\nSent task [%d / %d] to client \n", i1,i2); 
      memset(&protocol, 0, sizeof(protocol));
     if(recvfrom(sock, (struct calcProtocol*)&protocol, sizeof(protocol),   0, (struct sockaddr *) &echoServAddr, &cliAddrLen)<0){

      
        DieWithError("recvfrom() failed");
        }
      ans = ntohl(protocol.inResult);
      printf("\nReceived %d as task results from client\n",ans);
      memset(&protocol, 0, sizeof(protocol));
      if (abs(iresult- ans) < 0.01){
      message.message = htonl(1);
      printf("Accepting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        memset(&message, 0, sizeof(message));
      } 
      else {
      message.message = htonl(0);
      printf("Rejecting this results\n");
      send = sendto(sock, (struct calcMessage*)&message, sizeof(message), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr));
        if (send < 0 ) {

                printf("Error. Could not inform client\n");
        }
        printf("Client informed\n");
        printf("==========================================\n");
        memset(&message, 0, sizeof(message));
      } 
      
    }
  }


       // close(sock);
        //return 0;
  } 
}

