/*
Author : Deepak kumar
Designation : Senior Member Technical
Employer : Broadridge
Description : This program will poll the database and assign task to different threads
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mysql.h>
#include "../database_handler/database_access.h"
#include "../adapter/function_lookup_and_invocation.h"
#include "../esb_request_handler/parser/xml_parser.h"
#include <stdlib.h>
// #include "worker.h"

#define NUM_WORKERS 5
#define threshold_for_processing_attempts 5

void cleanup(bmd* req,task* mytask){
  remove(mytask->fpath);
  free(req);
  free(mytask);
  pthread_exit(NULL);
}

void *work(void * t){
  task * mytask = (task*)t;
  bmd * req=parse_xml(mytask->fpath);
  MYSQL* connection=give_me_mysql_connection();
  char id[10];
  sprintf(id,"%d",mytask->id);
  if(!authenticate_and_validate_BMD(req)){
    update_single_field(connection, "esb_request","status","failed","id",id);
    update_single_field(connection, "esb_request","status_details","BMD authentication or validation Failed.","id", id);
    cleanup(req,mytask);
  }
  call_function("transform",req->Destination, req);
  call_function("transport",req->Destination, req);
  update_single_field(connection, "esb_request","status","done","id",id);
  cleanup(req,mytask);
}

void esb_request_poller(){
  //defining thread
	pthread_t threads[NUM_WORKERS];
  unsigned int count=0;
  MYSQL* connection=give_me_mysql_connection();
  while (1) {
    task * polled_task=poll_database_for_task(connection);
    if(polled_task!=NULL){
      char val[10],id[10];
      sprintf(val,"%d",polled_task->processing_attempts+1);
      sprintf(id,"%d",polled_task->id);
      if (polled_task->processing_attempts<threshold_for_processing_attempts) {
        update_single_field(connection, "esb_request","status","taken","id", id);
        update_single_field(connection, "esb_request","processing_attempts",val,"id", id);
        //handover task to thread
   		  if(pthread_create(&threads[count%NUM_WORKERS],NULL,&work,(void*)polled_task)!=0){
   			  printf ("ERROR: child thread not created\n");
   			  exit(-1);
   		  }
        else{
          count++;
        }
      }
      else{
        update_single_field(connection, "esb_request","status","failed","id", id);
        update_single_field(connection, "esb_request","status_details","Max Processing attempts threshold reached.","id", id);
        //do cleanup
        remove(polled_task->fpath);
        free(polled_task);
      }
    }
    sleep(5);
  }
  // waiting to terminate all NUM_THREADS thread
  for(int i=0;i<NUM_WORKERS;i++){
    pthread_join(threads[i], NULL);
  }
}

/* for testing purpose only */
// int main(int argc, char const *argv[]) {
//   esb_request_poller();
//   return 0;
// }
