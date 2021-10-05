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

bool check_word_in_sentence(const char* const word, const char* const sentence) {
  // printf("%s<-->%s\n",word,sentence);
	int n = strlen(word);
	int m = strlen(sentence);
  if(m>100 || n>100){
    fprintf(stderr,"ERROR : more character than expected\n");
  }
  //m^2 method
  for(int i=0;i<m;i++){
    char temp[100];
    bzero(temp,100);
    int x=0;
    for(int j=i;j<m&&sentence[i]!=' '&&sentence[i]!='/';j++){
      temp[x++]=sentence[j];
      if(strcmp(temp,word)==0){
        return true;
      }
    }
  }
	return false;
}

bool is_request_http_post(char * buff){
  char* first_line=malloc(100*sizeof(char));
  int n=0;
  while(*buff!='\n'){
    first_line[n++]=*buff;
    buff++;
  }
  first_line[n]='\0';
  if (check_word_in_sentence("HTTP",first_line) && check_word_in_sentence("POST",first_line)) {
		free(first_line);
    return true;
  }
	free(first_line);
  return false;
}

char* get_field_from_request(char * buff, char * field){

  int n=strlen(field);

  while(*buff!='\0'){
    int i=0;
    while(i<n  && *buff!='\n' && *buff==field[i]){
      buff++;
      i++;
    }
    if(i==n && *buff==':'){
      char * ans=malloc(100*sizeof(char));
      buff++;
      i=0;
      while(*buff!='\n'){
        if(*buff!=' ')ans[i++]=*buff;
        buff++;
      }
      ans[i]='\0';
      return ans;
    }
    while(*buff!='\n')buff++;
    buff++;
  }
  return "empty";
}

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


// int main(int argc, char const *argv[]) {
//   char * filename="../../tmp/HTTP_REQUEST_1632730985.txt";
//   char buff[1024000];
//   bzero(buff,1024000);
//   char c;
//   FILE *  f_r=fopen(filename,"r");
//   int i=0;
//   while((c=fgetc(f_r))!=EOF){
//     buff[i++]=c;
//   }
//   buff[i]='\0';
//   // printf("%s\n",buff);
//   printf("%s\n",get_field_from_request(buff,"accessl_token"));
//   printf("%s\n",get_field_from_request(buff,"access_token"));
//   printf("%s\n",get_field_from_request(buff,"access_tokenklklm"));
//   printf("%d\n",is_request_http_post(buff));
//
//   return 0;
// }
