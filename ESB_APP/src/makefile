#\
Author : Deepak kumar\
Designation : Senior Member Technical\
Employer : Broadridge\
Description : This is makefile with two main target `all` and `setup` with `clean`.\
#\

all: server.o http_parser.o xml_parser.o database_access.o
	gcc -o run http_parser.o xml_parser.o server.o database_access.o -lxml2 `mysql_config --libs`
	rm *.o
	mkdir -p tmp
server.o: esb_request_handler/server.c
	gcc -c esb_request_handler/server.c -I/usr/include/mysql/
http_parser.o: esb_request_handler/parser/http_parser.c
	gcc -c esb_request_handler/parser/http_parser.c
xml_parser.o: esb_request_handler/parser/xml_parser.c
	gcc -c -I/usr/include/libxml2/ esb_request_handler/parser/xml_parser.c
database_access.o: database_handler/database_access.c
	gcc -c -I/usr/include/mysql/ database_handler/database_access.c

setup: database_handler/setup.c
	gcc -I/usr/include/mysql/ -o setup database_handler/setup.c `mysql_config --libs`

clean:
	rm setup run *.o
