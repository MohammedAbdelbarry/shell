OBJS = command_parser.o commands.o environment.o file_processing.o main.o variables.o strutil.o
CC = gcc
CFLAGS  = -g -Wall
TARGET = bsh

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully

main.o: main.c environment.h variables.h commands.h command_parser.h constants.h file_processing.h strutil.h
	$(CC) $(CFLAGS) -c main.c

command_parser.o: command_parser.c command_parser.h constants.h strutil.h
	$(CC) $(CFLAGS) -c command_parser.c

commands.o: commands.c commands.h variables.h file_processing.h constants.h strutil.h
	$(CC) $(CFLAGS) -c commands.c

file_processing.o: file_processing.c file_processing.h variables.h constants.h
	$(CC) $(CFLAGS) -c file_processing.c

environment.o: environment.c environment.h variables.h
	$(CC) $(CFLAGS) -c environment.c

strutil.o: strutil.c strutil.h variables.h constants.h
	$(CC) $(CFLAGS) -c strutil.c

variables.o: variables.c variables.h
	$(CC) $(CFLAGS) -c variables.c

clean:
	rm $(OBJS) $(TARGET)
