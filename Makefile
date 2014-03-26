make:
	gcc client.c -o client
	gcc server.c -o server -lpthread
clean:
	rm client server
