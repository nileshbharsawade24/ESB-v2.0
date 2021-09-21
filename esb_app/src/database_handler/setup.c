//Author : Deepak kumar
//Designation : Senior Member Technical
//Employer : Broadridge

/*
This c program will setup the database and tables if they don't exist.
NOTE1 : To run this program make sure to your mysql user have all permission on atleast `CAMEL_DB` database.
NOTE2 : change mysql_user_name and my_sql_password appropritely or just create one same as these.
*/

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

#define mysql_user_name "test_user"
#define mysql_user_password "test_password"
#define mysql_host "localhost"


void handle_error(MYSQL *connection){
      fprintf(stderr, "%s\n", mysql_error(connection));
      mysql_close(connection);
      exit(1);
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, mysql_host, mysql_user_name, mysql_user_password, NULL, 0, NULL, 0) == NULL)
  {
      handle_error(con);
  }

  if (mysql_query(con, "create database IF NOT EXISTS CAMEL_DB"))
  {
        handle_error(con);
  }

  con=mysql_init(NULL);
  if (mysql_real_connect(con, mysql_host, mysql_user_name, mysql_user_password,
          "CAMEL_DB", 0, NULL, 0) == NULL)
  {
        handle_error(con);
  }

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS routes ("
  			"route_id INT,"
  			"sender VARCHAR(45),"
  			"destination VARCHAR(45),"
  			"message_type VARCHAR(45),"
  			"is_active TINYINT,"
        "PRIMARY KEY (route_id,sender,destination),"
        "UNIQUE (sender,destination,message_type)"
  			 ")")){
        handle_error(con);
  }

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS esb_request ("
  			"id INT PRIMARY KEY,"
  			"sender_id VARCHAR(45),"
  			"dest_id VARCHAR(45),"
  			"message_type VARCHAR(45),"
  			"reference_id VARCHAR(45),"
  			"message_id VARCHAR(45),"
  			"received_on DATETIME,"
  			"data_location TEXT,"
  			"status VARCHAR(20),"
  			"status_details TEXT"
  			 ")")){
        handle_error(con);
  }

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS transform_config ("
  			"id INT PRIMARY KEY,"
  			"route_id INT,"
  			"config_key VARCHAR(45),"
  			"config_value TEXT,"
        "UNIQUE (route_id,config_key)"
  			 ")")){
        handle_error(con);
  }

  if (mysql_query(con, "CREATE INDEX route_idx"
                       "ON transform_config (route_id)"){
        handle_error(con);
  }

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS transport_config ("
  			"id INT PRIMARY KEY,"
  			"route_id INT,"
  			"config_key VARCHAR(45),"
  			"config_value TEXT,"
        "UNIQUE (route_id,config_key)"
  			 ")")){
        handle_error(con);
  }

  if (mysql_query(con, "CREATE INDEX route_idx"
                       "ON transport_config (route_id)"){
        handle_error(con);
  }

  mysql_close(con);
  exit(0);
}
