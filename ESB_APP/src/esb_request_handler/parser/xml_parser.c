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
#include "../../database_handler/database_access.h"

//token for authentication
char *token = "63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c";

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
        // printf("No matching nodes found at the xpath.\n");
        return NULL;
    }
    return result;
}


/*
*validation : BMD should have some esssential fields
* As of now these fields are: Sender, destination and messagetype and messageId  (Will change if needed)

The validation will be done mainly for the envelope part of the BMD.

    1.The mandatory values (Sender, Destination, Message Type, etc.) must be present in the received BMD.
    2.For the received {Sender, Destination, Message Type}, there should be an active route record present in the routes table.
    3.For the selected route record, there should be corresponding records present in transport_config and transform_config tables.
    4.We will also check for some upper limit on the payload size. For example, the payload larger than 5MB may not be allowed, or are allowed only for certain senders and message types, etc.
*/
bool authenticate_and_validate_BMD (bmd* bmd_msg){

    //authentication
    if(bmd_msg->envelop.Signature == NULL){
       return false;
    }

    //validation : part1
    if(bmd_msg->envelop.Sender == NULL){
       return false;
    }
    if(bmd_msg->envelop.Destination == NULL){
       return false;
    }
    if(bmd_msg->envelop.MessageID == NULL){
        return false;
    }
    if(bmd_msg->envelop.MessageType == NULL){
       return false;
    }

    //validation : part2
    char * route_id=is_route_active(bmd_msg->envelop.Sender,bmd_msg->envelop.Destination,bmd_msg->envelop.MessageType);
    if(route_id==NULL){
      return false;
    }
    //validation : part3
    if(!is_route_present_in_transform_config(route_id)){
      return false;
    }
    if(!is_route_present_in_transport_config(route_id)){
      return false;
    }
    //validation : part3
    //check for payload size, if its greater than 5 MB then its not valid.
    float payload_size_in_MB=strlen(bmd_msg->payload)/1000000.0;
    // printf("----payload_size_in_MB------->%f\n",payload_size_in_MB);
    if(payload_size_in_MB>5){
      return false;
    }
   /*---- If all 4 points are validated , than return true-----*/

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
    }
    else {
        // printf("ERROR: Node not found at xpath %s\n", node_xpath);
        return NULL;
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
    bmd_msg->envelop.MessageID= get_element_text("//MessageID", doc);
    bmd_msg->envelop.MessageType = get_element_text("//MessageType", doc);
    bmd_msg->envelop.Sender= get_element_text("//Sender", doc);
    bmd_msg->envelop.Destination= get_element_text("//Destination", doc);
    bmd_msg->envelop.CreationDateTime= get_element_text("//CreationDateTime", doc);
    bmd_msg->envelop.Signature= get_element_text("//Signature", doc);
    bmd_msg->envelop.ReferenceID= get_element_text("//ReferenceID", doc);
    bmd_msg->payload= get_element_text("//Payload", doc);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return bmd_msg;
}

// int main(int argc, char const *argv[]) {
//   bmd * b=parse_xml("test.xml");
//   return 0;
// }
