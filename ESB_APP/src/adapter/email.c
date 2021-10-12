

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "email.h"

#define FROM_ADDR "<esbtest321@gmail.com>"
#define CC_ADDR "<rohitbhamu6@gmail.com>"
#define TO_ADR "<esb.proj2021.bat@gmail.com>"

void transport_for_email_service_123(bmd * msg)
{
  //text is file path for JSON
  printf("in email function\n");
  static int i=0;
  i++;
  char filename[50];

  //char *file=filename;
  char op[20001];
  snprintf(filename, sizeof(filename), "output_%d.html", i);
  snprintf(op, sizeof(op), "<!DOCTYPE html>\n\t<html>\n\t\t<head>\n\t\t\t<meta charset='UTF-8'>\n\t\t\t\t<title>XML TO HTML</title>\n\t\t</head>\n\t<body>\n\t\t<table boarder=1>\n\t\t\t<thead>\n\t\t\t\t<tr>\n\t\t\t\t\t<th>Source: %s</th>\n\t\t\t\t</tr>\n\t\t\t\t<tr>\n\t\t\t\t\t<th>Payload: %s</th>\n\t\t\t\t</tr>\n\t\t\t</thead>\n\t\t\t<tbody>\n\t\t\t</tbody>\n\t\t</table>\n\t</body>\n</html>\n", msg->envelop.Sender, msg->payload);
  printf("successfully transformed to HTML, filename=%s\n",filename);
  FILE *fp = fopen(filename, "w"); //open that file
  fprintf(fp, "%s", op);
  fclose(fp);
  char *file = filename;    // transformation done


  printf("Sending mail to %s\n", TO_ADR);

  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;

  curl = curl_easy_init();
  if (curl)
  {
    /* This is the URL for your mailserver */
    curl_easy_setopt(curl, CURLOPT_USERNAME, "esbtest321@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "testesb@321"); //enter password

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587/");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);

    recipients = curl_slist_append(recipients, TO_ADR);
    recipients = curl_slist_append(recipients, CC_ADDR);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    //JSON file to be send
    char *filepath = file;
    FILE *fd = fopen(filepath, "r");
    //         curl_easy_setopt(curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(curl, CURLOPT_READDATA, fd);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Send the message */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      //return -1;
    }
    /* Free the list of recipients */
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
  }
  printf("\n********| \"Mail sent successfully\" |********\n");
  //free(file);
  fclose(fp);
  //remove();
  // free(filename);
  // return 0;
}

void transform_for_email_service_123(bmd* msg){
  msg->payload=msg->payload;
  return;
}
