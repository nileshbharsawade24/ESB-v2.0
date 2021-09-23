/*
/ Author : Rohit Kumar Bhamu
/ Designation : Senior Technical Member
/ Employer : Broadridge
*/

//Structure for BMD Message data
typedef struct {
    const char *MessageID;
    const char *MessageType;
    const char *Sender;
    const char *Destination;
    const char *CreationDateTime;
    const char *Signature;
    const char *ReferenceID;
    const char *Payload;
}bmd;
