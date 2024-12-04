build:
	gcc main.c -z noexecstack -o main

server:
	./main

client:
	nc 0.0.0.0 8000
