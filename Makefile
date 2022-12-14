CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -lcurl -lpthread

Project: MenuMain.o ReadLoadData.o Stocks.o bootstrap.o Group.o Matrix.o
	$(CC) $(CFLAGS) -o Project MenuMain.o ReadLoadData.o Stocks.o Group.o bootstrap.o Matrix.o

MenuMain.o: Stocks.h ReadLoadData.h bootstrap.h Matrix.h Group.h MenuMain.cpp
	$(CC) $(CFLAGS) -c MenuMain.cpp
	
bootstrap.o: Stocks.h bootstrap.h Matrix.h Group.h
	$(CC) $(CFLAGS) -c bootstrap.cpp

Group.o: Stocks.h Group.h Group.cpp
	$(CC) $(CFLAGS) -c Group.cpp
	
ReadLoadData.o: Stocks.h ReadLoadData.h ReadLoadData.cpp 
	$(CC) $(CFLAGS) -c ReadLoadData.cpp
	
Stocks.o: Stocks.h Stocks.cpp
	$(CC) $(CFLAGS) -c Stocks.cpp
	
Matrix.o: Matrix.h Matrix.cpp
	$(CC) $(CFLAGS) -c Matrix.cpp
	
clean:
	rm -rf  Project *.o
