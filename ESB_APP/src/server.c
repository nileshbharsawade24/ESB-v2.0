#include <stdio.h>
#include <unistd.h>
#include "esb_request_handler/request_handler.h"
#include "worker/worker.h"

int main(int argc, char const *argv[]) {
  pid_t pid = fork();
  if (pid < 0){
    fprintf(stderr,"ERROR: failed to fork\n");
    return 1;
  }
  else if (pid == 0){
    handle_request();
  }
  else{
    esb_request_poller();
  }
  return 0;
}
