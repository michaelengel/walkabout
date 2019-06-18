

#CC := ${CC} -DNODETAILS
CC = /proj/walkabout/tools/gcc-2.95.3/install/bin/gcc -Wall -g -DUNIX -I.  
#CC = gcc -Wall -g -DUNIX -I.
CXX = ${CC}

OBJS = instrm_scanner.o instrm_parser.o instrm_test.o 

instrm_test: ${OBJS}
	${CC} -o $@ ${OBJS} -lstdc++
	
instrm_test.o: instrm_parser.h instrm_test.cc
instrm_parser.o: instrm_parser.cc instrm_parser.h instrm_scanner.h 
instrm_scanner.o: instrm_scanner.h instrm_parser.h

instrm_parser.cc instrm_parser.h: instrmparser.y instrmscanner.l 
	bison++ -d -v -oinstrm_parser.cc -hinstrm_parser.h instrmparser.y

instrm_scanner.cc instrm_scanner.h: instrmscanner.l instrmparser.y
#	flex++ -d -hinstrm_scanner.h -oinstrm_scanner.cc instrmscanner.l
	flex++ -hinstrm_scanner.h -oinstrm_scanner.cc instrmscanner.l

