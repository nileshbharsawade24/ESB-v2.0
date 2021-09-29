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
// #include "../worker/worker.h"
#define mysql_user_name "test_user"
#define mysql_user_password "test_password"
#define mysql_host "localhost"
#define mysql_db_name "CAMEL_DB"

// handle error based on mysql connection
void handle_error(MYSQL *connection){
      fprintf(stderr, "%s\n", mysql_error(connection));
      mysql_close(connection);
      exit(1);
}

task * poll_database_for_task(MYSQL *conn){
  if (mysql_query(conn, "select id,processing_attempts,data_location from esb_request where status='available' order by id asc limit 1")){
    handle_error(conn);
  }
  MYSQL_RES *result_rows = mysql_store_result(conn);
  MYSQL_ROW result_row=mysql_fetch_row(result_rows);
  task * t=NULL;
  if(result_row){
    t=malloc(sizeof(task));
    t->id=atoi(result_row[0]);
    t->fpath=result_row[2];
    t->processing_attempts=atoi(result_row[1]);
  }
  return t;
}

//check whether column is int or not
bool is_field_int(const char* const str){
      if(!strcmp(str,"id"))return true;
      if(!strcmp(str,"route_id"))return true;
      if(!strcmp(str,"is_active"))return true;
      if(!strcmp(str,"processing_attempts"))return true;
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

  //printf("\n--> QUERY : %s\n\n",buffer);
  if(mysql_stmt_execute(stmt)!=0){
    handle_error(conn);
  }
}

