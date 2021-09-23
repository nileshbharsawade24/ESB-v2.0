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
#include <mysql.h>
#include <time.h>
#include<pthread.h>

#include "parser/http_parser.h"
#include "parser/xml_parser.h"
#include "../database_handler/database_access.h"

#define MAX 102400
#define PORT 8889
#define PATH_MAX 50
#define NUM_THREADS 10
#define SA struct sockaddr
#define mysql_user_name "test_user"
#define mysql_user_password "test_password"
#define mysql_host "localhost"
#define mysql_db_name "CAMEL_DB"

//this will serve to a client given socket file descriptor 'sockfd'
void serve(int sockfd)
{
	struct timeval tv;
	tv.tv_sec = 2; //2 second timer
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	char buff[MAX];//this buff will store the data from http request and we will use it to write in .txt file
	bzero(buff, MAX);
	unsigned long tm=(unsigned long)time(NULL);
	char *filename_http = malloc(PATH_MAX * sizeof(char));
	char *filename_xml = malloc(PATH_MAX * sizeof(char));
	char *base_dir = "./tmp";
	sprintf(filename_http, "%s/HTTP_REQUEST_%lu.txt", base_dir, tm);
	sprintf(filename_xml, "%s/BMD_%lu.xml", base_dir, tm);
	FILE * fp=fopen(filename_http,"w"); //opening http_request.tt file in writing mode

	while (recv(sockfd, buff, MAX, 0)>=0) {
    fprintf(fp,"%s",buff); //writing in file
  }
	fclose(fp);
	printf("---------FILE RECIEVED------------------\n");
	//parse http request
	parse_http_request(filename_http,filename_xml);
	//parse xml
	bmd * req=parse_xml(filename_xml);
	//store parsed xml in esb_request table
	MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

	//connecting to db
  if (mysql_real_connect(con, mysql_host, mysql_user_name, mysql_user_password,
          mysql_db_name, 0, NULL, 0) == NULL)
  {
        handle_error(con);
  }

	//inserting a tuple in esb_request table with given fields
  insert_one_in_esb_request(con,"1",req->Sender,req->Destination,req->MessageType,req->ReferenceID,req->MessageID,"9999-12-31 23:59:59","-","Available","-");
	//do all other operation in sequence...
	//close the client socket
	close(sockfd);
}

// Driver function
int main()
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
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	//keep listening for new clients
	 while (1) {
		// Accept the data packet from client and verification
		printf("Server is listening...\n");
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			printf("server acccept failed...\n");
			exit(0);
		}
		else
			printf("server acccept the client...\n");



		//----Mutrithreading from here----
		
		pthread_t threads[NUM_THREADS];
		int rc; //return code of thread
		long t;

		for(t=0;t<NUM_THREADS;t++){
			printf("creaed child thread \n");
			//Child thread created
			rc=pthread_create(&threads[t],NULL,serve,(void *) t);
			if(rc){
				printf ("ERROR; return code from pthread_create() is %d\n",rc);
				exit(-1);

			}
		}
		 
		 pthread_exit(NULL);
		// Function for chatting between client and server
		serve(connfd);
	// }

	// After chatting close the socket
	close(sockfd);
}
