CC:= $(CROSS_COMPILE)gcc
make:
	$(CC) client.c -o client
	$(CC) server.c -o server -lpthread
clean:
	rm client server
