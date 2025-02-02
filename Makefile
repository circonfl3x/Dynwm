flags=-lm
flagdev = $(flags) -g -fsanitize=address -Wall -Wpedantic
bin = Dynwall
CC = gcc
files = $(wildcard *.c)
default:
	$(CC) $(files) -o $(bin) $(flags)
dev:
	$(CC) $(files) -o $(bin) $(flagdev)

