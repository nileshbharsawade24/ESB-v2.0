#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

char * parse_http_request(char const * input_fpath){
  char * output_fpath="BMD.xml";
  FILE * f_r=fopen(input_fpath,"r");
  FILE * f_w=fopen(output_fpath,"w");

  if (f_r == NULL)return "0";

  int last_idx=-1;
  int i=0;
  char c;
  while((c=fgetc(f_r))!=EOF){
    if(c=='>')last_idx=i;
    i++;
  }

  f_r=fopen(input_fpath,"r");
  i=0;

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

  fclose(f_r);
  fclose(f_w);
  return output_fpath;
}

int main(int argc, char const *argv[]) {
  /* code */
  printf("%s\n",parse_http_request("http_request.txt"));
  return 0;
}
