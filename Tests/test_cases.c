#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../ESB_APP/src/esb_request_handler/parser/xml_parser.h"
#include "../ESB_APP/src/database_handler/database_access.h"
#include "../ESB_APP/src/esb_request_handler/parser/http_parser.h"

bool test_authenticate_and_validate_BMD()
{
    //case 1:
    bmd *bmd1 = malloc(sizeof(bmd)); //null values invalid case
    if (authenticate_and_validate_BMD(bmd1) == false)
        return true;
    else
        return false;

    //case 2:
    bmd *bmd2 = malloc(sizeof(bmd)); //all values included, valid case
    bmd2->payload = "";
    bmd2->envelop.Sender = "";
    bmd2->envelop.Signature = "";
    bmd2->envelop.Destination = "";
    bmd2->envelop.MessageID = "";
    bmd2->envelop.MessageType = "";
    if (authenticate_and_validate_BMD(bmd2) == true)
        return true;
    else
        return false;

    //case 3:
    bmd *bmd3 = malloc(sizeof(bmd)); //partial values included, invalid case
    bmd2->payload = "";
    //bmd2->envelop.Sender="";      //sender is null here
    bmd2->envelop.Signature = "";
    bmd2->envelop.Destination = "";
    bmd2->envelop.MessageID = "";
    bmd2->envelop.MessageType = "";
    if (authenticate_and_validate_BMD(bmd3) == true)
        return true;
    else
        return false;
}

int test_check_word_in_sentence()
{
    //case 1: valid case
    char *word1 = "hello";
    char *sentence1 = "hello world";
    if (check_word_in_sentence(word1, sentence1) == 1)
        return 1;
    else
        return 0;

    //case 2: invalid case
    char *word2 = "hemlo";
    char *sentence2 = "hello world";
    if (check_word_in_sentence(word2, sentence2) == 0)
        return 1;
    else
        return 0;

    //case 3: invalid case
    char *word3 = "hello wordl123";
    char *sentence3 = "world";
    if (check_word_in_sentence(word3, sentence3) == 0)
        return 1;
    else
        return 0;

    //case 4: valid case
    char *word4 = "";
    char *sentence4 = "world";
    if (check_word_in_sentence(word4, sentence4) == 0)
        return 1;
    else
        return 0;
}

int test_is_request_http_post()
{
    //case 1: valid case
    char *buff1 = "this is supposed to be HTTP POST request";
    if (is_request_http_post(buff1) == 1)
        return 1;
    else
        return 0;

    //case 2: invalid case
    char *buff2 = "this is not supposed to be http POST request";
    if (is_request_http_post(buff2) == 0)
        return 1;
    else
        return 0;

    //case 3: invalid case
    char *buff3 = "this is supposed to be proper request";
    if (is_request_http_post(buff3) == 0)
        return 1;
    else
        return 0;

    //case 4: invalid case
    char *buff4 = "";
    if (is_request_http_post(buff4) == 0)
        return 1;
    else
        return 0;

    //case 5: invalid case
    char *buff5 = "this is only http request";
    if (is_request_http_post(buff5) == 0)
        return 1;
    else
        return 0;
}

int test_get_field_from_request()
{
    //case 1: Valid case
    char *buff1 = "key:value";
    char *field1 = "key";
    if (strcmp("value", get_field_from_request(buff1, field1)) == 0)
        return 1;
    else if (strcmp("empty", get_field_from_request(buff1, field1)) == 0)
        return 0;

    //case 2: Invalid case
    char *buff2 = "key:value";
    char *field2 = "kei";
    if (strcmp("empty", get_field_from_request(buff2, field2)) == 0)
        return 1;
    else if (strcmp("value", get_field_from_request(buff2, field2)) == 0)
        return 0;

    //case 3: Invalid case
    char *buff3 = "";
    char *field3 = "kei";
    if (strcmp("empty", get_field_from_request(buff2, field2)) == 0)
        return 1;
    else if (strcmp("empty", get_field_from_request(buff2, field2)) == 1)
        return 0;

    //case 4: Invalid case
    char *buff4 = "key::value";
    char *field4 = "key";
    if (strcmp("empty", get_field_from_request(buff4, field4)) == 0)
        return 1;
    else if (strcmp("empty", get_field_from_request(buff4, field4)) == 1)
        return 0;
}

int main()
{
    // if(test_authenticate_and_validate_BMD()==true) printf("inside ../ESB_APP/src/esb_request_handler/parser/xml_parser.c ==> authenticate_and_validate_BMD is [Tested ok]\n");
    if (test_check_word_in_sentence() == 1)
        printf("inside ESB_APP/src/esb_request_handler/parser/http_parser.c ==> check_word_in_sentence [Tested ok]\n");
    if (test_is_request_http_post() == 1)
        printf("inside ESB_APP/src/esb_request_handler/parser/http_parser.c ==> is_request_http_post [Tested ok]\n");
    if (test_get_field_from_request() == 1)
        printf("inside ESB_APP/src/esb_request_handler/parser/http_parser.c ==> get_field_from_request [Tested ok]\n");

    return 0;
}