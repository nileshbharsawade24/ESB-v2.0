//parse http request present in filepath `input_fpath` and output BMD.xml
void parse_http_request(const char* const input_fpath,const char* const output_fpath);
bool is_request_http_post(char * buff);
char* get_field_from_request(char * buff, char * field);
