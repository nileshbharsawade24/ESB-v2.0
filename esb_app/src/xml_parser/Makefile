xmltest: xmltest.o
	gcc xmltest.o -L/usr/lib64/libxml2 -lxml2 -lm -o xmltest
xmltest.o : parseXML.c
	gcc -c parseXML.c -I/usr/include/libxml2 -o xmltest.o

    