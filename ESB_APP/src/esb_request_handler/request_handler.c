/*
Author : Deepak kumar and Rohit bhamu
Designation : Senior Member Technical
Employer : Broadridge
Description : This c program will act like a server
Note : Configure the port and database credential appropriately
Helpful link : https://stackoverflow.com/questions/45339874/how-to-make-fprintf-writes-immediately
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
#include <ctype.h>
#include <pthread.h>

#include "parser/http_parser.h"
#include "parser/xml_parser.h"
#include "../database_handler/database_access.h"

#define ESB_AUTHENTICATION_TOKEN "nho2021"
#define MAX 3072
#define PORT 1111
#define PATH_MAX 50
#define NUM_THREADS 5
#define SA struct sockaddr


bool perform_request_authentication_and_validation(int sockfd,char * buffer){
  if(!is_request_http_post(buffer)){
    char * reply=malloc(500*sizeof(char));
    sprintf(reply,"-----------------------------------------------------------------------\n"
                  "Your request has been REJECTED because it is not HTTP POST request.\n"
                  "TRY AGAIN with HTTP POST.\n"
                  "Thanks for using our ESB SERVICE.:)\n"
                  "-----------------------------------------------------------------------\n");
    send(sockfd, reply, strlen(reply),0);
    free(reply);
    return false;
  }
  char* request_authentication_token=get_field_from_request(buffer,"authentication_token");
  //here is a crack with strcmp
  if(strncmp(ESB_AUTHENTICATION_TOKEN,request_authentication_token,strlen(ESB_AUTHENTICATION_TOKEN))!=0){
    char * reply=malloc(500*sizeof(char));
    sprintf(reply,"-----------------------------------------------------------------------\n"
                  "Your request has been REJECTED because of authentication_token.\n"
                  "TRY AGAIN with correct authentication_token.\n"
                  "Thanks for using our ESB SERVICE.:)\n"
                  "-----------------------------------------------------------------------\n");
    send(sockfd, reply, strlen(reply),0);
    free(reply);
    free(request_authentication_token);
    return false;
  }
  free(request_authentication_token);
  return true;
}

char * persist_BMD(char * buff,int sockfd){
  unsigned long tm=(unsigned long)time(NULL);
	char *filename_http = malloc(PATH_MAX * sizeof(char));
	char *filename_xml = malloc(PATH_MAX * sizeof(char));
	char *base_dir = "./tmp";
	sprintf(filename_http, "%s/HTTP_REQUEST_%d_%lu.txt", base_dir, sockfd, tm);
	sprintf(filename_xml, "%s/BMD_%d_%lu.xml", base_dir, sockfd, tm);
	FILE * fp=fopen(filename_http,"w"); //opening http_request.tt file in writing mode
  fprintf(fp,"%s",buff);
  fflush(fp);
  fclose(fp);
  // parse http request
  parse_http_request(filename_http,filename_xml);
  //parse xml
  bmd * req=parse_xml(filename_xml);
  //checking special case
  if(strcmp(req->envelop.Destination,"ESB")==0 && strcmp(req->envelop.MessageType,"CheckStatus")==0){
    char * status=malloc(20*sizeof(char));
    strcpy(status,select_single_field_on_one_condition("esb_request","status","id",req->envelop.ReferenceID));
    for(char *p = status; *p; p++)*p=toupper(*p);
    char * reply=malloc(500*sizeof(char));
    sprintf(reply,"-----------------------------------------------------------------------\n"
                  "STATUS regarding your Correlation Id \"%s\" ---> %s.\n"
                  "Thanks for using our ESB SERVICE.:)\n"
                  "-----------------------------------------------------------------------\n",req->envelop.ReferenceID,status);
    send(sockfd, reply, strlen(reply),0);
    remove(filename_http);
    free(filename_http);
    free(filename_xml);
    free(reply);
    free(req);
    free(status);
    return "special_case";
  }
  // inserting a tuple in esb_request table with given fields
  char * req_id=insert_one_in_esb_request(req->envelop.Sender,req->envelop.Destination,req->envelop.MessageType,req->envelop.ReferenceID,\
    req->envelop.MessageID,"now()",filename_xml,"Available","0","-");
  remove(filename_http);
  free(filename_http);
  free(filename_xml);
  free(req);
  return req_id;
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
	while (read(sockfd, temp, 1024)>=0) {
    temp=temp+strlen(temp);
  }
  *temp='\0';
  //printf("buffer : %s\n",buffer);
	printf("\nClient Socket Id %d Request came.\n",sockfd);
  //incomming HTTP POST request authentication and validation
  if(perform_request_authentication_and_validation(sockfd,buffer)){
    //persist BMD
    char * correlation_id=persist_BMD(buffer,sockfd);
    if(strcmp(correlation_id,"special_case")!=0){
      char * reply=malloc(500*sizeof(char));
      sprintf(reply,"-----------------------------------------------------------------------\n"
                    "Your request has been SUBMITTED.\n"
                    "\"%s\" is your Correlation Id to check the status later.\n"
                    "Thanks for using our ESB SERVICE.:)\n"
                    "-----------------------------------------------------------------------\n",correlation_id);
      //send acknowledege to client
      send(sockfd, reply, strlen(reply),0);
      free(reply);
    }
  }
  free(buffer);
  //close the client socket
	close(sockfd);
  free(fd);
  printf("Closed Client with Socket Id %d...\n\n",sockfd);
	pthread_exit(NULL);
}

// Driver function
void handle_request()
{

	int sockfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created...\n");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
    		fprintf(stderr,"ERROR : setsockopt(SO_REUSEADDR) failed\n");
  	}

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
		printf("Server socket successfully binded...\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Server Listen failed...\n");
		exit(0);
	}
	else{
		printf("Server listening on PORT NO %d...\n",PORT);
  }
	len = sizeof(cli);

	//defining thread
	pthread_t threads[NUM_THREADS];

	printf("\n============= WELCOME IN CAMEL ESB ==================\n\n");

  unsigned int count=0;

	// keep listening for new clients
	while(1){
		 // Accept the client socket connection
		 int * connfd = malloc(sizeof(int));
     *connfd= accept(sockfd, (SA*)&cli, &len);
		 if (connfd < 0) {
			 printf("server acccept failed...\n");
			 exit(0);
		 }
		 else
			 printf("server acccept the client Socket Id %d ...\n",*connfd);

		 // assign different client to different thread
		 if(pthread_create(&threads[count%NUM_THREADS],NULL,serve,connfd)!=0){
			 printf ("ERROR: child thread not created\n");
			 exit(-1);
		 }
     count++;
  }

	// close the server socket
	close(sockfd);

	// waiting to terminate all NUM_THREADS thread
  for(int i=0;i<NUM_THREADS;i++){
    pthread_join(threads[i], NULL);
  }
}

/* for testing purpose  only */
// int main(int argc, char const *argv[]) {
//   handle_request();
//   return 0;
// }
