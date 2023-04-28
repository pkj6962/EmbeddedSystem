CC = gcc 
OBJS = io.o merge.o main.o ipc.o util.o
SRCS = io.c merge.c main.c ipc.c util.c
TARGET = kvstore_exe

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) 

io.o: io.c io.h
	$(CC) -c -o io.o io.c

merge.o: merge.c merge.h
	$(CC) -c -o merge.o merge.c 

main.o: main.c main.h
	$(CC) -c -o main.o main.c

ipc.o: ipc.c ipc.h
	$(CC) -c -o ipc.o ipc.c

util.o: util.c util.h
	$(CC) -c -o util.o util.c


clean: 
	rm *.o kvstore_exe




