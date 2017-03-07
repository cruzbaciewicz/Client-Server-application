//Name of Author: Robert Hakimi and Cruz Baciewicz
//Course Number and Name: CSE 434, Computer Networks
//Semester: Fall 2016
//Project Part: 2
//Time Spent: 15 Hours

//header files

//input - output declarations included in all C programs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//contains definitions of a number of data types used in system calls
#include <sys/types.h>

//definitions of structures needed for sockets
#include <sys/socket.h>

//in.h contains constants and structures needed for internet domain addresses
#include <netinet/in.h>

//netdb.h defines the structure hostent,
#include <netdb.h>

//***********************STACK STUFF*****************************
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
 
    //printf("\n All stack elements destroyed\n");
    count = 0;
}

//****************************END STACK STUFF***************************





//This function is called when a system call fails. It displays a message about the error on stderr and then aborts the program.
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//********************************************MAIN**********************************
int main(int argc, char *argv[])
{
if(argc != 4)
{
  printf("Usage: ./client <hostname> <client number> <server port>\n");
  return;
}
//variable declarations

//sockfd is a file descriptor
//portno stores the port number on which the server accepts connections.
    int i;

    char inputchar;
    int sockfd, portno, n;
    char copy[256];
    char string[256];				//used to copy the input message in order to use strtok without modifying the original string	
    char *filename, *operation, *extra;	//used to store the filename and operation from the string tokenator
    const char delim[2] = ", ";

    // File pointer
    FILE *fp;    


//serv_addr will contain the address of the server

    struct sockaddr_in serv_addr;

//variable server is a pointer to a structure of type hostent
/*
struct  hostent
{
  char    *h_name;        // official name of host
  char    **h_aliases;    // alias list
  int     h_addrtype;     // host address type
  int     h_length;       // length of address
  char    **h_addr_list;  // list of addresses from name server
  #define h_addr  h_addr_list[0]  // address, for backward compatiblity
r
};<F4>
*/
    struct hostent *server;
    char buffer[256];


//atoi() function can be used to convert port number from a string of digits to an integer, if your input is in the form of a string.
    portno = atoi(argv[3]);
    char *clientID;
    clientID = argv[2];
//create socket
//it take three arguments - address domain, type of socket, protocol (zero allows the OS to choose thye appropriate protocols based on type of socket)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
//funcion gethostbyname() takes name as an argument and returns a pointer to a hostent containing information about that host
//name is taken as an argument from the user
    server = gethostbyname(argv[1]);

//If hostent structure is NULL (after the above assignment), the system could not locate a host with this name
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

//set server address buffer with zeros using bzero or memset
//two arguments - pointer to buffer and sizeof buffer
    bzero((char *) &serv_addr, sizeof(serv_addr));

//following code sets the fields in serv_addr
//contains a code for the address family
    serv_addr.sin_family = AF_INET;

//copies length bytes from s1 to s2
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

//contain the port number
    serv_addr.sin_port = htons(portno);

//connect function is called by the client to establish a connection to the server. It takes three arguments, the socket file descriptor, the address of the host to which it wants to connect (including the port number), and the size of this address
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    //printf("Please enter the message: ");

//After a connection a client has successfully connected to the server
//initialize the buffer using the bzero() function
    
   //send clientID to server to check for uniqueness
   write(sockfd,clientID,strlen(clientID));
   char IDresponse[256];

   bzero(IDresponse, 256);
   read(sockfd, IDresponse, 255);
  if (strcmp(IDresponse, "ok") != 0)
   printf("%s\n", IDresponse); 
  else
 {
   while(strcmp(buffer, "N\n") != 0)
   
{



   printf("Please enter the request: ");
//set the buffer to the message entered on console at client end for a maximum of 255 characters
    bzero(buffer,256);
    fgets(buffer,255,stdin);

//take the input string and copy it for modification
    bzero(copy,256);
    strcpy(copy, buffer);    


//break up the request, filename gets the filename, operation gets "r" or "w"

    filename = strtok(copy, delim);	
    if(filename != NULL){
	operation = strtok(NULL, delim);
    }
   
if(strcmp(operation, "r\n")==0){
//write from the buffer into the socket
    n = write(sockfd,buffer,strlen(buffer));

//check of the write function was successful or not
    if (n < 0)
         error("ERROR writing to socket");
//Creates an empty stack
create();
bzero(buffer, 256);


//read the first line of the file from server

n = read(sockfd,buffer,255);

	if(n<0)
		error("ERROR reading from socket");

//go into while loop to read in lines until eof
while(strcmp(buffer, "end")!=0){
//push string onto stack
push(buffer);
bzero(buffer,256);
strcpy(buffer, "ready");
n = write(sockfd,buffer,strlen(buffer));
bzero(buffer,256);
n = read(sockfd,buffer,255);
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

}//End if operation "r"



else if(strcmp(operation, "w\n")==0){
n = write(sockfd, buffer,strlen(buffer));


 fp = fopen(filename, "r");
       bzero(buffer, 256);
   if(!fp)
     printf("File not found.\n");

  while(fgets(buffer,255,fp)!=NULL){
     //fopen the file named in buffer, testing for READ ONLY

    
     //read one line from the file

     n = write(sockfd,buffer,strlen(buffer));
     if (n < 0) error("ERROR writing to socket");

     bzero(buffer,256);
     n = read(sockfd,buffer,255); //arbitrary read call to make sure server and client are in sync

     bzero(buffer, 256);
}

    fclose(fp);

// if null, it won't write to the socket, so itll send a message
    bzero(buffer,256);
    strcpy(buffer, "end");
    n = write(sockfd, buffer, strlen(buffer));
//end of if(operation...)
}//End of operation "w"

//both the server can read and write after a connection has been established.
//everything written by the client will be read by the server, and everything written by the server will be read by the client.
    printf("Go ahead with another request? Y/N?\n");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

//write from the buffer into the socket
   
    n = write(sockfd,buffer,strlen(buffer));
    
}//*end of while*
} //end of "ok" else condition  
  
//close connections using file descriptors
    close(sockfd);
    return 0; 

}
