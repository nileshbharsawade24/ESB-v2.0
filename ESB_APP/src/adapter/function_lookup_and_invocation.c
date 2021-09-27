/**
 * When the function to be invoked is determined via a table lookup,
 * following is one of the ways to invoke a dyamically loaded function in C.
 * Help taken from: https://stackoverflow.com/a/1120834
 */
#include <stdio.h>
#include <string.h>
#include "header/https.h"
/**
 * These should come from the header file of the respective service
 * provider's implementation of the transformation and transport.
 */
// void tranform_bmd_PO_svc(void* ptr) { printf("Transformation for PO service: %d\n", *((int*)ptr)); }
// void transport_bmd_PO_svc(void* ptr) { printf("Transport for PO service\n"); }
//
// void tranform_bmd_Credit_svc(void* ptr) { printf("Transformation for Credit service: %d\n", *((int*)ptr)); }
// void transport_bmd_Credit_svc(void* ptr) { printf("Transport for Credit service\n"); }

/* The ESB would have this table */
typedef struct
{
    const char *name;
    void (*func)(char*);
} function_map;

const function_map load_corrosponding_transportation_function[] = {
    {"HTTPS GET", https_get},
    // {"PO svc transport", transport_bmd_PO_svc},
    // {"Credit service tranform", tranform_bmd_Credit_svc},
    // {"Credit service transport", transport_bmd_Credit_svc}
};

/* This is how the ESB may dynamically invoke the service adapter functions */
void call_function(const char * mode,const char *name, void* data)
{
  if(strcmp("transport",mode)==0){
    for (int i = 0; i < (sizeof(load_corrosponding_transportation_function) / sizeof(load_corrosponding_transportation_function[0])); i++)
    {
        if (!strcmp(load_corrosponding_transportation_function[i].name, name) && load_corrosponding_transportation_function[i].func)
        {
            load_corrosponding_transportation_function[i].func(data);
            return;
        }
    }
  }
  else{
    fprintf(stderr, "\nERROR : There is no `%s` mode.\n",mode);
  }
}

int main()
{
    // int d = 998;
    // call_function("PO svc tranform", &d);
    // call_function("PO svc transport", &d);
    // call_function("Credit service tranform", &d);
    char * url="https://api.nationalize.io/?name=Mahesh";
    call_function("transport","HTTPS GET",url);

}
