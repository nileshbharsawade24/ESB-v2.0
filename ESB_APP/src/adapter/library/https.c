#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define URL_MAX 100

char* transform_for_nationality_predictor_880(const char* const base_url, const char* const key, const char* const value){
  char* full_url=malloc(URL_MAX*sizeof(char));
  sprintf(full_url,"%s/?%s=%s",base_url,key,value);
  return full_url;
}

void transport_for_nationality_predictor_880(const char* const url){
  CURL *curl;
  CURLcode response_code;
  //Global libcurl initialisation
  if(curl_global_init(CURL_GLOBAL_ALL)==0){
    //Start a libcurl easy session
    curl = curl_easy_init();
    if(curl) {
      //setting curl in curl object
      curl_easy_setopt(curl, CURLOPT_URL, url);
      printf("\nHTTPS GET Response ----------->\n");
      /* Perform the request, res will get the return code and error checking*/
      if((response_code=curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "ERROR : curl_easy_perform() failed: %s",curl_easy_strerror(response_code));
      }
      printf("\n\n");
      //easy session cleanup
      curl_easy_cleanup(curl);
    }
    else{
      fprintf(stderr,"ERROR : Curl easy initilization failed and you cannot use the other curl functions");
    }
    //cleanup
    curl_global_cleanup();
  }
  else{
    fprintf(stderr,"ERROR : libcurl Global initilisation failed and you cannot use the other curl functions");
  }
}

// int main(void)
// {
//   transport_for_nationality_predictor_880("https://api.nationalize.io/?name=deepak");
//   transport_for_nationality_predictor_880("https://api.wrong.io/?name=deepak");
//   printf("%s\n",transform_for_nationality_predictor_880("https://api.nationalize.io","name","deepak"));
//   return 0;
// }
