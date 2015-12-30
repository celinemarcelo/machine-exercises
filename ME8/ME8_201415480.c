#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/tcp.h>

int main(int argc, char **argv) {

   /*Parsing of index.html file*/
   char response[] = "HTTP/1.1 200 OK\r\n"
                       "Connection: close\r\n"
                       "Content-type: text/html\r\n"
                       "\r\n";
   
   int l = 0;
   char temp[51] = {};
   char c;
   FILE *fp = fopen("index.html", "r");

   if (!fp) {
      perror(NULL);
      exit(-1);
   }

   while (c != EOF) {
      c = fgetc(fp);
      if (c != EOF) {
         fseek(fp, -1, SEEK_CUR);
      } else {
         break;
      }
      fgets(temp, 50, fp);
      strcat(response, temp);
   }

   strcat(response, "\r\n");

   fclose(fp);
   puts (response);

   /*Main program*/
   int listenfd, connfd;                                                      //fds for listener and new connections
   struct sockaddr_in6 servaddr, cliaddr[3];                                     //structs for server and client information
   socklen_t len = sizeof(cliaddr);                                           //other necessary variables
   char cli_ip[32];
   char mesg[1024];
   char cport[10] = {};
   char *holder;
   char a[2];
   int pid = 0;
   int i = 0, j = 0, k = 0;
   int fdmax;
   int clients[3] = {0};
   char strport[10];

   fd_set masterfds;                                                          //declaration of fd sets to be monitored by select
   fd_set readfds;

   holder = malloc(1024);

   pid = getpid();                                                            //monitoring of PID
   printf("PID = %d\n", pid);
   strcpy(cport, *(argv + 1));


   listenfd = socket(PF_INET6, SOCK_STREAM, 0);                               //initialization of listener


   bzero(&servaddr, sizeof(servaddr));                                        //clearing of sockaddr_in6 structs
   
   for(i = 0; i <= 3; i++){
      bzero(&cliaddr[i], sizeof(cliaddr[i]));
   }
   servaddr.sin6_family = AF_INET6;                                           //necessary IPv6 declarations
   servaddr.sin6_addr = in6addr_any;
   servaddr.sin6_port = htons(atoi(cport));


   if ( bind( listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr) ) < 0 ) { //binding of socket to server
      perror(NULL);
      exit(-1);
   }

   printf("Port %s opened for possible connections.\n", cport);

   //not present in udp server
   if (listen(listenfd, 3) < 0) {                                              //listening
      perror(NULL);
      close(listenfd);
      exit(-1);
   }

   puts("Waiting for incoming connections...");

   //FD_ZERO(&masterfds);                                                         //initialization of fd sets
   FD_ZERO(&readfds);
   fdmax = listenfd;                                                            //listenfd is the highest fd so far (keeping track of max fd is imperative for select)
   FD_SET(listenfd, &masterfds);                                                //insertion of listenfd to master set

   while (1) {
      //printf("fdmax = %d\n", fdmax);

      for (k = 0; k <= fdmax; k++) {                                            //go through fds to handle either new connections or parse requests from existing clients

         readfds = masterfds;

         if (select(fdmax + 1, &readfds, NULL, NULL, NULL) == -1) {             //select statement which is the heart of this multiple client program
            perror(NULL);
            exit(-1);
         }

         if (FD_ISSET(k, &readfds)) {                                           //check if current fd is in readfds
            if (k == listenfd) {                                                //new connection
               //printf("LISTENFD\n");


               connfd = accept(listenfd, (struct sockaddr *)&cliaddr[l], &len);    //establish connection with new client

               if (connfd == -1) {                                              //error checking
                  perror(NULL);
                  exit(-1);
               }

               FD_SET(connfd, &masterfds);                                       //insert new fd to master set


               sprintf(strport, "%d", ntohs(cliaddr[l].sin6_port));
               strcat(strport, "<---PORT NUMBER");
               puts(strport);

               if (connfd > fdmax) {                                             //check if new fd is now max
                  fdmax = connfd;
               }

               inet_ntop(AF_INET6, (struct in_addr *) &cliaddr[l].sin6_addr, cli_ip, sizeof(cli_ip) );   //conversion of numeric address to string

               for (i = 0; clients[i] != 0; i++);                                //loop for naming of client
                  clients[i] = connfd;


               printf("(Client %d) New connection from %s on port %d.\n", i + 1, cli_ip, ntohs(cliaddr[l].sin6_port));


               if (send(connfd, response, strlen(response), 0) != strlen(response)) { //sending of response
                  puts("HERE");
                  perror(NULL);
               }

               fflush(stdout);

               fputs("\n", stdout);

               puts("SENDING DONE");

               printf("(Client %d) CHECK  New connection from %s on port %d.\n", i + 1, cli_ip, ntohs(cliaddr[l].sin6_port));

               //l++;
               //bzero(&cliaddr[l], sizeof(cliaddr[l]));

            } else {                                                             //parsing of request from existing client

               memset(mesg, 0, sizeof(mesg));
               if (read(k, mesg, sizeof(mesg)) > 0 ) {   //getting the request

                  //parsing of message

                  strcpy(holder, mesg);
                  memset(mesg, 0, strlen(mesg));

                  holder += 4;

                  for (i = 0, j = 0; * (holder + i) != ' '; i++, j++) {
                     if (*(holder + i) == '+') { //parse space
                        *(mesg + j) = ' ';
                     } else if (*(holder + i) == '%') { // parse hex values
                        a[0] = *(holder + i + 1);
                        a[1] = *(holder + i + 2);
                        *(mesg + j) = strtol(a, NULL, 16);
                        i += 2;
                     } else { // parse content
                        *(mesg + j) = *(holder + i);
                     }
                  }


                  printf("Request from %s on port %d: \"%s\"\n", cli_ip, ntohs(cliaddr[l].sin6_port), mesg);
               } else {                                                     //client disconnected


                  getpeername(k , (struct sockaddr *)&cliaddr[l] , &len);

                  for (i = 0; clients[i] != k; i++);                        //loop for naming of client
                     clients[i] = 0;

                  printf("Client %d disconnected: ip %s, port %d.\n", i + 1, cli_ip, ntohs(cliaddr[l].sin6_port));
                  close(k);                                                 //closing of current fd
                  FD_CLR(k, &masterfds);                                    //deleting current fd from master set
                  fflush(stdout);
               }
            }
         }
      }
   }

   close(listenfd);                                                         //closing of listener
   return 0;
}