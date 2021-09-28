#include <mysql.h>
#include "../worker/worker.h"
//this will handle error given mysql connection 'connection'
void handle_error(MYSQL *connection);
//this function will insert tuple in esb_request table given mysql connection 'conn' and 10 field
void insert_one_in_esb_request(const char* const f2,const char* const f3,const char* const f4,const char* const f5,const char* const f6,const char* const f7,const char* const f8,const char* const f9,const char* const f10,const char* const f11);
char * is_route_active(const char* const sender,const char* const destination,const char* const message_type);
bool is_route_present_in_transform_config(const char* const route_id);
bool is_route_present_in_transport_config(const char* const route_id);
MYSQL * give_me_mysql_connection();
task * poll_database_for_task(MYSQL *conn);
void update_single_field(MYSQL * conn, const char* const table_name, const char* const key_to_set, const char* const value_to_set, const char* const key_to_find, const char* const value_to_find);
char * get_route_id_form_unique_tuple(const char* const sender,const char* const destination,const char* const message_type);
char * select_single_field_on_one_condition(const char* const table_name,const char* const find,const char* const key,const char* const value);
char * select_single_field_on_two_condition(const char* const table_name,const char* const find,const char* const key1,const char* const value1,const char* const key2,const char* const value2);
