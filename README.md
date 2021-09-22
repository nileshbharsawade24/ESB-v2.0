# Camel
ESB Application of Camel group

# Installation required -
    sudo apt update
    sudo apt install mysql-server
    sudo apt install libmysqlclient-dev
    sudo apt install libxml2-dev

<!-- Also make sure you have mysql installed in your system. <br /> -->

# How to run -
If you have not, then first setup your database and tables with appropriate schema and index with following commands :
    
    make clean
    make setup
    ./setup

Then run the esb application with following commands :

    make clean
    make all
    ./run

# Contributors -
[Deepak kumar](https://github.com/deepakjnv880)  
[Rohit bhamu](https://github.com/rohitbhamu)  

