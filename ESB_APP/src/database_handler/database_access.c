/*
Author : Deepak kumar
Designation : Senior Member Technical
Employer : Broadridge
Description : This c program access the mysql database 'CAMEL_DB' and perform selection,insertion and updation
*/

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "database_access.h"

// handle error based on mysql connection
void handle_error(MYSQL *connection){
      fprintf(stderr, "%s\n", mysql_error(connection));
      mysql_close(connection);
      exit(1);
}

//check whether column is int or not
bool is_field_int(const char* const str){
      if(!strcmp(str,"id"))return true;
      if(!strcmp(str,"route_id"))return true;
      if(!strcmp(str,"is_active"))return true;
      return false;
}

//check string `str` is null or not
bool is_null(const char* const str){
      return !strcmp(str,"NULL");
}

//check string `str` is number or not
bool is_number(const char* const str){
  int length = strlen (str);
  for (int i=0;i<length; i++)if (!isdigit(str[i]))return false;
  return true;
}

//perform bindig on B based on key and its value given mysql connection
void do_binding(MYSQL_BIND * B,const char* const key,const char* const value){
 if(is_field_int(key)){
   if(!is_number(value)){
     fprintf(stderr, "ERROR : Key `%s` is a number while value `%s` is not a number.\n",key,value);
     exit(1);
   }
   long long int * temp=malloc(sizeof(int));
   *temp=atoi(value);
   B->buffer_type= MYSQL_TYPE_LONG;
   B->buffer= (char *)temp;
 }
 else{
   long unsigned * temp=malloc(sizeof(int));
   *temp=strlen(value);
   B->buffer_type= MYSQL_TYPE_STRING;
   B->buffer= (char *)value;
   B->buffer_length= *temp;
   B->is_null= 0;
   B->length= temp;
 }
}

//update single field based on single condition given mysql connection and values
void update_single_field(MYSQL * conn, const char* const table_name, const char* const key_to_set, const char* const value_to_set, const char* const key_to_find, const char* const value_to_find){
  MYSQL_STMT *stmt=mysql_stmt_init(conn);
  MYSQL_BIND bind[2];
  memset(bind,0,sizeof(bind));

  char* statement = "UPDATE %s SET %s=? WHERE %s=?";
  char buffer[1024];
  sprintf(buffer, statement, table_name,key_to_set,key_to_find);
  mysql_stmt_prepare(stmt, buffer, strlen(buffer));

  do_binding(&bind[0],key_to_set,value_to_set);
  do_binding(&bind[1],key_to_find,value_to_find);

  mysql_stmt_bind_param(stmt, bind);

  printf("\n--> QUERY : %s\n\n",buffer);
  if(mysql_stmt_execute(stmt)!=0){
    handle_error(conn);
  }
}

//select single field based on single condition given mysql connection and values
char * select_single_field(MYSQL * conn,const char* const table_name,const char* const find,const char* const key,const char* const value){
      char buffer[1024];
      if(is_field_int(key)){
            char * query_str = "SELECT %s from %s where %s=%s";
            sprintf(buffer, query_str,find,table_name,key,value);
      }
      else if(is_null(value)){
            char * query_str = "SELECT %s from %s where %s is %s";
            sprintf(buffer, query_str,find,table_name,key,value);
      }
      else{
            char * query_str = "SELECT %s from %s where %s=\"%s\"";
            sprintf(buffer, query_str,find,table_name,key,value);
      }
      printf("\n--> QUERY : %s\n\n",buffer);
      if (mysql_query(conn, buffer)){
        handle_error(conn);
      }
      MYSQL_RES *result_rows = mysql_store_result(conn);
      MYSQL_ROW result_row=mysql_fetch_row(result_rows);
      if(!result_row){
        return "Empty set";
      }
      return result_row[0]; //returning first row and first column value
}

