#include <stdio.h>
#include <unistd.h>
#include "esb_request_handler/request_handler.h"
#include "worker/worker.h"

int main(int argc, char const *argv[]) {
  //creating child process
  pid_t pid = fork();
  //fork will negative value if it failed
  if (pid < 0){
    fprintf(stderr,"ERROR: failed to fork\n");
    return 1;
  }
  //fork will zero to child process
  else if (pid == 0){
    handle_request();//child will handle incomming request
  }
  //fork will positive pid to parent process
  else{
    esb_request_poller();//parent will do polling and assigning task to threads
  }
  while(1);
  return 0;
}
