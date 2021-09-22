<<<<<<< HEAD
=======
/*
/ Author : Rohit Kumar Bhamu
/ Designation : Senior Technical Member
/ Employer : Broadridge
*/


//int process_esb_request(char* bmd_file_path);

/*
typedef struct {
    char* sender_id;
    char* destination_id;
    char* message_type;
    // TODO: Other fields
} bmd_envelop;

typedef struct {
    bmd_envelop envelop;
    char* payload;
} bmd;
*/

>>>>>>> b3e04b6f6b9b073615dd607e6acd78313c3032d1
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
