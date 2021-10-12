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

# Project Layout
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
│   │   │   ├── email.c  
│   │   │   ├── email.h  
│   │   │   ├── function_lookup_and_invocation.c  
│   │   │   ├── function_lookup_and_invocation.h  
│   │   │   ├── https.c  
│   │   │   ├── https.h  
│   │   │   └── mail.txt  
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
└── README.md  
```

# How to run
First go to ESB_APP/src.</br>
If not, then setup your database and tables with appropriate schema and index and also fill manual data with following commands (or use mysql dump):

    make clean
    make setup
    ./setup

    =====================OR====================

    can setup with CAMEL_DB.dump (database file)

Now make sure that no process is running on port number 8888.</br>
Then run the esb application server with following commands :

    make clean
    make all
    ./server

Then open another terminal and go to Client/ directory and test the ESB application by running the simultaneous clients of different BMD types with following commands :

    gcc -o client test.c -lpthread -lcurl
    ./client

# References
[The libcurl C API](https://curl.se/libcurl/c/)  
[The MySQL C API](https://dev.mysql.com/doc/c-api/8.0/en/c-api-function-reference.html)

# Contributors
[Deepak kumar](https://github.com/deepakjnv880)  
[Rohit bhamu](https://github.com/rohitbhamu)  


# Happy coding :slightly_smiling_face: .
