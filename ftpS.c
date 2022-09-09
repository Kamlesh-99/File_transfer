/*
Group 48
Sayantan Saha(19CS30041)
Kamalesh Garnayak(19CS10074)
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#define MAXCHAR 200
#define MAXLINE 200
#define MAXTOKENS 20
#define Port 20000
#define max 200
char string_success[10] = "200";
char string_failure[10] = "500";
char string_failure2[10] = "600";
char string_failure3[10] = "700";


//this function will parse commands wrt one or more space
void tokenise(char command[], char tokens[MAXTOKENS][MAXCHAR], int *n)
{
    (*n) = 0;
    char *pch;
    pch = strtok(command, " ");
    while (pch != NULL)
    {
        strcpy(tokens[(*n)++], pch);
        pch = strtok(NULL, " ");
    }
    return;
}
char final_password[200];
char password[200];

  
  



int main(){
    int sockfd, newsockfd;
    int clilen;
    
    int nwords=0, nsentences=0, nchars=0, delim=1;
    struct sockaddr_in cli_addr, serv_addr;
    int i;
   
    
    char buf[100];
    for(i=0; i < MAXCHAR; i++) buf[i] = '\0';

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Cannot create socket\n");
		exit(0);
	}

    serv_addr.sin_family	= AF_INET;
    serv_addr.sin_addr.s_addr	= INADDR_ANY;
    serv_addr.sin_port		= htons(Port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		perror("Unable to bind local address\n");
		exit(0);
	}

    listen(sockfd, 5);
    
    //int c=1;
    int r;
    while(1){
        //int c=1;
        clilen = sizeof(cli_addr);
       
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen) ;
        //printf("Connected with Client\n");
        if (newsockfd < 0) {
            perror("Accept error\n");
            exit(0);
        }
        printf("Connected with Client\n");
        //fork will make the server concurrent
        if(fork()==0){
        
        int c=1;
        int j;
        //From here it will start receiving commands from a specific client
        while((j=read(newsockfd,buf,200))>0){
        
            //for(i=0; i < MAXCHAR; i++) buf[i] = '\0';
            
            
            char tokens[MAXTOKENS][MAXCHAR];
            int m;
            char tempbuf[200];
            strcpy(tempbuf,buf);
            //printf("%d",c);
            //printf("Command\n");
            //printf("%s\n",buf);
            
            
            tokenise(buf, tokens, &m);
            //printf("%d",m);
            //printf("%s",tokens[0]);
            for(i=0; i < MAXCHAR; i++) buf[i] = '\0';
            //char pass[200];
            //for(i=0;i<200;i++)pass[i]='\0';
            
            /*if(m==1 && strcmp(tokens[0],"quit")==0){
            close(newsockfd);break;
            
            
            }*/
            
            
            if(c==1){
            //checking whether the first command is correct user command
                if(m==2 && strcmp(tokens[0],"user")==0){
                    char const* const fileName = "user.txt";
                  

                    FILE* file = fopen(fileName, "r"); 

                    if(!file){
                    printf("\n Unable to open : %s ", fileName);
                    exit(1);
                    }
                    int k = 1;
                    char line[500];
                    for(i=0;i<500;i++)line[i]='\0';
                    while (fgets(line, sizeof(line), file)) {
                        char tok[MAXTOKENS][MAXCHAR];
                        int n;
                        tokenise(line,tok,&n);
                        
                        if(!strcmp(tok[0],tokens[1])){
                            strcpy(password,tok[1]);
                            int length=strlen(password)-1;
                            
                            if(password[length]=='\n')
                            password[length]='\0';
                            //printf("match");
                            //printf("%s\n",password);
                            strcpy(final_password,password);
                            k=0;
                            send(newsockfd,string_success,strlen(string_success)+1,0);
                            c++;
                           break;
                        }
                    }
                    fclose(file);
                    if(k==1){
                        send(newsockfd,string_failure,strlen(string_failure)+1,0);
                        c=1;
                        //close(newsockfd);
                    }
                }
                else{
                    send(newsockfd,string_failure2,strlen(string_failure2)+1,0);
                    c=1;
                    //close(newsockfd);
                }
            }
            else if(c==2){
                //checking whether the second command is correct pass command
                if(m==2 && strcmp(tokens[0],"pass")==0){
                    if(!strcmp(password,tokens[1])){
                        send(newsockfd,string_success,strlen(string_success)+1,0);
                        c++;
                    }
                    else{
                        send(newsockfd,string_failure,strlen(string_failure)+1,0);
                        c=1;
                    }
                }
                else{
                    send(newsockfd,string_failure2,strlen(string_failure2)+1,0);
                    c=1;
                }
            }
            
            else{ 
                //cd command handling
                if(m==2 && strcmp(tokens[0],"cd")==0){
                    if(chdir(tokens[1])==0){
                        send(newsockfd,string_success,strlen(string_success)+1,0);
                        char path[200];

                    getcwd(path, 200);
                     printf("Current working directory: %s\n", path);
                    }
                    else{
                        send(newsockfd,string_failure,strlen(string_failure)+1,0);
                    }
                }
                //dir command handling
                if( strcmp(tokens[0],"dir")==0)
                {
                
                
                    // printf("Entered direct");
                     int j = 1;
                    DIR *d;
                    
                    struct dirent *dir;
                    d = opendir(".");
                    if (d)
                    {
                        while ((dir = readdir(d)) != NULL)
                        {
                            if(j>=3){
                                send(newsockfd, dir->d_name, strlen(dir->d_name)+1, 0);
                                //printf("%s",dir->d_name);
                            }
                            j++;
                        }
                        char str[10] = "";
                        send(newsockfd, str, strlen(str)+1, 0);
                        closedir(d);
                    }
            
                }
                //get command handling
                 if(m==3 && strcmp(tokens[0],"get")==0){
                   //printf("23\n");
                    char filename[100];
                    strcpy(filename,tokens[1]);
                    if( access( filename, F_OK ) == 0 ) {
                        // file exists
                        //printf("1\n");
                        send(newsockfd,string_success,strlen(string_success)+1,0);
                        unsigned char buffer[MAXLINE];
                        int fd = open(filename, O_RDONLY);
                        if (fd < 0) { perror("Error: Can not Open File"); exit(1); }
                        int bytesread=0;
                        //reading file by block by bock sending headers before sending the block
                        do{
                            for(i=0; i < MAXLINE; i++) buffer[i] = '\0';
                            bytesread = read(fd, buffer,MAXLINE-1);
                            buffer[bytesread]='\0';
                            if(bytesread==MAXLINE-1){
                                send(newsockfd,"M", 1, 0);
                                short b =  (short)(MAXLINE-1);
                                send(newsockfd, &b,sizeof(b),0);
                                send(newsockfd,buffer,strlen(buffer)+1,0);
                            }
                            else{
                                send(newsockfd,"L", 1, 0);
                                short b =  (short)bytesread;
                                //printf("b val");
                                //printf("%d\n",b);
                                send(newsockfd, &b,sizeof(b),0);
                                //printf("%s",buffer);
                                send(newsockfd,buffer,strlen(buffer)+1,0);
                            }
                        }while(bytesread==MAXLINE-1);
                    }
                    else{
                        send(newsockfd,string_failure,strlen(string_failure)+1,0);
                        //printf("2\n");
                    }
                }
                //put command handling
                else if(m==3 && strcmp(tokens[0],"put")==0){
                    int fd = open(tokens[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd<0){
                    
                         send(newsockfd,string_failure,strlen(string_failure)+1,0);
                  
                    }
                    else{
                        send(newsockfd,string_success,strlen(string_success)+1,0);
                        char buff[max];
                        
                       
                        
                        //printf("22\n");
                        //int ch = 0;
                        char bb[2];
                        //receiving content of file by block by clock with headers
                        while(1){
                            for(int i=0;i<2;i++)bb[i]='\0';
                            recv(newsockfd,bb,1,0);
                            bb[1]='\0';
                            if(strcmp(bb,"M")==0){
                                short b;
                                recv(newsockfd, &b, sizeof(b), 0);
                                int d = (int)b;
                                
                                char buff[max];
                                int r=recv(newsockfd,buff,d,0);
                                buff[r]='\0';
                                write(fd, buff, d);
                            }
                            else if(strcmp(bb,"L")==0){
                                short b;
                                recv(newsockfd, &b, sizeof(b), 0);
                                int d = (int)b;
                                //printf("b val");
                                //printf("%d\n",d); 
                                      
                                char buff[max];
                                int tot=0;

                                int r=read(newsockfd,buff,sizeof(buff));
                                buff[r]='\0';
                           
                                
                                write(fd, buff, d);
                                
                                break;
                            }
                        }
                    
                
                    close(fd);
                }
                }
                

               

   
            }
            
        }
        }

        
		close(newsockfd);
    }
    return 0;
}
