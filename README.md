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
.</br>
├── ESB_APP</br>
│   ├── src</br>
│   │   ├── adapter</br>
│   │   ├── database_handler</br>
│   │   ├── esb_request_handler</br>
│   │   ├── worker</br>
│   │   ├── tmp</br>
│   │   └── server.c</br>
│   │   └── makefile</br>
│   ├── test</br>

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
