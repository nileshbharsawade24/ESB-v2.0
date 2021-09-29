/*
Author : Deepak kumar and Rohit bhamu
Designation : Senior Member Technical
Employer : Broadridge
Description : This c program will act like a server
Note : Configure the port and database credential appropriately
*/

#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#include "parser/http_parser.h"
#include "parser/xml_parser.h"
#include "../database_handler/database_access.h"

#define ESB_AUTHENTICATION_TOKEN "nho2021"
#define MAX 10240
#define PORT 8889
#define PATH_MAX 50
#define NUM_THREADS 5
#define SA struct sockaddr


bool perform_request_authentication_and_validation(int sockfd,char * buffer){
  if(!is_request_http_post(buffer)){
    char * reply=malloc(500*sizeof(char));
    sprintf(reply,"Your request has been REJECTED because it is not HTTP POST request.\nTRY AGAIN with HTTP POST\n");
    send(sockfd, reply, strlen(reply),0);
    // printf("%s\n", );
    // fprintf(stderr, "ERROR : client id %d request is not HTTP POST\n",sockfd);
    // close(sockfd);
  	// pthread_exit(NULL);
    free(reply);
    return false;
  }
  char* request_authentication_token=get_field_from_request(buffer,"authentication_token");
  //here is a crack with strcmp
  if(strncmp(ESB_AUTHENTICATION_TOKEN,request_authentication_token,strlen(ESB_AUTHENTICATION_TOKEN))!=0){
    char * reply=malloc(500*sizeof(char));
    sprintf(reply,"Your request has been REJECTED because of authentication_token.\nTRY AGAIN with correct authentication_token\n");
    send(sockfd, reply, strlen(reply),0);
    // printf("------------------->%ld\n",strlen(reply));
    // fprintf(stderr, "ERROR : client id %d have problem with AUTHENTICATION TOKEN\n",sockfd);
    // close(sockfd);
  	// pthread_exit(NULL);
    free(reply);
    return false;
  }
  return true;
}

void persist_BMD(char * buff){
  unsigned long tm=(unsigned long)time(NULL);
	char *filename_http = malloc(PATH_MAX * sizeof(char));
	char *filename_xml = malloc(PATH_MAX * sizeof(char));
	char *base_dir = "./tmp";
	sprintf(filename_http, "%s/HTTP_REQUEST_%lu.txt", base_dir, tm);
	sprintf(filename_xml, "%s/BMD_%lu.xml", base_dir, tm);
	FILE * fp=fopen(filename_http,"w"); //opening http_request.tt file in writing mode
  fprintf(fp,"%s",buff);
  fclose(fp);
  // parse http request
  parse_http_request(filename_http,filename_xml);
  //parse xml
  bmd * req=parse_xml(filename_xml);
  // inserting a tuple in esb_request table with given fields
  insert_one_in_esb_request(req->Sender,req->Destination,req->MessageType,req->ReferenceID,req->MessageID,"now()",filename_xml,"Available","0","-");
  remove(filename_http);
  free(filename_http);
  free(filename_xml);
}

//this will serve to a client given socket file descriptor 'sockfd'
void *serve(void* fd) {
  int sockfd=*(int*)fd;
	struct timeval tv;
	tv.tv_sec = 2; //2 second timer
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

  char * temp=malloc(MAX*sizeof(char));
  char * buffer=temp;
	bzero(buffer, MAX);
	while (strlen(buffer)+1024<10240 && read(sockfd, temp, 1024)>=0) {
    temp=temp+strlen(temp);
  }
	printf("\nClient id %d Request came.\n",sockfd);
  //incomming HTTP POST request authentication and validation
  if(perform_request_authentication_and_validation(sockfd,buffer)){
    //persist BMD
    persist_BMD(buffer);
    //send acknowledege to client
    char * reply=malloc(100*sizeof(char));
    sprintf(reply,"Your request has been ACCEPTED.\nThanks for using our ESB SERVICE.:)\n");
    send(sockfd, reply, strlen(reply),0);
    free(reply);
  }

  free(buffer);
  //close the client socket
	close(sockfd);
  printf("Closed client with socket id %d...\n\n",sockfd);
	pthread_exit(NULL);
}

// Driver function
void handle_request()
{

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Server socket bind failed...\n");
		exit(0);
	}
	else
		printf("Server socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Server Listen failed...\n");
		exit(0);
	}
	else{
		printf("Server listening on PORT NO %d ...\n",PORT);
  }
	len = sizeof(cli);

	//defining thread
	pthread_t threads[NUM_THREADS];

	printf("\n============= WELCOME IN CAMEL ESB ==================\n\n");

  unsigned int count=0;

	//keep listening for new clients
	while(1){
		 // Accept the data packet from client and verification
		 connfd = accept(sockfd, (SA*)&cli, &len);
		 if (connfd < 0) {
			 printf("server acccept failed...\n");
			 exit(0);
		 }
		 else
			 printf("server acccept the client with id %d ...\n",connfd);

		 //----create child thread----
		 if(pthread_create(&threads[count%NUM_THREADS],NULL,&serve,&connfd)!=0){
			 printf ("ERROR: child thread not created\n");
			 exit(-1);
		 }
     count++;
  }

	// close the server socket
	close(sockfd);

	/*this is the last thing main() should do */
	pthread_exit(NULL);
}

// int main(int argc, char const *argv[]) {
//   handle_request();
//   return 0;
// }
