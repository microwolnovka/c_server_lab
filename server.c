/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

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
   int err;
   char data[10000];
   thrdata *arg;
   arg = (thrdata *) ptr;
   printf("gotovims9 otpravit %s\nconnection - %d\n",arg->name,arg->connection);
   f = fopen(arg->name, "r");
   if(f == NULL) error("failik is not open");
   while(1){
       if(!fgets(data,sizeof(data),f))break;
       printf("read is good    %d\n",arg->connection);
       err = write(arg->connection, data,strlen(data));
       if(err < 0 ){
           error("error on the writing file");
       }
   }
   close(arg->connection);
}


int main(int argc, char *argv[])
{
    int my_socket, n, connection;
    char buffer[256];
    my_socket = create_connection(4441);
    bzero(buffer,256);
    while(1){
        pthread_t pthr;
        thrdata data;

        connection = get_client_connect(my_socket);
        n = read(connection,buffer,255);
        if(strlen(buffer) == 0) return 0;
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        data.connection = connection;
        strcpy(data.name,buffer);

        pthread_create(&pthr, NULL, (void *) &write_to_socket,(void *)&data);

    }

    return 0;
}
