/**
 * When the function to be invoked is determined via a table lookup,
 * following is one of the ways to invoke a dyamically loaded function in C.
 * Help taken from: https://stackoverflow.com/a/1120834
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "https.h"
#include "email.h"
#include "ftp.h"
#include "function_lookup_and_invocation.h"
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
    void (*func)(bmd*);
} function_map;


const function_map load_corrosponding_transformation_function[] = {
    {"nationality_predictor_880", transform_for_nationality_predictor_880},//,
    {"email_service_transform", transform_for_email_service_123},
    {"sftp_application_880", transform_for_ftp_serice_880},
    // {"Credit service tranform", tranform_bmd_Credit_svc},
    // {"Credit service transport", transport_bmd_Credit_svc}
};

const function_map load_corrosponding_transportation_function[] = {
    {"nationality_predictor_880", transport_for_nationality_predictor_880},
    {"email_service_transform",transport_for_email_service_123},
    {"sftp_application_880", transport_for_ftp_serice_880}
    // {"Credit service tranform", tranform_bmd_Credit_svc},
    // {"Credit service transport", transport_bmd_Credit_svc}
};

/* This is how the ESB may dynamically invoke the service adapter functions */
void call_function(const char * mode,const char *name, void* data)
{
  if(strcmp("transform",mode)==0){
    for (int i = 0; i < (sizeof(load_corrosponding_transformation_function) / sizeof(load_corrosponding_transformation_function[0])); i++)
    {
        if (!strcmp(load_corrosponding_transformation_function[i].name, name) && load_corrosponding_transformation_function[i].func)
        {
            load_corrosponding_transformation_function[i].func(data);
            return;
        }
    }
  }
  else if(strcmp("transport",mode)==0){
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

// int main()
// {
//     // int d = 998;
//     // call_function("PO svc tranform", &d);
//     // call_function("PO svc transport", &d);
//     // call_function("Credit service tranform", &d);
//     // char * url="https://api.nationalize.io/?name=Mahesh";
//     bmd * bmd_msg= malloc (sizeof (bmd)); //intializing bmd_msg object
//
//
//     //storing value in bmd_bsg object
//     bmd_msg->MessageID= "sd";
//     bmd_msg->MessageType = "sd";
//     bmd_msg->Sender= "sd";
//     bmd_msg->Destination= "sd";
//     bmd_msg->CreationDateTime= "sd";
//     bmd_msg->Signature= "sd";
//     bmd_msg->ReferenceID= "sd";
//     bmd_msg->Payload= "sd";
//     call_function("transport","nationality_predictor_880",bmd_msg);
//
// }
