//Structure for task
typedef struct {
    unsigned int id;
    char* fpath;
    unsigned int processing_attempts;
}task;

void esb_request_poller();
