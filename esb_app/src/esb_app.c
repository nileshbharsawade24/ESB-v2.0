// #include <pthread.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/time.h>
// #include "esb.h"
//
// #define MAX_PATH 500
// int send_bmd_path_to_socket(char *msg, const char *socket_file);
// int send_bmd_path_to_socket(char *msg, const char *socket_file){
// 	/**
// 	 * TODO: This function can be implemented in a separate module
// 	 * which will also implement the socket server.
// 	 */
// 	return 1; /* 1 => Success */
// }
//
// /**
//  * Define a suitable struct for holding the endpoint request handling result.
//  */
// typedef struct
// {
// 	int status;
// 	char *bmd_path;
// } endpoint_result;
//
// const char* my_sock_file = "./my_sock";
//
// int esb_endpoint(struct http_request *);
// endpoint_result save_bmd(struct http_request *);
//
// /**
//  * HTTP request handler function mapped in the conf file.
//  */
// int esb_endpoint(struct http_request *req)
// {
// 	printf("Received the BMD request.\n");
// 	endpoint_result epr = save_bmd(req);
// 	if (epr.status < 0)
// 	{
// 		printf("Failed to handle the BMD request.\n");
// 		return (KORE_RESULT_ERROR);
// 	}
// 	else
// 	{
// 		int sock_status = send_bmd_path_to_socket(epr.bmd_path, my_sock_file);
// 		if (sock_status > 0)
// 		{
// 			kore_log(LOG_INFO, "BMD path sent to server socket.");
// 			return (KORE_RESULT_OK);
// 		}
// 		else
// 		{
// 			kore_log(LOG_ERR, "Failed to send BMD path sent to server socket.");
// 			return (KORE_RESULT_ERROR);
// 		}
// 	}
// }
// // char *create_work_dir_for_request();
// char *create_work_dir_for_request()
// {
// 	kore_log(LOG_INFO, "Creating the temporary work folder.");
// 	/**
// 	 * Create a temporary folder in the current directory.
// 	 * Its name should be unique to each request.
// 	 */
// 	char *temp_path = malloc(MAX_PATH * sizeof(char));
// 	char *base_dir = "./bmd_files";
// 	sprintf(temp_path, "%s/%lu", base_dir, (unsigned long)time(NULL));
// 	printf("1------->%s\n",temp_path);
// 	if (mkdir(base_dir, 0700) != 0)
// 	{
// 		printf("2------->%s\n",temp_path);
// 		kore_log(LOG_ERR, "Failed to create folder %s.", base_dir);
// 	}
// 	printf("22------->%s\n",temp_path);
// 	if (mkdir(temp_path, 0700) != 0)
// 	{
// 		printf("3------->%s\n",temp_path);
// 		kore_log(LOG_ERR, "Failed to create folder %s.", temp_path);
// 	}
// 	else
// 	{
// 	printf("4------->%s\n",temp_path);
// 		kore_log(LOG_INFO, "Temporary work folder: %s", temp_path);
// 	}
// 	printf("5------->%s\n",temp_path);
// 	return temp_path;
// }
//
// void do_cleanup(endpoint_result *ep_res_ptr, char *bmd_file_path, int fd)
// {
// 	if (close(fd) == -1)
// 		kore_log(LOG_WARNING, "close(%s): %s", bmd_file_path, errno_s);
//
// 	if (ep_res_ptr->status < 0)
// 	{
// 		kore_log(LOG_ERR, "We got an error. Deleteing the file %s", bmd_file_path);
// 		if (unlink(bmd_file_path) == -1)
// 		{
// 			kore_log(LOG_WARNING, "unlink(%s): %s",
// 					 bmd_file_path, errno_s);
// 		}
// 	}
// 	else
// 	{
// 		ep_res_ptr->status = 1;
// 		printf("BMD is saved\n");
// 	}
// }
//
// void write_data_to_file(struct http_request *req,
// 						endpoint_result *ep_res_ptr,
// 						char *bmd_file_path, int fd,
// 						struct http_file *file)
// {
// 	u_int8_t buf[BUFSIZ];
// 	ssize_t ret, written;
// 	/* While we have data from http_file_read(), write it. */
// 	/* Alternatively you could look at file->offset and file->length. */
// 	for (;;)
// 	{
// 		ret = http_file_read(file, buf, sizeof(buf));
// 		if (ret == -1)
// 		{
// 			kore_log(LOG_ERR, "failed to read from file");
// 			http_response(req, 500, NULL, 0);
// 			ep_res_ptr->status = -1;
// 			goto cleanup;
// 		}
//
// 		if (ret == 0)
// 			break;
//
// 		written = write(fd, buf, ret);
// 		kore_log(LOG_INFO, "Written %d bytes to %s.", written, bmd_file_path);
// 		if (written == -1)
// 		{
// 			kore_log(LOG_ERR, "write(%s): %s",
// 					 bmd_file_path, errno_s);
// 			http_response(req, 500, NULL, 0);
// 			ep_res_ptr->status = -1;
// 			goto cleanup;
// 		}
//
// 		if (written != ret)
// 		{
// 			kore_log(LOG_ERR, "partial write on %s",
// 					 bmd_file_path);
// 			http_response(req, 500, NULL, 0);
// 			ep_res_ptr->status = -1;
// 			goto cleanup;
// 		}
// 	}
//
// cleanup:
// 	do_cleanup(ep_res_ptr, bmd_file_path, fd);
// }
//
// endpoint_result save_bmd(struct http_request *req)
// {
// 	endpoint_result ep_res;
// 	/* Default to OK. 1 => OK, -ve => Errors */
// 	ep_res.status = 1;
//
// 	/* Only deal with POSTs. */
// 	if (req->method != HTTP_METHOD_POST)
// 	{
// 		kore_log(LOG_ERR, "Rejecting non POST request.");
// 		http_response(req, 405, NULL, 0);
// 		ep_res.status = -1;
// 		return ep_res;
// 	}
//
// 	/* Parse the multipart data that was present. */
// 	http_populate_multipart_form(req);
//
// 	/* Find our file. It is expected to be under parameter named bmd_file */
// 	struct http_file *file;
// 	if ((file = http_file_lookup(req, "bmd_file")) == NULL)
// 	{
// 		http_response(req, 400, NULL, 0);
// 		ep_res.status = -1;
// 		return ep_res;
// 	}
//
// 	//printf("succedd");
//
// 	/* Open dump file where we will write file contents. */
// 	char bmd_file_path[MAX_PATH];
// 	char *req_folder = create_work_dir_for_request();
// 	sprintf(bmd_file_path, "%s/%s", req_folder, file->filename);
// 	printf("------------->%s\n",bmd_file_path);
// 	int fd = open(bmd_file_path, O_CREAT | O_TRUNC | O_WRONLY, 0700);
// 	if (fd == -1)
// 	{
// 		kore_log(LOG_ERR, "Failed to create the file at %s\n", bmd_file_path);
// 		http_response(req, 500, NULL, 0);
// 		ep_res.status = -1;
// 		return ep_res;
// 	}
// 	printf("BMD file to be saved at: %s\n", bmd_file_path);
// 	write_data_to_file(req, &ep_res, bmd_file_path, fd, file);
// 	if (ep_res.status > 0)
// 	{
// 		http_response(req, 200, NULL, 0);
// 		kore_log(LOG_INFO, "BMD file '%s' successfully received",
// 				 file->filename);
// 		ep_res.bmd_path = malloc(strlen(bmd_file_path) * sizeof(char) + 1);
// 		strcpy(ep_res.bmd_path, bmd_file_path);
// 	}
// 	return ep_res;
// }
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
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		int x=read(sockfd, buff, sizeof(buff));
		printf("=========================================\n");
		// print buffer which contains the client contents
		printf("%s", buff);

		bzero(buff, MAX);
		sprintf(buff,"GOOD");
		//
		// and send that buffer to client
		write(sockfd, "buff", 5);
		//
		// // if msg contains "Exit" then server exit and chat ended.
		// if (strncmp("exit", buff, 4) == 0) {
		// 	printf("Server Exit...\n");
		// 	break;
		// }
	}
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
