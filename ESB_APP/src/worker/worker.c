/*
Author : Deepak kumar
Designation : Senior Member Technical
Employer : Broadridge
Description : WORKER
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mysql.h>
#include "../database_handler/database_access.h"
#include "../esb_request_handler/parser/xml_parser.h"
#include <stdlib.h>
// #include "worker.h"

#define NUM_WORKERS 5
#define threshold_for_processing_attempts 5

void *work(void * t){
  task * mytask = (task*)t;
  printf("---->%s\n",mytask->fpath);
  //parse xml
  char * actual_file_path=malloc(30*sizeof(char));
  sprintf(actual_file_path,"../%s",mytask->fpath);
  bmd * req=parse_xml(actual_file_path);
  if(!authenticate_and_validate_BMD(req)){
    free(actual_file_path);
    free(mytask);
    pthread_exit(NULL);
  }
  free(actual_file_path);
  free(mytask);
}

void esb_request_poller(){
  //defining thread
	pthread_t threads[NUM_WORKERS];
  unsigned int count=0;
  MYSQL* connection=give_me_mysql_connection();
  // while (1) {
    printf("1==========\n");
    task * polled_task=poll_database_for_task(connection);
    if(polled_task!=NULL){
      printf("11==========\n");
      char val[10],id[10];
      sprintf(val,"%d",polled_task->processing_attempts+1);
      sprintf(id,"%d",polled_task->id);
      if (polled_task->processing_attempts<threshold_for_processing_attempts) {
        printf("111==========\n");
        update_single_field(connection, "esb_request","status","taken","id", id);
        update_single_field(connection, "esb_request","processing_attempts",val,"id", id);
      }
      else{
        printf("112==========\n");
        update_single_field(connection, "esb_request","status","failed","id", id);
        //do cleanup
      }
      //----create child thread----
 		 if(pthread_create(&threads[count%NUM_WORKERS],NULL,&work,(void*)polled_task)!=0){
 			 printf ("ERROR: child thread not created\n");
 			 exit(-1);
 		 }
      count++;
    }
    sleep(5);
  // }
}

int main(int argc, char const *argv[]) {
  esb_request_poller();
  return 0;
}