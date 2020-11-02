

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


       if (!argv[1]) {
              printf("No server IP:PORT sepcified at arg 1\n");
              printf("Supply IP:PORT as an argument\n");
              
       }

/*       else if (!argv[2]) {*/
/*              fprintf(stderr,"No port Number Sepcified at arg 2\n");*/
/*              exit(2);*/
/*}*/

/*       else if (!argv[3]) {*/
/*              fprintf(stderr,"no message string specified at arg 3\n");*/
/*              exit(3);*/
/*       }*/



        //struct TextMessage newMess = { "Send" , 300 , 200 };
        
        struct calcMessage newMess;
        
        newMess.version = htons(1.0);
        //newMess.minor_version =0;
        newMess.protocol =htons(17);
        newMess.message = htons(0);
        newMess.type = htons(22);
       

        //    memset(&newMess, 0, sizeof(newMess));    /* Zero out structure */
        //    strcpy(newMess.request_Type,"Send"); 
        //    newMess.SenderId = 300;
        //    newMess.RecipientId = 200;
        //strncpy(newMess.message, argv[3],sizeof(argv[3]));

        printf("Size struct: %d\n", sizeof(newMess));

        //servIP = argv[1];           /* First arg: server IP address (dotted quad) */
       // echoServPort = atoi(argv[2]);  /* Use given port, if any */

        struct calcProtocol  temp; //= malloc(sizeof(struct calcProtocol));


        /* Create a datagram/UDP socket */
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
               DieWithError("socket() failed");

        /* Construct the server address structure */
        memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
        echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
        echoServAddr.sin_addr.s_addr = inet_addr(Desthost);  /* Server IP address */
        echoServAddr.sin_port   = htons(port);     /* Server port */


        fd_set          readfds;    // descriptor set
        struct timeval  timeout;    // Time out value


        int tempint = 0;
        serverlen = sizeof(echoServAddr);
        /* Send the string to the server */
        tempint = sendto(sock, (struct calcMessage*)&newMess, sizeof(newMess), 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr)); 
        memset(&newMess, 0, sizeof(newMess));
        
      
        
        FD_ZERO( &readfds );        // Reset all bits
        FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
        timeout.tv_sec = 2;         // Set the time out value (5 sec)
        timeout.tv_usec = 0;
        if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0){
         //printf("sent:%d\n", newMess.protocol);
            if (tempint == -1 ) 
                 tempint = sendto(sock, (struct calcMessage*)&newMess, sizeof(newMess), 0, (struct sockaddr *)
                 &echoServAddr, sizeof(echoServAddr)); 
                 memset(&newMess, 0, sizeof(newMess));
        
                 FD_ZERO( &readfds );        // Reset all bits
                 FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
                 timeout.tv_sec = 2;         // Set the time out value (5 sec)
                 timeout.tv_usec = 0;
                 
                 if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0)
                       if (tempint == -1 ) 
                       tempint = sendto(sock, (struct calcMessage*)&newMess, sizeof(newMess), 0, (struct sockaddr *)
                       &echoServAddr, sizeof(echoServAddr)); 
                       memset(&newMess, 0, sizeof(newMess));
        
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
              
               //DieWithError("sendto() sent a different number of bytes than expected\n");
        else
        
/*        if ((recvfrom(sock, (struct calcMessage*)&newMess, sizeof(newMess),   0, (struct sockaddr *) &echoServAddr, &serverlen))==0){*/
/*        printf("type: %d\n", ntohs(newMess.type)); */
/*         printf("message: %d\n", ntohs(newMess.type));*/
/*         memset(&newMess, 0, sizeof(newMess));*/
/*         printf("Protocol specifications are bad, closing . . .\n");*/
/*        */
/*         close(sock);*/
/*        */
/*        }*/
/*        else*/
       // recvfrom(sock, temp, sizeof(*temp), 0,(struct sockaddr *) &echoClntAddr, &cliAddrLen)) 
        //recvfrom(sock,buff,sizeof(buff),0,(struct sockaddr *)  &echoServAddr, &(echoServAddr));
        recvfrom(sock, (struct calcProtocol*)&temp, sizeof(temp),   0, (struct sockaddr *) &echoServAddr, &serverlen);
        
/*         if(ntohs(temp.type) == 2){*/
/*        printf("Protocol specifications are bad, closing . . .\n");*/
/*        */
/*         close(sock);*/
/*        }*/
/*        */
        
        //recvfrom(sock, temp, sizeof(*temp),   0, (struct sockaddr *) &echoServAddr, &serverlen);
        printf("Connected to server %s\n", inet_ntoa(echoServAddr.sin_addr));
        printf("type: %d\n", ntohs(temp.type)); 
        printf("major_version: %d\n", ntohs(temp.major_version)); 
        printf("minor_version: %d\n", ntohl(temp.minor_version)); 
        printf("arith: %d\n", ntohl(temp.arith)); 
        printf("inValue1: %d\n", ntohl(temp.inValue1)); 
        printf("inValue2: %d\n", ntohl(temp.inValue2)); 
        printf("inResult: %d\n", ntohl(temp.inResult)); 
        printf("flValue1: %f\n", (temp.flValue1)); 
        printf("flValue2: %f\n", (temp.flValue2));
        printf("flResult: %f\n", (temp.flResult)); 
        printf("communication id: %d\n", ntohl(temp.id));
        
        
       
        
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
         //printf("sent:%d\n", newMess.protocol);
            if (tempint == -1 ) 
                 tempint = sendto(sock, (struct calcMessage*)&newMess, sizeof(newMess), 0, (struct sockaddr *)
                 &echoServAddr, sizeof(echoServAddr)); 
                 memset(&newMess, 0, sizeof(newMess));
        
                 FD_ZERO( &readfds );        // Reset all bits
                 FD_SET ( sock, &readfds );     // Set the bit that corresponds to
                               // socket s
                 timeout.tv_sec = 2;         // Set the time out value (5 sec)
                 timeout.tv_usec = 0;
                 
                 if ( select ( 32, &readfds, NULL, NULL, &timeout )== 0)
                       if (tempint == -1 ) 
                       tempint = sendto(sock, (struct calcMessage*)&newMess, sizeof(newMess), 0, (struct sockaddr *)
                       &echoServAddr, sizeof(echoServAddr)); 
                       memset(&newMess, 0, sizeof(newMess));
        
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
         //printf("sent:%d\n", newMess.protocol);
/*        if (tempint == -1 ) {*/

/*               printf("Sent struct size: %d\n", tempint);*/
/*              */
/*               DieWithError("sendto() sent a different number of bytes than expected\n");*/
/*        } */
        recvfrom(sock, (struct calcMessage*)&newMess, sizeof(newMess),   0, (struct sockaddr *) &echoServAddr, &serverlen);
        
        printf("message: %d\n", ntohl(newMess.message));
        memset(&newMess, 0, sizeof(newMess));
        //printf("Operation from server is: %s\n", buff);
        //memset(sCN, 0, sizeof(sCN));
        bzero(buff, sizeof(buff));
        close(sock);
        exit(0);
}



