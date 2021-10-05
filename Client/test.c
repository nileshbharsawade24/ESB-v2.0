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

#define NUMBER_OF_MESG_TYPE 2
#define NUMBER_OF_BMD_GENERATED_PER_BMD_TYPE 10

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

char BMD_TYPES[][100]={
                    {"user_app_880 nationality_predictor_880 predict_nationality"},
                    {"user_app_880 ESB CheckStatus"}
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

void build_bmd_file(char * file_name,bmd * msg){
  FILE * fp=fopen(file_name,"w");
  fprintf(fp,
"<BMD>\n\
  <Envelop>\n\
      <MessageID>%s</MessageID>\n\
      <MessageType>%s</MessageType>\n\
      <Sender>%s</Sender>\n\
      <Destination>%s</Destination>\n\
      <CreationDateTime>%s</CreationDateTime>\n\
      <Signature>%s</Signature>\n\
      <ReferenceID>%s</ReferenceID>\n\
  </Envelop>\n\
  <Payload>%s</Payload>\n\
</BMD>",\
  msg->envelop.message_id,msg->envelop.message_type,msg->envelop.sender,msg->envelop.destination,\
  msg->envelop.create_on,msg->envelop.signature,msg->envelop.reference_id,msg->payload);
  fclose(fp);
}

void *test_for_different_msg_type(void * ty){
  int type=*(int*)ty;
  char * bmd_type = BMD_TYPES[type];
  char* sender=NULL;
  char* destination=NULL;
  char* message_type=NULL;
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
        printf("Not expected field %d\n",i);
        break;
    }
    field=strtok(NULL," ");
  }

  // Use current time as seed for random generator
  srand(time(0));
  for(i=0;i<NUMBER_OF_BMD_GENERATED_PER_BMD_TYPE;i++){
    char * file_name=malloc(100*sizeof(char));
    sprintf(file_name,"BMD/BMD_TYPE_%d_NO_%d.xml",type,i);
    bmd * msg=malloc(sizeof(bmd));
    if(type==0){
      msg->envelop.message_id="A9ECAEF2-107A-4452-9553-043B6D25386E";
      msg->envelop.message_type=message_type;
      msg->envelop.sender=sender;
      msg->envelop.destination=destination;
      msg->envelop.create_on="2020-08-12T05:18:00+0000";
      msg->envelop.signature="63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c";
      msg->envelop.reference_id="INV-PROFILE-889712";
      msg->payload=name[rand()%(sizeof(name)/sizeof(name[0]))];
    }
    else if(type=1){
      msg->envelop.message_id="A9ECAEF2-107A-4452-9553-043B6D25386E";
      msg->envelop.message_type=message_type;
      msg->envelop.sender=sender;
      msg->envelop.destination=destination;
      msg->envelop.create_on="2020-08-12T05:18:00+0000";
      msg->envelop.signature="63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c";
      char temp[10];
      sprintf(temp,"%d",(rand()%10)+127);
      msg->envelop.reference_id=temp;
      msg->payload="";
    }
    else{
      printf("Not expected BMD TYPE %d\n",type);
      break;
    }
    build_bmd_file(file_name,msg);
    post_data(file_name);
    remove(file_name);
    free(file_name);
    free(msg);
  }
  free(ty);
}

int main() {
  //defining thread
	pthread_t threads[NUMBER_OF_MESG_TYPE];
  unsigned int count=0;
  for(int i=0;i<NUMBER_OF_MESG_TYPE;i++) {
    int * type=malloc(sizeof(int));
    *type=i;
    if(pthread_create(&threads[*type],NULL,test_for_different_msg_type,type)!=0){
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
