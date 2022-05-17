assembler:main.o AssemblerManager.o fileHandler.o firstRead.o secondRead.o DataBuilder.o InsractionHandler.o symbolTabelHandler.o
	gcc -g -ansi -Wall -pedantic main.o AssemblerManager.o fileHandler.o firstRead.o secondRead.o symbolTabelHandler.o DataBuilder.o InsractionHandler.o -o assembler
main.o : main.c AssemblerManager.h 
	gcc -c -ansi -Wall -pedantic main.c -o main.o
AssemblerManager.o : AssemblerManager.c AssemblerManager.h globals.h fileHandler.o firstRead.o
	gcc -c -ansi -Wall -pedantic AssemblerManager.c -o AssemblerManager.o
fileHandler.o : fileHandler.c fileHandler.h
	gcc -c -ansi -Wall -pedantic fileHandler.c -o fileHandler.o
symbolTabelHandler.o:symbolTabelHandler.c symbolTabelHandler.h
	gcc -c -ansi -Wall -pedantic symbolTabelHandler.c -o symbolTabelHandler.o
firstRead.o: firstRead.c firstRead.h
	gcc -c -ansi -Wall -pedantic firstRead.c -o firstRead.o
secondRead.o: secondRead.c secondRead.h
	gcc -c -ansi -Wall -pedantic secondRead.c -o secondRead.o
DataBuilder.o: DataBuilder.c DataBuilder.h
	gcc -c -ansi -Wall -pedantic DataBuilder.c -o DataBuilder.o
InsractionHandler.o: InsractionHandler.c InsractionHandler.h
	gcc -c -ansi -Wall -pedantic InsractionHandler.c -o InsractionHandler.o