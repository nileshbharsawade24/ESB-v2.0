#\
Author : Deepak kumar\
Designation : Senior Member Technical\
Employer : Broadridge\
Description : This is makefile with two main target `all` and `setup` with `clean`.\
#\

headers=-I/usr/include/mysql/ -I/usr/include/libxml2/
libraries=-lxml2 -lcurl `mysql_config --libs`
OBJ= server.o request_handler.o worker.o https.o email.o ftp.o function_lookup_and_invocation.o http_parser.o database_access.o xml_parser.o

all:$(OBJ)
	gcc -o server $(OBJ) $(libraries)
	rm *.o
	mkdir -p tmp
server.o: server.c
	gcc -c server.c
worker.o: worker/worker.c
	gcc -c $(headers) worker/worker.c
request_handler.o: esb_request_handler/request_handler.c
	gcc -c $(headers) esb_request_handler/request_handler.c
https.o: adapter/https.c
	gcc -c $(headers) adapter/https.c
email.o: adapter/email.c
	gcc -c $(headers) adapter/email.c
ftp.o: adapter/ftp.c
	gcc -c $(headers) adapter/ftp.c
function_lookup_and_invocation.o: adapter/function_lookup_and_invocation.c
	gcc -c adapter/function_lookup_and_invocation.c
http_parser.o: esb_request_handler/parser/http_parser.c
	gcc -c esb_request_handler/parser/http_parser.c
xml_parser.o: esb_request_handler/parser/xml_parser.c
	gcc -c $(headers) esb_request_handler/parser/xml_parser.c
database_access.o: database_handler/database_access.c
	gcc -c $(headers) database_handler/database_access.c

setup: database_handler/setup.c
	gcc $(headers) -o setup database_handler/setup.c $(libraries)

clean:
	rm setup server *.o
