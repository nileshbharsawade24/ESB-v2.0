# Camel
ESB Application of Camel group

# Installation required
    sudo apt update
    sudo apt install mysql-server
    sudo apt install libmysqlclient-dev
    sudo apt install libxml2-dev
    sudo apt install libcurl4-openssl-dev

Also make sure in your mysql server, there is a user with  name *test_user*, password *test_password* and have all access grant on *CAMEL_DB* database. One can do this with following steps:<br />
1. Open mysql promt with sudo privilege
    ```
    sudo mysql
    ```
2. Create a user *test_user* with password *test_password*
    ```
    CREATE USER 'test_user' IDENTIFIED BY 'test_password';
    ```
3. Grant all access on *CAMEL_DB* database to user *test_user*
    ```
    GRANT ALL ON CAMEL_DB.* TO 'test_user';
    ```

# Layout overview
```
username@system_name:~/WFH/Camel$ tree
.  
├── Client  
│   ├── BMD  
│   └── test.c  
├── Daily_Work_Report.md  
├── ESB_APP  
│   ├── conf  
│   │   ├── build.conf  
│   │   └── esb_app.conf  
│   ├── src  
│   │   ├── adapter  
│   │   │   ├── function_lookup_and_invocation.c  
│   │   │   ├── function_lookup_and_invocation.h  
│   │   │   ├── header  
│   │   │   │   ├── email.h  
│   │   │   │   └── https.h  
│   │   │   └── library  
│   │   │       ├── email.c  
│   │   │       └── https.c  
│   │   ├── database_handler  
│   │   │   ├── database_access.c  
│   │   │   ├── database_access.h  
│   │   │   └── setup.c  
│   │   ├── esb_request_handler  
│   │   │   ├── esb.h  
│   │   │   ├── parser  
│   │   │   │   ├── http_parser.c  
│   │   │   │   ├── http_parser.h  
│   │   │   │   ├── xml_parser.c  
│   │   │   │   └── xml_parser.h  
│   │   │   ├── request_handler.c  
│   │   │   └── request_handler.h  
│   │   ├── makefile  
│   │   ├── server.c  
│   │   ├── tmp  
│   │   └── worker  
│   │       ├── worker.c  
│   │       └── worker.h  
│   └── test  
│       ├── munit.c  
│       └── munit.h  
├── Modules.pdf  
├── mysql_dump.sql  
├── README.md  
├── Single_flow.png  
└── Test Scenerio Example.pdf  
```

# How to run
First go to ESB_APP/src.</br>
If not, then setup your database and tables with appropriate schema and index and also fill manual data with following commands (or use mysql dump):

    make clean
    make setup
    ./setup

Then run the esb application with following commands :

    make clean
    make all
    ./server

# References
[The libcurl C API](https://curl.se/libcurl/c/)  
[The MySQL C API](https://dev.mysql.com/doc/c-api/8.0/en/c-api-function-reference.html)

# Contributors
[Deepak kumar](https://github.com/deepakjnv880)  
[Rohit bhamu](https://github.com/rohitbhamu)  

# Thanks for visiting :slightly_smiling_face: .
