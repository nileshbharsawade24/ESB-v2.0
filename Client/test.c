/*
Author : Deepak kumar
Designation : Senior Member Technical
Employer : Broadridge
Description : This program will test overall application
Helpful link : https://stackoverflow.com/questions/7343833/srand-why-call-it-only-once/
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define NUMBER_OF_MESG_TYPE 1

char BMD_TYPES[][100]={
                    {"user_app_880 BMD CheckStatus"},
                    {"user_app_880 predict_nationality nationality_predictor_880"}
                  };

char name[][10]={{"Rohit"},{"Deepak"},{"Rashid"},{"Suresh"},{"Biden"},{"Sunil"},{"Kiron"},{"chrish"}};

/* taken from https://curl.se/libcurl/c/postit2.html */
void post_data(char * file_name){

    CURL *curl;
    CURLcode res;

    curl_mime *form = NULL;
    curl_mimepart *field = NULL;
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist,"authentication_token:nho2021");

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
      /* Create the form */
      form = curl_mime_init(curl);

      /* Fill in the file upload field */
      field = curl_mime_addpart(form);
      curl_mime_name(field, "bmd_file");
      curl_mime_filedata(field, file_name);

      /* what URL that receives this POST */
      curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8888");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
      curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
      curl_easy_setopt(curl, CURLOPT_HTTP09_ALLOWED, 1);

      /* Perform the request, res will get the return code */
      res = curl_easy_perform(curl);
      /* Check for errors */
      if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

      /* always cleanup */
      curl_easy_cleanup(curl);

      /* then cleanup the form */
      curl_mime_free(form);
      /* free slist */
      curl_slist_free_all(headerlist);
    }
}

void *test_for_different_msg_type(void * t){
  int temp=*((int*)t);
  char * bmd_type = BMD_TYPES[temp];
  char* sender=malloc(100*sizeof(char));
  char* destination=malloc(100*sizeof(char));
  char* message_type=malloc(100*sizeof(char));
  char * field=strtok(bmd_type," ");
  int i=0;
  while(field!=NULL){
    switch(i++){
      case 0:
        sender=field;
        break;
      case 1:
        destination=field;
        break;
      case 2:
        message_type=field;
        break;
      default:
        printf("Not expected\n");
        break;
    }
    field=strtok(NULL," ");
  }

  // Use current time as seed for random generator
  srand(time(0));
  for(int i=0;i<10;i++){
    int number_of_different_payload=sizeof(name)/sizeof(name[0]);;
    unsigned int rnd=rand()%number_of_different_payload;
    char * file_name=malloc(100*sizeof(char));
    sprintf(file_name,"BMD/BMD_TYPE_%d_NO_%d",temp,rnd);
    FILE * fp=fopen(file_name,"w");
    char * payload=name[rnd];
    fprintf(fp,
"<BMD>\n\
    <Envelop>\n\
        <MessageID>A9ECAEF2-107A-4452-9553-043B6D25386E</MessageID>\n\
        <MessageType>%s</MessageType>\n\
        <Sender>%s</Sender>\n\
        <Destination>%s</Destination>\n\
        <CreationDateTime>2020-08-12T05:18:00+0000</CreationDateTime>\n\
        <Signature>63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c</Signature>\n\
        <ReferenceID>INV-PROFILE-889712</ReferenceID>\n\
    </Envelop>\n\
    <Payload>%s</Payload>\n\
</BMD>",\
    message_type,sender,destination,payload);
    fclose(fp);
    post_data(file_name);
    // remove(file_name);
  }

}

int main(int argc, char const *argv[]) {
  //defining thread
	pthread_t threads[NUMBER_OF_MESG_TYPE];
  unsigned int count=0;
  for(int i=0;i<NUMBER_OF_MESG_TYPE;i++) {
    if(pthread_create(&threads[i],NULL,&test_for_different_msg_type,&i)!=0){
      printf ("ERROR: child thread not created\n");
      exit(-1);
    }
  }
  // waiting to terminate all NUM_THREADS thread
  for(int i=0;i<NUMBER_OF_MESG_TYPE;i++){
    pthread_join(threads[i], NULL);
  }
  return 0;
}
