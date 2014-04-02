/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#define LENGTH 1024
#define _ERVER_USING_THREADS_
void error(char *msg)
{
    perror(msg);
    exit(1);
}

typedef struct str_thdata
{
    int connection;
    char name[256];
} thrdata;



int create_connection(int portno)
{
    int sockfd;
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
       error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");
    listen(sockfd,5);
    return sockfd;
}

int get_client_connect(int socket_d)
{
    struct sockaddr_in client;
    int client_len, cli_d;
    client_len = sizeof(client);
    cli_d = accept(socket_d, (struct sockaddr *) &client, &client_len);
    if(cli_d < 0) error("error on client connecting");
    return cli_d;
}

void write_to_socket(void *ptr){
   FILE* f;
   char data[LENGTH];
   unsigned long size=0,fileSize;
   thrdata *arg;
   arg = (thrdata *) ptr;
   printf("gotovims9 otpravit %s\nconnection - %d\n",arg->name,arg->connection);
   f = fopen(arg->name, "r");
   if(f == NULL) error("failik is not open");
   fseek (f, 0 , SEEK_END);
   fileSize = ftell (f);
   rewind (f);
   snprintf(data, LENGTH, "%ld", fileSize);
   send(arg->connection, data, 1024, 0);
   while(size < fileSize){
       int read = 0,sent = 0;
       read = fread(data,1,LENGTH,f);
       sent = send(arg->connection, data, read, 0 );
       size+= sent;
   }

   printf("size of %ld",size);
   close(arg->connection);
}


int main(int argc, char *argv[])
{
    int my_socket, n, connection;
    char buffer[256];
    my_socket = create_connection(atoi(argv[1]));
    bzero(buffer,256);
    while(1){
        pthread_t pthr;
        thrdata data;
        bzero(buffer,256);
        connection = get_client_connect(my_socket);

        recv(connection, buffer, 256, 0);
        printf("Here is the message: %s\n",buffer);
        data.connection = connection;
        strcpy(data.name,buffer);
#ifdef _SERVER_USING_THREADS_
        pthread_create(&pthr, NULL, (void *) &write_to_socket,(void *)&data);
#else
        pid_t childPid = fork();
        if (childPid == 0){
            write_to_socket((void*)&data);
            exit(0);
        }
#endif
    }

    return 0;
}
