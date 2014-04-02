#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define LENGTH 1024
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
    int sockfd, portno, n, fileSize, readSize;
    FILE *file =  fopen("recieved.file","w");
    char buffer[LENGTH];
    printf("start\n");
    portno = atoi(argv[2]);
    sockfd = get_connect(argv[1],portno);
    printf("connected %d",sizeof(argv[3]));
    puts(argv[3]);
    bzero(buffer,LENGTH);

    send(sockfd, argv[3], strlen(argv[3]), 0);
    recv(sockfd, buffer, LENGTH, 0);
    fileSize = atoi(buffer);
    readSize = 0;
    bzero(buffer,LENGTH);
    while (readSize< fileSize) {
        int r = recv(sockfd, buffer, LENGTH, 0);
        readSize += r;
        fwrite(buffer, 1, r, file);
    }
    printf("read size%d\n filesize %d\n",readSize,fileSize);
    return 0;
}
