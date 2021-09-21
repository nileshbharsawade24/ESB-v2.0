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
