#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int get_connect(char * server_name, int portno){
    int sockfd;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(server_name);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    return sockfd;
}


int main(int argc, char *argv[])
{
    int sockfd, portno, n;


    char buffer[1000];
    printf("start\n");
    sockfd = get_connect("localhost",4441);
    printf("connected");
    puts(argv[1]);

    n = write(sockfd,argv[1],strlen(argv[1]));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,1000);
    n = read(sockfd,buffer,1000);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
