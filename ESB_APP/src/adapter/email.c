
/* 
* Author : Rohit Kumar Bhamu
* Designation : Senior Technical Member
* Employer : Broadridge
*
*ref : https://github.com/curl/curl/blob/master/docs/examples/smtp-mail.c
*fill sender and destination mail addres in respective fields
*use -lcurl in command line for running the code
*
*/


#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <curl/curl.h>
#include <time.h>
#include "./emails.h"

#define PATH_MAX 50


typedef struct {
    char* sender;
    char* destination;
    char* message_type;
    char* message_id;
    char* signature;
    char* reference_id;
    char* create_on;
} bmd_envelop;

typedef struct {
    bmd_envelop envelop;
    char* payload;
} bmd;

char * transform_for_email_service_123(bmd * msg){
    //writing into text file which will be needed
    //right now returning file_name.. [DOUBT about generalizing return type]

    unsigned long tm=(unsigned long) time(NULL);
    char *filename_txt=malloc(PATH_MAX*sizeof(char)); //define PATH_MAX
    char *temp_dir="./temp";
    
    sprintf(filename_txt, "%s/mail_%lu.txt",temp_dir,tm);

    FILE* fp=fopen(filename_txt,"w"); //opening the mail.txt in writing mode_t

    //write in file
    fprintf(fp,
    "From: \"ESB Application\" <esbtest321@gmail.com>\n\
    To: \"Destination Application\" <%s>\n\
    Subject: Trail mail\n\
    \n\
    %s\n",\
    msg->envelop.destination,msg->payload);

    //close the file pointer
    fclose (fp);

return filename_txt; //returning the name of newly created mail.txt file

}

void transport_for_email_service_123 (char* file_name){
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *recipients;
    FILE *fd;
    
    recipients = NULL;
    recipients = curl_slist_append(recipients, "rohitbhamu6@gmail.com");
    
    fd = fopen(file_name, "rb");
    if (!fd) { fprintf(stderr, "ERROR : Invalid mail request failed: %s",curl_easy_strerror(response_code));
      }
      printf("\n\n");
      

    hnd = curl_easy_init();

    char smtp_path[] = "smtps://smtp.gmail.com:465/";
    char filename[] = file_name;
    strcat(smtp_path,file_name);

    curl_easy_setopt(hnd, CURLOPT_INFILESIZE_LARGE, (curl_off_t)179);
    curl_easy_setopt(hnd, CURLOPT_URL, smtp_path);
    curl_easy_setopt(hnd, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(hnd, CURLOPT_READDATA, fd); 
    curl_easy_setopt(hnd, CURLOPT_USERPWD, "esbtest321@gmail.com:testesb@321");
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.68.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_MAIL_FROM, "esbtest321@gmail.com");
    curl_easy_setopt(hnd, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

    ret = curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(recipients);
    recipients = NULL;


}

// int main(void)
// {
//     CURLcode ret;
//     CURL *hnd;
//     struct curl_slist *recipients;
//     FILE *fd;
    
//     recipients = NULL;
//     recipients = curl_slist_append(recipients, "rohitbhamu6@gmail.com");
    
//     fd = fopen("mail.txt", "rb");
//     if (!fd) {return 1;} 

//     hnd = curl_easy_init();
//     curl_easy_setopt(hnd, CURLOPT_INFILESIZE_LARGE, (curl_off_t)179);
//     curl_easy_setopt(hnd, CURLOPT_URL, "smtps://smtp.gmail.com:465/mail.txt");
//     curl_easy_setopt(hnd, CURLOPT_UPLOAD, 1L);
//     curl_easy_setopt(hnd, CURLOPT_READDATA, fd); 
//     curl_easy_setopt(hnd, CURLOPT_USERPWD, "esbtest321@gmail.com:testesb@321");
//     curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.68.0");
//     curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
//     curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
//     curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
//     curl_easy_setopt(hnd, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
//     curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
//     curl_easy_setopt(hnd, CURLOPT_MAIL_FROM, "esbtest321@gmail.com");
//     curl_easy_setopt(hnd, CURLOPT_MAIL_RCPT, recipients);
//     curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

//     ret = curl_easy_perform(hnd);

//     curl_easy_cleanup(hnd);
//     hnd = NULL;
//     curl_slist_free_all(recipients);
//     recipients = NULL;

//     return (int)ret;
// }