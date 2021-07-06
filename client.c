

  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <strings.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #define MAXLINE 1024 

  void DieWithError(char *err) {
       perror(err);
       exit(1);
  }

char buff[MAXLINE];
 int serverlen;
    
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

  int main(int argc, char *argv[]) 
  { 
       int sock,port;                        /* Socket descriptor */
       struct sockaddr_in echoServAddr; /* Echo server address */
       struct sockaddr_in fromAddr;     /* Source address of echo */
       unsigned short echoServPort;     /* Echo server port */
       unsigned int fromSize;           /* In-out of address size for recvfrom() */
       char *servIP;                    /* IP address of server */
       unsigned int structLen;                   /* Length of string to echo */
       int respStringLen;               /* Length of received response */
       
       char delim[]=":";
        char *Desthost=strtok(argv[1],delim);
        char *Destport=strtok(NULL,delim);
        port = atoi(Destport);

       if (argc != 2) {
	   fprintf(stderr,"usage: ./client IP:PORT\n");
	   exit(1);
	}
     //   if (!argv[1]) {
     //          printf("No server IP:PORT sepcified at arg 1\n");
     //          printf("Supply IP:PORT as an argument\n");
              
     //   }


        
        struct calcMessage msg;
        
        msg.major_version = htons(1);
        msg.minor_version =htons(0);
        msg.protocol =htons(17);
        msg.message = htons(0);
        msg.type = htons(22);
       
        printf("\nSize of sent calcMessage struct: %d\n", sizeof(msg));

      

        struct calcProtocol  temp; 


        /* Create a datagram/UDP socket */
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
               DieWithError("socket() failed");

        /* Construct the server address structure */
        memset(&echoServAddr, 0, sizeof(echoServAddr));    
        echoServAddr.sin_family = AF_INET;                
        echoServAddr.sin_addr.s_addr = inet_addr(Desthost);  
        echoServAddr.sin_port   = htons(port);     


        fd_set          readfds;    
        struct timeval  timeout;    


        int tempint = 0;
        serverlen = sizeof(echoServAddr);
        /* Send the string to the server */
        tempint = sendto(sock, (struct calcMessage*)&msg, sizeof(msg), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr)); 
        //memset(&msg, 0, sizeof(msg));
        
      
        
        FD_ZERO( &readfds );        // Reset all bits
        FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
        timeout.tv_sec = 2;         // Set the time out value (5 sec)
        timeout.tv_usec = 0;
        if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0){
             if (tempint == -1 ) 
                 tempint = sendto(sock, (struct calcMessage*)&msg, sizeof(msg), 0, (struct sockaddr *)
                 &echoServAddr, sizeof(echoServAddr)); 
                 //memset(&msg, 0, sizeof(msg));
        
                 FD_ZERO( &readfds );        // Reset all bits
                 FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
                 timeout.tv_sec = 2;         // Set the time out value (5 sec)
                 timeout.tv_usec = 0;
                 
                 if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0)
                       if (tempint == -1 ) 
                       tempint = sendto(sock, (struct calcMessage*)&msg, sizeof(msg), 0, (struct sockaddr *)
                       &echoServAddr, sizeof(echoServAddr)); 
                       //memset(&msg, 0, sizeof(msg));
        
                       FD_ZERO( &readfds );        // Reset all bits
                       FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
                       timeout.tv_sec = 2;         // Set the time out value (5 sec)
                       timeout.tv_usec = 0;
                       if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0)
                            if (tempint == -1 ) 
                            printf("Sent failed\n");
                            close(sock);
                            exit(1);
          }             
      printf("\ncalcMessage sucessfully sent to server\n");
        
        memset(&temp, 0, sizeof(temp));
        recvfrom(sock, (struct calcProtocol*)&temp, sizeof(temp),   0, (struct sockaddr *) &echoServAddr, &serverlen);
        if (ntohl(temp.arith)==0 ){
         memset(&msg, 0, sizeof(msg));
        //recvfrom(sock, (struct calcMessage*)&msg, sizeof(msg),   0, (struct sockaddr *) &echoServAddr, &serverlen);
         //if (ntohl(msg.message)==2 && ntohl(msg.type)==2){
              printf("Client exiting...\n");
              close(sock);
              exit(1);
         //}

        }

        
        printf("\n\nReceived from server:\n");
        //printf("type: %d\n", ntohs(temp.type)); 
        //printf("major_version: %d\n", ntohs(temp.major_version)); 
        //printf("minor_version: %d\n", ntohl(temp.minor_version));
        if (ntohs(temp.type)==ntohl(msg.message)==2){
             printf("Protocol not supported\n");
             close(sock);
             exit(1);
        }
        printf("arith: %d\n", ntohl(temp.arith)); 
        if (ntohl(temp.arith)>4 && ntohl(temp.arith)<9 ){
             
        printf("flValue1: %f\n", (temp.flValue1)); 
        printf("flValue2: %f\n", (temp.flValue2));
        
        }
       if (ntohl(temp.arith)>0 && ntohl(temp.arith)<5 ){
       
       printf("inValue1: %d\n", ntohl(temp.inValue1)); 
       printf("inValue2: %d\n", ntohl(temp.inValue2)); 
        //printf("Result: %d\n", ntohl(temp.inResult)); 
          
       }
     printf("Size of received calcProtocol message struct is %d\n",sizeof(temp));
        int32_t inResult; 
        double flResult;
        if (ntohl(temp.arith)==1)
             inResult = ntohl(temp.inValue1)+ntohl(temp.inValue2);
            // temp.inResult = htons(temp.inResult);
        if (ntohl(temp.arith)==2)
             inResult = ntohl(temp.inValue1)-ntohl(temp.inValue2); 
        if (ntohl(temp.arith)==3)
             inResult = ntohl(temp.inValue1)*ntohl(temp.inValue2);
        if (ntohl(temp.arith)==4)
             inResult = ntohl(temp.inValue1)/ntohl(temp.inValue2);
        if (ntohl(temp.arith)==5)
             flResult = temp.flValue1+temp.flValue2; 
        if (ntohl(temp.arith)==6)
             flResult = temp.flValue1-temp.flValue2; 
        if (ntohl(temp.arith)==7)
             flResult = temp.flValue1*temp.flValue2;
        if (ntohl(temp.arith)==8)
             flResult = temp.flValue1/temp.flValue2;  
        //memset(&temp, 0, sizeof(temp)); 
        temp.inResult = htonl(inResult);
        temp.flResult = flResult;
        uint32_t idr = ntohl(temp.id);
        temp.id = htonl(idr);
        tempint = sendto(sock, (struct calcProtocol*)&temp, sizeof(temp), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr)); 
         
         
         
         FD_ZERO( &readfds );        // Reset all bits
        FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
        timeout.tv_sec = 2;         // Set the time out value (5 sec)
        timeout.tv_usec = 0;
        if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0){
         
            if (tempint == -1 ) 
                 tempint = sendto(sock, (struct calcMessage*)&msg, sizeof(msg), 0, (struct sockaddr *)
                 &echoServAddr, sizeof(echoServAddr)); 
                 memset(&msg, 0, sizeof(msg));
        
                 FD_ZERO( &readfds );        // Reset all bits
                 FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
                 timeout.tv_sec = 2;         // Set the time out value (5 sec)
                 timeout.tv_usec = 0;
                 
                 if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0)
                       if (tempint == -1 ) 
                       tempint = sendto(sock, (struct calcMessage*)&msg, sizeof(msg), 0, (struct sockaddr *)
                       &echoServAddr, sizeof(echoServAddr)); 
                       memset(&msg, 0, sizeof(msg));
        
                       FD_ZERO( &readfds );        // Reset all bits
                       FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
                       timeout.tv_sec = 2;         // Set the time out value (5 sec)
                       timeout.tv_usec = 0;
                       if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0)
                            if (tempint == -1 ) 
                            printf("Sent failed\n");
                            close(sock);
          } 
       if (ntohl(temp.arith)==1 ){
       
       printf("\n\nSending %d back to server as results for [%d + %d] \n", inResult, ntohl(temp.inValue1),ntohl(temp.inValue2)); 
       }
       if (ntohl(temp.arith)==2 ){
       
       printf("\n\nSending %d back to server as results for [%d - %d]\n", inResult, ntohl(temp.inValue1),ntohl(temp.inValue2)); 
       }
       if (ntohl(temp.arith)==3 ){
       
       printf("\n\nSending %d back to server as results for [%d * %d] \n", inResult, ntohl(temp.inValue1),ntohl(temp.inValue2)); 
       }
       if (ntohl(temp.arith)==4 ){
       
       printf("\n\nSending %d back to server as results for [%d / %d] \n", inResult, ntohl(temp.inValue1),ntohl(temp.inValue2)); 
       }
       if (ntohl(temp.arith)==5 ){
       
       printf("\n\nSending %f back to server as results for [%f + %f] \n", flResult, temp.flValue1,temp.flValue2); 
       }
       if (ntohl(temp.arith)==6 ){
       
       printf("\n\nSending %f back to server as results for [%f - %f] \n", flResult, temp.flValue1,temp.flValue2); 
       }
       if (ntohl(temp.arith)==7 ){
       
       printf("\n\nSending %f back to server as results for [%f * %f] \n", flResult, temp.flValue1,temp.flValue2); 
       }
       if (ntohl(temp.arith)==8 ){
       
       printf("\n\nSending %f back to server as results for [%f / %f] \n", flResult, temp.flValue1,temp.flValue2); 
       }
        memset(&temp, 0, sizeof(temp));
        memset(&msg, 0, sizeof(msg));
        recvfrom(sock, (struct calcMessage*)&msg, sizeof(msg),   0, (struct sockaddr *) &echoServAddr, &serverlen);
        printf("\n\nFeedback from server \n");
        if (ntohl(msg.message)==1){
           printf("message: %d <-> 1 OK\n", ntohl(msg.message) );
           printf("Accepted \n");
        }
        
        if (ntohl(msg.message)==2){
          printf("message: %d <-> 1 NOT OK\n", ntohl(msg.message) );
          printf("Rejected\n");
        }
       
        if (ntohl(msg.message)==0){
          printf("message: %d <-> 1 NOT OK\n", ntohl(msg.message) );
          printf("(N/A. Not applicable\n");
        }
        
        
        memset(&msg, 0, sizeof(msg));
        //printf("Operation from server is: %s\n", buff);
        //memset(sCN, 0, sizeof(sCN));
        bzero(buff, sizeof(buff));
        close(sock);
        exit(0);
}