//insert ordered tuple in esb_request table given mysql connection and values
void insert_one_in_esb_request(MYSQL * conn,const char* const f2,const char* const f3,const char* const f4,const char* const f5,const char* const f6,const char* const f7,const char* const f8,const char* const f9,const char* const f10){
  char buffer[1024];
  char * query_str = "INSERT INTO esb_request"
                     "(sender_id,dest_id,message_type,reference_id,message_id,received_on,data_location,status,status_details) "
                     "VALUES (\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%s,\"%s\",\"%s\",\"%s\")";
  sprintf(buffer, query_str,f2,f3,f4,f5,f6,f7,f8,f9,f10);
  printf("\n--> QUERY : %s\n\n",buffer);
  if (mysql_query(conn, buffer)){
    handle_error(conn);
  }
}

// //insert ordered tuple in routes table given mysql connection and values
// void insert_one_in_routes(MYSQL * conn,const char* const f1,const char* const f2,const char* const f3,const char* const f4,const char* const f5){
//   char buffer[1024];
//   char * query_str = "INSERT INTO routes VALUES (%d,\"%s\",\"%s\",\"%s\",%d)";
//   sprintf(buffer, query_str,atoi(f1),f2,f3,f4,atoi(f5));
//   printf("\n--> QUERY : %s\n\n",buffer);
//   if (mysql_query(conn, buffer)){
//     handle_error(conn);
//   }
// }
//
// //insert ordered tuple in transport_config table given mysql connection and values
// void insert_one_in_transport_config(MYSQL * conn,const char* const f1,const char* const f2,const char* const f3,const char* const f4){
//   char buffer[1024];
//   char * query_str = "INSERT INTO transport_config VALUES (%d,%d,\"%s\",\"%s\")";
//   sprintf(buffer, query_str,atoi(f1),atoi(f2),f3,f4);
//   printf("\n--> QUERY : %s\n\n",buffer);
//   if (mysql_query(conn, buffer)){
//     handle_error(conn);
//   }
// }
//
// //insert ordered tuple in transform_config table given mysql connection and values
// void insert_one_in_transform_config(MYSQL * conn,const char* const f1,const char* const f2,const char* const f3,const char* const f4){
//   char buffer[1024];
//   char * query_str = "INSERT INTO transform_config VALUES (%d,%d,\"%s\",\"%s\")";
//   sprintf(buffer, query_str,atoi(f1),atoi(f2),f3,f4);
//   printf("\n--> QUERY : %s\n\n",buffer);
//   if (mysql_query(conn, buffer)){
//     handle_error(conn);
//   }
// }

/*
int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, mysql_host, mysql_user_name, mysql_user_password,
          mysql_db_name, 0, NULL, 0) == NULL)
  {
        handle_error(con);
  }

  insert_one_in_esb_request(con,"1","t","t","t","t","t"," 9999-12-31 23:59:59","t","t","t");
  insert_one_in_routes(con,"1","t","t","t","10");
  insert_one_in_transport_config(con,"1","1","t","t");
  insert_one_in_transform_config(con,"1","1","t","t");

  insert_one_in_esb_request(con,"2","t","t","t","t","t"," 9999-12-31 23:59:59","t","t","t");
  insert_one_in_routes(con,"2","t","t","t","10");
  insert_one_in_transport_config(con,"2","2","t","t");
  insert_one_in_transform_config(con,"2","2","t","t");

  insert_one_in_esb_request(con,"3","t","t","t","t","t"," 9999-12-31 23:59:59","t","t","t");
  insert_one_in_routes(con,"3","t","t","t","10");
  insert_one_in_transport_config(con,"3","3","t","t");
  insert_one_in_transform_config(con,"3","3","t","t");

  printf("%s\n",select_single_field(con,"routes","is_active","route_id","3"));

  char * table_name="routes";
  char * key="is_active";
  char * value="88";
  char * find="route_id";
  char * type="3";
  update_single_field(con,table_name,key,value,find,type);

  printf("%s\n",select_single_field(con,"routes","is_active","route_id","3"));
  printf("%s\n",select_single_field(con,"routes","is_active","route_id","35"));

  mysql_close(con);
  exit(0);
}
*/