//select single field based on single condition given mysql connection and values
char * select_single_field_on_two_condition(const char* const table_name,const char* const find,const char* const key1,const char* const value1,const char* const key2,const char* const value2){
      MYSQL * conn = give_me_mysql_connection();

      MYSQL_STMT *stmt=mysql_stmt_init(conn);
      MYSQL_BIND bind[2];
      memset(bind,0,sizeof(bind));

      char buffer[1024];
      sprintf(buffer, "SELECT %s FROM %s WHERE %s=? AND %s=?", find, table_name, key1, key2);
      mysql_stmt_prepare(stmt, buffer, strlen(buffer));

      do_binding(&bind[0],key1,value1);
      do_binding(&bind[1],key2,value2);

      mysql_stmt_bind_param(stmt, bind);

      //printf("\n--> QUERY : %s\n\n",buffer);

      if(mysql_stmt_execute(stmt)!=0){
        handle_error(conn);
      }

      /* Fetch result set meta information */
      MYSQL_RES* prepare_meta_result = mysql_stmt_result_metadata(stmt);
      if (!prepare_meta_result)
      {
        fprintf(stderr,
               " mysql_stmt_result_metadata(), \
                 returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
      }

      if (mysql_num_fields(prepare_meta_result) != 1) /* validate column count */
      {
        printf("=====================>%d<===========\n",mysql_num_fields(prepare_meta_result));
        // fprintf(stderr, " invalid column count returned by MySQL\n");
        // exit(0);
        return NULL;
      }

      char * result=malloc(50*sizeof(char));
      /* Bind the result buffers for all 4 columns before fetching them */
      MYSQL_BIND rb;
      memset(&rb, 0, sizeof(rb));

      /* STRING COLUMN */
      long unsigned * temp=malloc(sizeof(int));
      *temp=50;
      rb.buffer_type= MYSQL_TYPE_STRING;
      rb.buffer= (char *)result;
      rb.buffer_length= *temp;
      rb.is_null= 0;
      rb.length= temp;

      /* Bind the result buffers */
      if (mysql_stmt_bind_result(stmt, &rb))
      {
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
      }

      /* Fetch all rows */
      unsigned int row_count= 0;
      // fprintf(stderr, "Fetching results ...\n");
      while (!mysql_stmt_fetch(stmt))
      {
        return result;
      }

      /* Free the prepared result metadata */
      mysql_free_result(prepare_meta_result);


      /* Close the statement */
      if (mysql_stmt_close(stmt))
      {
        /* mysql_stmt_close() invalidates stmt, so call          */
        /* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_error(conn));
        exit(0);
      }
      return result;
}

//select is_active based on three condition given mysql connection and all string values only
char * get_route_id_form_unique_tuple(const char* const sender,const char* const destination,const char* const message_type){
      MYSQL * conn = give_me_mysql_connection();
      char buffer[1024];
      char * query_str = "select route_id from routes where sender='%s' AND destination='%s' AND message_type='%s'";
      sprintf(buffer,query_str,sender,destination,message_type);
      //printf("\n--> QUERY : %s\n\n",buffer);
      if (mysql_query(conn, buffer)){
        handle_error(conn);
      }
      MYSQL_RES *result_rows = mysql_store_result(conn);
      MYSQL_ROW result_row=mysql_fetch_row(result_rows);
      // printf("%s %s\n",result_row[0],result_row[1]);
      if(result_row){
        return result_row[0];
      }
      return NULL;
}

//select is_active based on three condition given mysql connection and all string values only
char * is_route_active(const char* const sender,const char* const destination,const char* const message_type){
      MYSQL * conn = give_me_mysql_connection();
      char buffer[1024];
      char * query_str = "select route_id,is_active from routes where sender='%s' AND destination='%s' AND message_type='%s'";
      sprintf(buffer,query_str,sender,destination,message_type);
      //printf("\n--> QUERY : %s\n\n",buffer);
      if (mysql_query(conn, buffer)){
        handle_error(conn);
      }
      MYSQL_RES *result_rows = mysql_store_result(conn);
      MYSQL_ROW result_row=mysql_fetch_row(result_rows);
      // printf("%s %s\n",result_row[0],result_row[1]);
      if(result_row && strcmp(result_row[1],"1")==0){
        return result_row[0];
      }
      return NULL;
}

//select is_active based on three condition given mysql connection and all string values only
bool is_route_present_in_transport_config(const char* const route_id){
      MYSQL * conn = give_me_mysql_connection();
      char buffer[1024];
      char * query_str = "select id from transport_config where route_id=%s";
      sprintf(buffer,query_str,route_id);
      //printf("\n--> QUERY : %s\n\n",buffer);
      if (mysql_query(conn, buffer)){
        handle_error(conn);
      }
      MYSQL_RES *result_rows = mysql_store_result(conn);
      MYSQL_ROW result_row=mysql_fetch_row(result_rows);
      if(!result_row){
        return false;
      }
      return (result_row && strcmp(result_row[0],"1")==0)?true:false;
}

//select is_active based on three condition given mysql connection and all string values only
bool is_route_present_in_transform_config(const char* const route_id){
      MYSQL * conn = give_me_mysql_connection();
      char buffer[1024];
      char * query_str = "select id from transform_config where route_id=%s";
      sprintf(buffer,query_str,route_id);
      //printf("\n--> QUERY : %s\n\n",buffer);
      if (mysql_query(conn, buffer)){
        handle_error(conn);
      }
      MYSQL_RES *result_rows = mysql_store_result(conn);
      MYSQL_ROW result_row=mysql_fetch_row(result_rows);
      if(!result_row){
        return false;
      }
      return (result_row && strcmp(result_row[0],"1")==0)?true:false;
}

//insert ordered tuple in esb_request table given mysql connection and values
void insert_one_in_esb_request(const char* const f2,const char* const f3,const char* const f4,const char* const f5,const char* const f6,const char* const f7,const char* const f8,const char* const f9,const char* const f10,const char* const f11){
  MYSQL *conn = give_me_mysql_connection();
  char buffer[1024];
  char * query_str = "INSERT INTO esb_request"
                     "(sender_id,dest_id,message_type,reference_id,message_id,received_on,data_location,status,processing_attempts,status_details) "
                     "VALUES (\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%s,\"%s\",\"%s\",%s,\"%s\")";
  sprintf(buffer, query_str,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11);
  //printf("\n--> QUERY : %s\n\n",buffer);
  if (mysql_query(conn, buffer)){
    handle_error(conn);
  }
}

MYSQL * give_me_mysql_connection(){
  MYSQL *connection = mysql_init(NULL);

  if (connection == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(connection));
      exit(1);
  }

  if (mysql_real_connect(connection, mysql_host, mysql_user_name, mysql_user_password,
          mysql_db_name, 0, NULL, 0) == NULL)
  {
        handle_error(connection);
  }
  return connection;
}

// //insert ordered tuple in routes table given mysql connection and values
// void insert_one_in_routes(MYSQL * conn,const char* const f1,const char* const f2,const char* const f3,const char* const f4,const char* const f5){
//   char buffer[1024];
//   char * query_str = "INSERT INTO routes VALUES (%d,\"%s\",\"%s\",\"%s\",%d)";
//   sprintf(buffer, query_str,atoi(f1),f2,f3,f4,atoi(f5));
//   //printf("\n--> QUERY : %s\n\n",buffer);
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
//   //printf("\n--> QUERY : %s\n\n",buffer);
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
//   //printf("\n--> QUERY : %s\n\n",buffer);
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

  // insert_one_in_esb_request(con,"1","t","t","t","t","t"," 9999-12-31 23:59:59","t","t","t");
  // insert_one_in_routes(con,"1","t","t","t","10");
  // insert_one_in_transport_config(con,"1","1","t","t");
  // insert_one_in_transform_config(con,"1","1","t","t");
  //
  // insert_one_in_esb_request(con,"2","t","t","t","t","t"," 9999-12-31 23:59:59","t","t","t");
  // insert_one_in_routes(con,"2","t","t","t","10");
  // insert_one_in_transport_config(con,"2","2","t","t");
  // insert_one_in_transform_config(con,"2","2","t","t");
  //
  // insert_one_in_esb_request(con,"3","t","t","t","t","t"," 9999-12-31 23:59:59","t","t","t");
  // insert_one_in_routes(con,"3","t","t","t","10");
  // insert_one_in_transport_config(con,"3","3","t","t");
  // insert_one_in_transform_config(con,"3","3","t","t");
  //
  // printf("%s\n",select_single_field_on_one_condition(con,"routes","is_active","route_id","3"));
  //
  // char * table_name="routes";
  // char * key="is_active";
  // char * value="88";
  // char * find="route_id";
  // char * type="3";
  // update_single_field(con,table_name,key,value,find,type);
  //
  // printf("%s\n",select_single_field_on_one_condition(con,"routes","is_active","route_id","3"));
  // printf("%s\n",select_single_field_on_one_condition(con,"routes","is_active","route_id","35"));

  // printf("%s\n",is_route_active(con,"user_app_880","nationality_predictor_880","predict_nationality"));
  // char * route_id=is_route_active(con,"user_app_880","nationality_predictor_880","predict_nationality");
  // printf("%s\n",route_id);
  // printf("%d\n",is_route_present_in_transform_config(con,route_id));
  // printf("%d\n",is_route_present_in_transport_config(con,route_id));
  printf("-->%s<---\n",select_single_field_on_two_condition("transport_config","config_value","route_id","1","config_key","api"));
  mysql_close(con);
  exit(0);
}
*/
