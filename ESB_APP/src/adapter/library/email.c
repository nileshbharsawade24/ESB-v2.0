/* ref : https://github.com/curl/curl/blob/master/docs/examples/smtp-mail.c
*fill sender and destination mail addres in respective fields
*use -lcurl in command line for running the code
*you should extract data from BMD file in the mail.txt file
*/


#include<stdio.h>
#include<stdlib.h>
#include <curl/curl.h>

int main(void)
{
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *recipients;
    FILE *fd;
    
    recipients = NULL;
    recipients = curl_slist_append(recipients, "recipients@gmail.com");
    
    fd = fopen("mail.txt", "rb");
    if (!fd) {return 1;} 

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_INFILESIZE_LARGE, (curl_off_t)179);
    curl_easy_setopt(hnd, CURLOPT_URL, "smtps://smtp.gmail.com:465/mail.txt");
    curl_easy_setopt(hnd, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(hnd, CURLOPT_READDATA, fd); 
    curl_easy_setopt(hnd, CURLOPT_USERPWD, "sender@gmail.com:password");
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.68.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_MAIL_FROM, "sender@gmail.com");
    curl_easy_setopt(hnd, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

    ret = curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(recipients);
    recipients = NULL;

    return (int)ret;
}

//#include<stdio.h>
//
//void send(char *to, char *text) {
//    printf("Sending to %s\n", to);
//}
