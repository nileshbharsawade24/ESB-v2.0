#include <mysql.h>
//this will handle error given mysql connection 'connection'
void handle_error(MYSQL *connection);
//this function will insert tuple in esb_request table given mysql connection 'conn' and 10 field
void insert_one_in_esb_request(const char* const f2,const char* const f3,const char* const f4,const char* const f5,const char* const f6,const char* const f7,const char* const f8,const char* const f9,const char* const f10);
char * is_route_active(MYSQL * conn,const char* const sender,const char* const destination,const char* const message_type);
bool is_route_present_in_transform_config(MYSQL * conn,const char* const route_id);
bool is_route_present_in_transport_config(MYSQL * conn,const char* const route_id);
MYSQL * give_me_mysql_connection();
