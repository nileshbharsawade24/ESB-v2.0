<<<<<<< HEAD

=======
>>>>>>> 321dfd26534674a8671b5ed538c19a479f7f8f7a
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 102400
#define PORT 8889
#define SA struct sockaddr


 
// Function designed for chat between client and server.
void func(int sockfd)
{
    // FILE* fp = fopen("test.txt","w");

<<<<<<< HEAD
	// char buff[MAX];
	// int n;

    // struct timeval tv;
	// tv.tv_sec = 2;
	// setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	// // infinite loop for chat
    //  for (int i =0;i<5;i++) {
	// 	bzero(buff, MAX);

	// 	// read the message from client and copy it in buffer
	// 	int x=read(sockfd, buff, sizeof(buff));
	// 	printf("=========================================\n");
	// 	// print buffer which contains the client contents
	// 	printf("%s", buff);
    //     fprintf(fp,"%s",buff);
        
	// }
    // close(sockfd);
    // fclose(fp);

    struct timeval tv;
	tv.tv_sec = 2;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	char buff[MAX];
	bzero(buff, MAX);
	FILE * fp=fopen("test.txt","w");
	fclose(fp);
	fp=fopen("test.txt","w");
	while (recv(sockfd, buff, MAX, 0)>=0) {
    fprintf(fp,"%s",buff);
  }
	fclose(fp);
	close(sockfd);
=======
		// read the message from client and copy it in buffer
		int x=read(sockfd, buff, sizeof(buff));
		printf("=========================================\n");
		// print buffer which contains the client contents
		printf("%s", buff);
	}
>>>>>>> 321dfd26534674a8671b5ed538c19a479f7f8f7a
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

	// After chatting close the socket
	close(sockfd);
}