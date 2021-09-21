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

#define MAX 102400
#define PORT 8888
#define SA struct sockaddr

void parse_http_request(char const * input_fpath){
  char * output_fpath="BMD.xml";
  FILE * f_r=fopen(input_fpath,"r");
  FILE * f_w=fopen(output_fpath,"w");

  if (f_r == NULL)return;

  int last_idx=-1;
  int i=0;
  char c;
  while((c=fgetc(f_r))!=EOF){
    if(c=='>')last_idx=i;
    i++;
  }

  f_r=fopen(input_fpath,"r");
  i=0;

    bool start=false;
    while((c=fgetc(f_r))!=EOF){
      if(start){
        fputc(c,f_w);
        if(i==last_idx)start=false;
      }
      else{
        if(c=='<'){
          fputc(c,f_w);
          start=true;
        }
      }
      i++;
    }

  fclose(f_r);
  fclose(f_w);
}

void func(int sockfd)
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
	parse_http_request(filename);
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

	while (1) {
		// Accept the data packet from client and verification
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			printf("server acccept failed...\n");
			exit(0);
		}
		else
			printf("server acccept the client...\n");

		// Function for chatting between client and server
		func(connfd);
	}

	// After chatting close the socket
	close(sockfd);
}
