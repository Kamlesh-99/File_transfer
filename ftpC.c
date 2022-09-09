/*
Group 48
Sayantan Saha(19CS30041)
Kamalesh Garnayak(19CS10074)
*/

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include<fcntl.h> 
#include<errno.h> 
extern int errno; 

#include <sys/types.h>
#define max 200
#define MAXLINE 200
char string_success[10] = "200";//sucess msg
char string_failure[10] = "500";//failure msg
char string_failure2[10] = "600";


void tokenize(char command[], char tokens[max][max], int *n)        //tokenize the commands
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

void concat(char s1[], char s2[])        //concatenate string 
  {
  
  
  int length, j;
  length = 0;
  
  while (s1[length] != '\0') {
    length++;
  }
  
  j=0;
  while(s2[j]!='\0')
  {
  s1[length]=s2[j];
  length++;
  j++;
  
  }


 
  
  }
  
  
int get(int sock,char tokens[max][max],int i)               // get command
{

int fd = open(tokens[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);  //open the file
                    int ans=1;
                    if(fd<0){
                    
                         printf("File can not be opened");
                         ans=0;
                  
                    }
                    else{
                    char temp[100];
                        for(int i=0;i<100;i++)
                        temp[i]='\0';
                        concat(temp,"get");
                        concat(temp," ");
                        concat(temp,tokens[i]);
                        concat(temp," ");
                        concat(temp,tokens[i]);
                        send(sock,temp,strlen(temp)+1,0);         // send get command in appropriate format
                        char buff[max];
                        int lk=read(sock, buff , sizeof(buff));   //read msg from serve
                        buff[lk]='\0';
                        if(strcmp(buff,"200")==0)
                        {
                     
                        //int ch = 0;
                        char bb[2];
                        while(1){
                            for(int i=0;i<2;i++)bb[i]='\0';
                            recv(sock,bb,1,0);
                            bb[1]='\0';
                            if(strcmp(bb,"M")==0)                  //received M 
                            {             
                                short b;
                                recv(sock, &b, sizeof(b), 0);
                                int d = (int)b;                   //received number of bytes
                                
                                char buff[max];
                                int r=recv(sock,buff,d,0);
                                buff[r]='\0';
                                write(fd, buff, d);
                            }
                            else if(strcmp(bb,"L")==0){          //received L
                                short b;
                                recv(sock, &b, sizeof(b), 0);
                                int d = (int)b;                   //receive number of bytes
                                
                           
                                      
                                char buff[max];
                                int tot=0;

                                int r=read(sock,buff,sizeof(buff));
                                buff[r]='\0';
                           
                                
                                write(fd, buff, d);               //write to file
                                
                                break;
                            }
                        }
                        printf("\nCode- 200\n");
                        printf("Command executed Sucessfully\n");
                    }
                    else
                    {remove(tokens[i]);
                    printf("Error code - 500\n");
                    printf("Server can not send file\n");
                    ans=0;
                    }
                    close(fd);
                }
                return ans;




}

  
  

int put(int sock,char tokens[max][max],int i)                 //put command
{                   
                     int ans=1;
                    char filename[100];
                    strcpy(filename,tokens[i]);
                  
                        // file exists
                        //printf("1\n");
                        
                        int fd = open(filename, O_RDONLY);            //open file to read
                        
                        char temp[100];
                        for(int i=0;i<100;i++)
                        temp[i]='\0';
                        concat(temp,"put");
                        concat(temp," ");
                        concat(temp,tokens[i]);
                        concat(temp," ");
                        concat(temp,tokens[i]);
                        
                         
                        if(fd>=0)
                        {
                        
                        send(sock,temp,strlen(temp)+1,0);               //send put command in appropriate format
                        
                        char buff[max];
                        int r=read(sock,buff,max);
                        buff[r]='\0';
                        if(strcmp(buff,"200")==0)                       
                        {
                        
                        unsigned char buffer[MAXLINE];
                        
                     
                        int bytesread=0;
                        do{
                            for(int i=0; i < MAXLINE; i++) buffer[i] = '\0';
                            bytesread = read(fd, buffer,MAXLINE-1);
                            buffer[bytesread]='\0';
                            if(bytesread==MAXLINE-1){
                                send(sock,"M", 1, 0);                       //send M 
                                short b =  (short)(MAXLINE-1);
                                send(sock, &b,sizeof(b),0);
                                send(sock,buffer,strlen(buffer)+1,0);
                            }
                            else{
                                send(sock,"L", 1, 0);                      //send L before last block
                                short b =  (short)bytesread;
                                
                                send(sock, &b,sizeof(b),0);
                              
                                send(sock,buffer,strlen(buffer)+1,0);
                            }
                        }while(bytesread==MAXLINE-1);
                        printf("\nCode- 200\n");
                        printf("Command executed Sucessfully\n");
                    }
                    else if(strcmp(buff,"500")==0){     
                    
                    printf("Error code - 500\n");                            
                        ans=0;
                        printf("Can't open file by server\n");
                    }
                    close(fd);
                    }
                    else
                    {ans=0;
                    printf("Error: Can not Open File");
                    
                    }
                
                

}





int main(int argc, char const *argv[])
{       
        

     
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		
		printf("\n Socket creation error \n");
		return -1;
	}
int state=0;

int loop=1;
int user_pass=0;
printf("The 'open' command should be like (open 127.0.0.1 20000) for our case");

while(loop)
{        


printf("\nmyFTP>");
//printf("\n");
char ss[max];
char s[max];
scanf("%[^\n]%*c", ss);

strcpy(s,ss);
char tokens[max][max];
int n;
tokenize(ss,tokens,&n);          //tokenize the user input
char comm[max];
strcpy(comm,tokens[0]);



if(n==3&&strcmp(comm,"open")==0)          //open command
{
serv_addr.sin_family = AF_INET;
        char port[max];
        strcpy(port,tokens[2]);             //port
        int PORT=atoi(port);
        char ip[max];
        strcpy(ip,tokens[1]);               //ip address
      
	serv_addr.sin_port = htons(PORT);          
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET,ip, &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
	}
	else
	{
	printf("\nConnection is done\n");
        state=1;
       }

}

else if(n==1&&strcmp(tokens[0],"quit")==0)               //quit command
{
loop=0;
if(state>0)
{
//send(sock,s,strlen(s)+1,0);
printf("Command executed Sucessfully\n");

close(sock);
return 0;
}



}


else if(state==1)                                 //to be executed only after open
{



if(n==2&&strcmp(comm,"user")==0)                    //user command
{

//printf("%s",s);
send(sock , s , strlen(s)+1 , 0 );
char buff[max];
int lk=read(sock, buff, sizeof(buff));
buff[lk]='\0';
//printf("%s",buff);
if(strcmp(buff,"500")==0)
{
printf("\nError code- 500");
printf("\nNo user present");
}
else if(strcmp(buff,"200")==0)
{printf("\nCode- 200\n");
printf("Command executed Sucessfully\n");
user_pass=1;
state=2;
}
else 
{
printf("\nError code- 600");
printf("\nEnter appropriate command-username");
}




}                                                      // no other command after open should be given
else 
{
printf("\nError code- 600");
printf("\nEnter appropriate command-username");
}


}




else if(state==2)                                         // to be executed after user command
{

if(n==2&&strcmp(comm,"pass")==0)                         //password
{
//printf("%s",s);
send(sock , s,strlen(s)+1 , 0 );
char buff[max];
int lk=read(sock, buff, sizeof(buff));
buff[lk]='\0';
//printf("%s",buff);
if(strcmp(buff,"500")==0)
{
printf("\nError code- 500");
printf("\nWrong password !!");
state=1;
}
else if(strcmp(buff,"200")==0)
{printf("\nCode- 200\n");
printf("Command executed Sucessfully\n");
user_pass=2;
state=3;
}
else if(strcmp(buff,"600")==0)
{state=1; 
printf("\nError code- 600");                                                          //go to user if wrong password
printf("\nEnter appropriate command :- username");

}

}
else                                                              
{state=1;
printf("\nError code- 600");   
printf("Enter appropriate command :- username");

}
}




else if(state==3)                                                      //to be executed after password matched
{

if(n==2&&strcmp(comm,"cd")==0)                                         //cd command
{

send(sock , s , strlen(s)+1 , 0 );
char buff[max];
int lk=read(sock, buff , sizeof(buff));
buff[lk]='\0';

if(strcmp(buff,"500")==0)
{
printf("\nError code- 500");
printf("\nError in changing directory");
}
else if(strcmp(buff,"200")==0)
{
printf("\nCode- 200\n");
printf("Command executed Sucessfully\n");
}

else if(strcmp(buff,"600")==0)
{
printf("\nError code- 600");
if(user_pass==0)
printf("\nEnter appropriate command :- username");
else if(user_pass==1)
printf("\nEnter appropriate command :- password");



}



}


else if(n==2&&strcmp(comm,"lcd")==0)                                            //lcd command
{
if(chdir(tokens[1])==0)printf("Command executed Sucessfully\n");
}


else if(strcmp(comm,"dir")==0)                                                    //dir command
{


//printf("%ld\n",strlen(s));
send(sock , s , strlen(s)+1 , 0 );

char buff[max];
int lk=read(sock, buff , sizeof(buff));
if(lk>1)buff[lk]='\0';
/* if(strcmp(buff,"600")==0)
{
if(user_pass==0)
printf("Enter appropriate command :- username");
else if(user_pass==1)
printf("Enter appropriate command :- password");



}
else
{*/
if(strcmp(buff,"")!=0){
while(strcmp(buff,"")!=0)
{
printf("%s\n",buff);
for(int k=0;k<max;k++)buff[k]='\0';
int lk=read(sock, buff , sizeof(buff));
if(lk>1)buff[lk]='\0';
//for(int k=0;k<max;k++)buff[k]='\0';
}
}
//}
}

else if(n==3 && strcmp(tokens[0],"put")==0)                                      
{
                   //printf("23\n");
                    char filename[100];
                    strcpy(filename,tokens[1]);
                    if( access( filename, F_OK ) == 0 ) {
                        // file exists
                        //printf("1\n");
                        int fd = open(filename, O_RDONLY);
                        if(fd>=0)
                        {
                        send(sock,s,strlen(s)+1,0);
                        char buff[max];
                        int r=read(sock,buff,max);
                        buff[r]='\0';
                        if(strcmp(buff,"200")==0)
                        {
                        unsigned char buffer[MAXLINE];
                        
                     
                        int bytesread=0;
                        do{
                            for(int i=0; i < MAXLINE; i++) buffer[i] = '\0';
                            bytesread = read(fd, buffer,MAXLINE-1);
                            buffer[bytesread]='\0';
                            if(bytesread==MAXLINE-1){
                                send(sock,"M", 1, 0);
                                short b =  (short)(MAXLINE-1);
                                send(sock, &b,sizeof(b),0);
                                send(sock,buffer,strlen(buffer)+1,0);
                            }
                            else{
                                send(sock,"L", 1, 0);
                                short b =  (short)bytesread;
                                //printf("b val");
                                //printf("%d\n",b);
                                send(sock, &b,sizeof(b),0);
                                //printf("%s",buffer);
                                send(sock,buffer,strlen(buffer)+1,0);
                            }
                        }while(bytesread==MAXLINE-1);
                        printf("\nCode- 200\n");
                        printf("Command executed Sucessfully\n");
                    }
                    else if(strcmp(buff,"500")==0){
                        printf("\nError code- 500");
                        printf("\nCan't open file by server\n");
                    }
                    }
                    else
                    { 
                    printf("Can't open file by client");
                    
                    
                    }
                
                }
                else
                    { 
                    printf("Can't open file by client");
                    
                    
                    }
                
                
                
                }
                
                
 else if(n==3 && strcmp(tokens[0],"get")==0){
                    int fd = open(tokens[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd<0){
                    
                         printf("\nFile can not be opened");
                  
                    }
                    else{
                        send(sock,s,strlen(s)+1,0);
                        char buff[max];
                        int lk=read(sock, buff , sizeof(buff));
                        buff[lk]='\0';
                        if(strcmp(buff,"200")==0)
                        {
                        //printf("22\n");
                        //int ch = 0;
                        char bb[2];
                        while(1){
                            for(int i=0;i<2;i++)bb[i]='\0';
                            recv(sock,bb,1,0);
                            bb[1]='\0';
                            if(strcmp(bb,"M")==0){
                                short b;
                                recv(sock, &b, sizeof(b), 0);
                                int d = (int)b;
                                
                                char buff[max];
                                int r=recv(sock,buff,d,0);
                                buff[r]='\0';
                                write(fd, buff, d);
                            }
                            else if(strcmp(bb,"L")==0){
                                short b;
                                recv(sock, &b, sizeof(b), 0);
                                int d = (int)b;
                                //printf("b val");
                                //printf("%d\n",d); 
                                      
                                char buff[max];
                                int tot=0;

                                int r=read(sock,buff,sizeof(buff));
                                buff[r]='\0';
                           
                                
                                write(fd, buff, d);
                                
                                break;
                            }
                        }
                        printf("\nCode- 200\n");
                        printf("Command executed Sucessfully\n");
                    }
                    else
                    {remove(tokens[2]);
                    printf("\nError code- 500");
                    printf("\nServer can not send file\n");
                    }
                    close(fd);
                }}
                
     
                
else if(strcmp(tokens[0],"mput")==0)         //mput command
{

int st=1;
int i=1;
while((i<n)&&st)                                      // get all the files in appropriate format
{
if(i!=n-1)
{
int length=strlen(tokens[i]);
tokens[i][length-1]='\0';

 
}
st=put(sock,tokens,i);


i++;
}
              
}
else if(strcmp(tokens[0],"mget")==0)           //mget command
{

int st=1;
int i=1;
while((i<n)&&st)                                    // get all the files in appropriate format
{
if(i!=n-1)
{
int length=strlen(tokens[i]);
tokens[i][length-1]='\0';

 
}
st=get(sock,tokens,i);


i++;
}              
}



else
{
if(state==1)
printf("\nEnter appropriate command -username");
else

printf("\nNot a valid command");


}






}


}

	

	
       
        

      

	  close(sock);
	
      
	return 0;
}
