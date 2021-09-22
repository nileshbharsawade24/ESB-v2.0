/*
/ Author : Rohit Kumar Bhamu
/ Designation : Senior Technical Member
/ Employer : Broadridge

*/

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "xml_parser.h"


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


    //insert_data(bmd_msg); //sending data to insert this data into esb_request table

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return bmd_msg;
}
// int main(int argc, char const *argv[]) {
//   bmd * b=parse_xml("../../BMD.xml");
//   return 0;
// }
