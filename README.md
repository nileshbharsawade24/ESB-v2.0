# Camel
ESB Application of Camel group

# Installation required -
    sudo apt update
    sudo apt install mysql-server
    sudo apt install libmysqlclient-dev
    sudo apt install libxml2-dev
    sudo apt install libcurl4-openssl-dev

<!-- Also make sure you have mysql installed in your system. <br /> -->

# Layout overview -
.</br>
├── ESB_APP</br>
│   ├── src</br>
│   │   ├── adapter</br>
│   │   ├── database_handler</br>
│   │   ├── esb_request_handler</br>
│   │   └── tmp</br>
│   │   ├── makefile</br>
│   └── test</br>

# How to run -
First go to ESB_APP/src.</br>
Then setup your database and tables with appropriate schema and index and also fill manual data with following commands (or use mysql dump):

    make clean
    make setup
    ./setup

Then run the esb application with following commands :

    make clean
    make all
    ./run

# References -
[The libcurl C API](https://curl.se/libcurl/c/)

# Contributors -
[Deepak kumar](https://github.com/deepakjnv880)  
[Rohit bhamu](https://github.com/rohitbhamu)  

# Thanks for visiting :slightly_smiling_face: .
