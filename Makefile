CC = gcc 
OBJS = io.o merge.o main.o ipc.o device.o util.o
SRCS = io.c merge.c main.c ipc.c device.c util.c
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

device.o: device.c device.h 
	$(CC) -c -o device.o device.c 

util.o: util.c util.h
	$(CC) -c -o util.o util.c


clean: 
	rm *.o kvstore_exe




