//Name of Author: Robert Hakimi and Cruz Baciewicz
//Course Number and Name: CSE434, Computer Networks
//Semester: Fall 2016
//Project Part: 2
//Time Spent: 15 hours


// A simple server in the internet domain using TCP and socket programming


//header files

//input - output declarations included in all C programs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
//contains definitions of a number of data types used in system calls
#include <sys/types.h>

//definitions of structures needed for sockets
#include <sys/socket.h>

//in.h contains constants and structures needed for internet domain addresses
#include <netinet/in.h>
struct node
{
    char info[256];
    struct node *ptr;
}*top,*top1,*temp;

char* topelement();
void push(char *string);
void pop(char string[]);
int empty();
void display();
void destroy();
void stack_count();
void create();
int count = 0;

/* Create empty stack */
void create()
{
    top = NULL;
}

/* Count stack elements */
void stack_count()
{
    //printf("\n No. of elements in stack : %d", count);
}

/* Push data into stack */

void push(char* string)
{
    if (top == NULL)
    {
        top =(struct node *)malloc(1*sizeof(struct node));
        top->ptr = NULL;
        strncpy(top->info,string,256);
    }
    else
    {
        temp =(struct node *)malloc(1*sizeof(struct node));
        temp->ptr = top;
        strncpy(temp->info, string, 256);
        top = temp;
    }
    count++;
}

/* Display stack elements */
void display()
{
    top1 = top;

    if (top1 == NULL)
    {
        return;
    }

    while (top1 != NULL)
    {
        top1 = top1->ptr;
    }
 }

/* Pop Operation on stack */

void pop(char string[])
{
    top1 = top;

    if (top1 == NULL)
    {
        return;
    }
    else
        top1 = top1->ptr;

    strcpy(string,top->info);
    free(top);
    top = top1;
    count--;
}

/* Return top element */

char* topelement()
{
    char* tempstring = top->info;
    return(tempstring);
}

/* Check if stack is empty or not */
int empty()
{
    if (top == NULL)
        return 1;
    
    else
        return 0;
    
}

/* Destroy entire stack */

void destroy()
{
    top1 = top;

    while (top1 != NULL)
    {
        top1 = top->ptr;
        free(top);
        top = top1;
        top1 = top1->ptr;
    }

    free(top1);
    top = NULL;

    
    count = 0;
}

//****************************END STACK STUFF***************************





//This function is called when a system call fails. It displays a message about the error on stderr and then aborts the program.

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

if(argc != 2)
{
  printf("Usage: ./server <server port>\n");
  return; 
}

//variable declarations

//fie type pointer
     FILE *fp;

//String uses
    char copy[256];
    char *filename, *operation, *extra;
    char string[256];
    char *fileArray[5];
    //initialize fileArray
    int g;
    for(g = 0; g<5; g++)
      fileArray[g] = "";
    
//delimeter specifier for tokenizer

    const char delim[2] = ", ";
     
//sockfd and newsockfd are file descriptors,These two variables store the values returned by the socket system call and the accept system call.
//portno stores the port number on which the server accepts connections.
     int sockfd, newsockfd, portno, pid, clientID;
     int i = 0;
     int breakflag = 0;
     int globID;
     int fileArrayCounter = 0;
     int incrementor = 0;
     bool duplicateFlag = false;
     int IDarray[100];
     for(i; i < 100; i++)
     {
      IDarray[i] = 0;
     }
//clilen stores the size of the address of the client. This is required for the accept system call.
     socklen_t clilen;

//server reads characters from the socket connection into this buffer.
     char buffer[256];

//sockaddr_in is a structure containing an internet address
/*
struct sockaddr_in
{
  short   sin_family; //must be AF_INET
  u_short sin_port;
  struct  in_addr sin_addr;
  char    sin_zero[8]; // Not used, must be zero
};
*/
//in_addr structure, contains only one field, a unsigned long called s_addr.

//serv_addr will contain the address of the server, and cli_addr will contain the address of the client which connects to the server.
     struct sockaddr_in serv_addr, cli_addr;
     int n;

//create socket
//it take three arguments - address domain, type of socket, protocol (zero allows the OS to choose thye appropriate protocols based on type of socket)
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

//set server address buffer with zeros using bzero or memset
//two arguments - pointer to buffer and sizeof buffer
     bzero((char *) &serv_addr, sizeof(serv_addr));

//atoi() function can be used to convert port number from a string of digits to an integer, if your input is in the form of a string.
     portno = atoi(argv[1]);

//contains a code for the address family
     serv_addr.sin_family = AF_INET;

//contains the IP address of the host
     serv_addr.sin_addr.s_addr = INADDR_ANY;

//contain the port number
     serv_addr.sin_port = htons(portno);

