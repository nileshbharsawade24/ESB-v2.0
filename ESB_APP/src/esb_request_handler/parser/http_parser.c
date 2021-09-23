/*
Author : Deepak kumar
Designation : Senior Member Technical
Employer : Broadridge
Description : This c program will perform parsing on http request output BMD.xml
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "http_parser.h"

//parse http request present in filepath `input_fpath` and output BMD.xml
void parse_http_request(const char* const input_fpath,const char* const output_fpath){
  FILE * f_r=fopen(input_fpath,"r");
  FILE * f_w=fopen(output_fpath,"w");

  if (f_r == NULL)return;

  int last_idx=-1;
  int i=0;
  char c;
  //reading data from http request
  while((c=fgetc(f_r))!=EOF){
    if(c=='>')last_idx=i;
    i++;
  }

  f_r=fopen(input_fpath,"r");
  i=0;

    //writing data in BMD.xml file
    bool start=false;
    while((c=fgetc(f_r))!=EOF){
      if(start){
        fputc(c,f_w);
        if(i==last_idx)start=false;
      }
      else{
        if(c=='<'){
          fputc(c,f_w);
          start=true;
        }
      }
      i++;
    }

  //closing file pointers

  fclose(f_r);
  fclose(f_w);
}

/*
int main(int argc, char const *argv[]) {
  parse_http_request("../../http_request.txt");
  return 0;
}
*/
