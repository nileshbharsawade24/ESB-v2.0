// ref : https://stackoverflow.com/questions/44068549/setting-socket-timeout-for-receive-function
// ref : https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
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

#include "parser/http_parser.h"
#include "parser/xml_parser.h"
#include "../database_handler/database_access.h"

#define MAX 102400
#define PORT 8888
#define SA struct sockaddr
#define mysql_user_name "test_user"
#define mysql_user_password "test_password"
#define mysql_host "localhost"
#define mysql_db_name "CAMEL_DB"

void serve(int sockfd)
{
	struct timeval tv;
	tv.tv_sec = 2;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	char buff[MAX];
	bzero(buff, MAX);
	char * filename="http_request.txt";
	FILE * fp=fopen(filename,"w");
	while (recv(sockfd, buff, MAX, 0)>=0) {
    fprintf(fp,"%s",buff);
  }
	fclose(fp);
	printf("---------FILE RECIEVED------------------\n");
	//parse http request
	parse_http_request(filename);
	//parse xml
	bmd * req=parse_xml("BMD.xml");
	//store parsed xml in esb_request table
	MYSQL *con = mysql_init(NULL);
  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, mysql_host, mysql_user_name, mysql_user_password,
          mysql_db_name, 0, NULL, 0) == NULL)
  {
        handle_error(con);
  }

  insert_one_in_esb_request(con,"1",req->Sender,req->Destination,req->MessageType,req->ReferenceID,req->MessageID,"9999-12-31 23:59:59","-","Available","-");
	//do all other operation in sequence
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

	// while (1) {
		// Accept the data packet from client and verification
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			printf("server acccept failed...\n");
			exit(0);
		}
		else
			printf("server acccept the client...\n");

		// Function for chatting between client and server
		serve(connfd);
	// }

	// After chatting close the socket
	close(sockfd);
}