//bind() system call binds a socket to an address, in this case the address of the current host and port number on which the server will run.
//three arguments, the socket file descriptor, the address to which is bound, and the size of the address to which it is bound.
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");

//listen system call allows the process to listen on the socket for connections.
//The first argument is the socket file descriptor, and second is number of connections that can be waiting while the process is handling a particular connection.
     listen(sockfd,5);

     clilen = sizeof(cli_addr);

while(1)
{
     duplicateFlag = false;
//accept() system call causes the process to block until a client connects to the server.
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     bzero(buffer,256);
     read(newsockfd,buffer,255);
     clientID = atoi(buffer);
     globID = clientID;
     for(i = 0; i <100; i++)
     {
      if(clientID == IDarray[i])
       {
        write(newsockfd,"duplicate client number....rejected by server",45 );
        close(newsockfd);
        duplicateFlag = true;
       }
     }
     if(!duplicateFlag)
      {
       IDarray[incrementor] = clientID;
       incrementor++;
       write(newsockfd, "ok", 2);
      }
     if(!duplicateFlag) //if connection wasnt duplicate clientID, then proceed
     {
     if (newsockfd < 0)
          error("ERROR on accept");

     pid = fork();
     if(pid < 0)
	error("Error on fork");
     if(pid == 0)
     {
      close(sockfd);
     while(strcmp(buffer, "N\n") != 0 && strcmp(buffer, "") != 0)
 {   
     
     //After a connection a client has successfully connected to the server
     //initialize the buffer using the bzero() function
     bzero(buffer,256);

    //reads from the socket into a buffer for a maximum of 255 characters
    //read call uses new file descriptor, the one returned by accept()
     
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");

    //both the server can read and write after a connection has been established.
    //everything written by the client will be read by the server, and everything written by the server will be read by the client.

//take the input string and copy it to 'copy' after initializing copy
      bzero(copy, 256);
      strcpy(copy, buffer);

       filename = strtok(copy, delim);
       if(filename != NULL)
        operation = strtok(NULL, delim);
       
        
       //file lock, commented out because couldnt break the last bug in it
      /*
       int IDindex= -1;

       for(i = 0; i < 5; i++)
       {
        if(strcmp(fileArray[i], filename) == 0)
         {
          IDindex = i;
         }
       }  
       if(IDindex == -1) 	
        {
         fileArray[globID] = filename;
        }
       else
       {
        if(IDindex != globID)
         printf("ERROR: file lock rejects this file being accessed by client: %d\n", globID);
         //close(newsockfd);
       } 
       //end filelock
       */
 
       printf("contents of fileARray at end of filelock: %s, %s, %s, %s, %s\n", fileArray[0], fileArray[1], fileArray[2], fileArray[3], fileArray[4]);
if(strcmp(operation, "r\n")==0){
       fp = fopen(filename, "r");
       bzero(buffer, 256);
     if(!fp)
      printf("File not found.\n");
	
  while(fgets(buffer,255,fp)!=NULL){
     //fopen the file named in buffer, testing for READ ONLY
          
  
     //read one line from the file
    
     n = write(newsockfd,buffer,strlen(buffer));
     if (n < 0) error("ERROR writing to socket");
    
     bzero(buffer,256);
     n = read(newsockfd,buffer,255); //arbitrary read call to make sure server and client are in sync

     bzero(buffer, 256);
}

    fclose(fp);

// if null, it won't write to the socket, so itll send a message
    bzero(buffer,256);
    strcpy(buffer, "end");
    n = write(newsockfd, buffer, strlen(buffer));
//end of if(operation...)
}//End operation "r"


else if(strcmp(operation, "w\n")==0){
//Creates an empty stack
create();
bzero(buffer, 256);
//read the first line of the file from server
n = read(newsockfd,buffer,255);
        if(n<0)
                error("ERROR reading from socket");
//go into while loop to read in lines until eof
while(strcmp(buffer, "end")!=0){
//push string onto stack
push(buffer);
bzero(buffer,256);
strcpy(buffer, "ready");
n = write(newsockfd,buffer,strlen(buffer));
bzero(buffer,256);
n = read(newsockfd,buffer,255);
        if(n<0)
                error("Error reading from socket");
} // End of while
//**********File operations***********
fp = fopen(filename, "w");
while(empty() == 0){
pop(string);
bzero(buffer,255);
strcpy(buffer,string);
fprintf(fp,"\n%s",buffer);
}
fclose(fp);
destroy();

}//End if operation "w"


bzero(buffer,256);
n = read(newsockfd, buffer, 255);
}

     exit(0);
     }
    else
     //close connections using file descriptors
     close(newsockfd);
 } //end of duplicateFlag if condition
}//*end of while*

     close(sockfd);
     return 0;
}
