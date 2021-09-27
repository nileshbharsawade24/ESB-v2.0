/*
/ Author : Rohit Kumar Bhamu
/ Designation : Senior Technical Member
/ Employer : Broadridge

*/

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdbool.h>
#include <string.h>
#include "xml_parser.h"


//token for authentication
char *token = "63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c";

//Structure for BMD Message data : Subject to change , As of now we are addding
//important fields of BMD file which we want to use
typedef struct {
    char *MessageID;
    char *MessageType;
    char *Sender;
    char *Destination;
    char *CreationDateTime;
    char *Signature;
    char *ReferenceID;
    char *Payload;


}msg_data;


//it will load xml document in xml document object

xmlDocPtr load_xml_doc(char *xml_file_path) {
    xmlDocPtr doc = xmlParseFile(xml_file_path);
    if (doc == NULL) {
        fprintf(stderr, "ERROR: Document not parsed successfully. \n");
        return NULL;
    }
    return doc;
}

/**
 * Extract the nodes matching the given xpath from the supplied
 * XML document object.
 */
xmlXPathObjectPtr get_nodes_at_xpath(xmlDocPtr doc, xmlChar *xpath) {

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        printf("ERROR: Failed to create xpath context from the XML document.\n");
        return NULL;
    }
    xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);  //save particular data in result object
    xmlXPathFreeContext(context);

    //there is no path
    if (result == NULL) {
        printf("ERROR: Failed to evaluate xpath expression.\n");
        return NULL;
    }

    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
        xmlXPathFreeObject(result);
        printf("No matching nodes found at the xpath.\n");
        return NULL;
    }
    return result;
}


/*
*validation : BMD should have some esssential fields
* As of now these fields are: Sender, destination and messagetype and messageId  (Will change if needed)
*/
bool is_bmd_valid (bmd* bmd_msg){

   if(bmd_msg->Sender == 0){

       printf("Validation fails: sender is missing\n");
       return false;

    }
   if(bmd_msg->Destination == 0){

       printf("Validation fails:destination is missing\n");
       return false;

    }
    if(bmd_msg->MessageID == 0){

        printf("Validation fails: messageId is missing\n");
        return false;

    }
   if(bmd_msg->MessageType == 0){

       printf("Validation fails: messageType is missing\n");
       return false;

    }

    // printf("Validation Successful\n");
    return true;

}

/**
 * Returns the text value of an XML element. It is expected that
 * there is only one XML element at the given xpath in the XML.
 */
xmlChar* get_element_text(char *node_xpath, xmlDocPtr doc) {
    xmlChar *node_text;
    xmlXPathObjectPtr result = get_nodes_at_xpath(doc,
        (xmlChar*)node_xpath);
    if (result) {
        xmlNodeSetPtr nodeset = result->nodesetval;
        if (nodeset->nodeNr == 1) {
            node_text = xmlNodeListGetString(doc,
                nodeset->nodeTab[0]->xmlChildrenNode, 1);
        } else {
            printf("ERROR: Expected one %s node, found %d\n", node_xpath, nodeset->nodeNr);
        }
        xmlXPathFreeObject(result);
    } else {
        printf("ERROR: Node not found at xpath %s\n", node_xpath);
    }
    return node_text;
}


bmd * parse_xml(char * filepath) {
    xmlDocPtr doc = load_xml_doc(filepath);
    /*
     used for testing purpose
    */
    // printf("MessageID=%s\n", get_element_text("//MessageID", doc));
    // printf("Sender=%s\n", get_element_text("//Sender", doc));
    // printf("Destination=%s\n", get_element_text("//Destination", doc));
    // printf("MessageType=%s\n", get_element_text("//MessageType", doc));
    // printf("Payload=%s\n", get_element_text("//Payload", doc));

    bmd * bmd_msg= malloc (sizeof (bmd)); //intializing bmd_msg object


    //storing value in bmd_bsg object
    bmd_msg->MessageID= get_element_text("//MessageID", doc);
    bmd_msg->MessageType = get_element_text("//MessageType", doc);
    bmd_msg->Sender= get_element_text("//Sender", doc);
    bmd_msg->Destination= get_element_text("//Destination", doc);
    bmd_msg->CreationDateTime= get_element_text("//CreationDateTime", doc);
    bmd_msg->Signature= get_element_text("//Signature", doc);
    bmd_msg->ReferenceID= get_element_text("//ReferenceID", doc);
    bmd_msg->Payload= get_element_text("//Payload", doc);



    /*
    * we will validate and authenticate  before inserting bmd into esb_able , if it fails we won't call insert into esb_request;
    */


    //Authentication  : As discussed in the common meeting, we're going to consider signature as a token for authentication
    //so we will compare sinagture field of BMD with our token.
     if(((strcmp(get_element_text("//Signature", doc),token))==0)){

   		// printf("Authentication is successful.\n");
    }

   	else{
   		printf("Authentication fails: Invalid BMD Request!!\n"); //if request is not valid we won't proceed with the flow
   		exit(1);
    	}



    //Validation
    if(is_bmd_valid(bmd_msg)){
        //if valid insert the data in esb_request otherwise
        //insert_data(bmd_msg); //sending data to insert this data into esb_request table

    }else{
        printf("Validation fails: Invalid BMD Request!!\n"); //if request is not valid we won't proceed with the flow
   		exit(1);
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return bmd_msg;
}

// int main(int argc, char const *argv[]) {
//   bmd * b=parse_xml("../../BMD.xml");
//   return 0;
// }
