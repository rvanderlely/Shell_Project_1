/*************************************************************************
*	Projec name: Shell Project 1
*	Author:		 Rachel Vanderlely
*   Professor:   Dr.Zhao
*	Date:		 Sept 12, 2022
*	Class:		 CS3513
*	Purpose:	 Execute a command on a shell with up to 3 optional arguments. 
8                If command is found then it runs. If command 
*                is not found an error message is delivered. 
****************************************************************************/

#include <stdio.h>      //for printf() and scanf()
#include <string.h>     //strlen(), strcpy(), strcmp()
#include <unistd.h>     //execl()
#include <stdlib.h>     //exit()
#define SIZE 80

int main()
{
	int status;                 //Holds the childs return status
	char User_Input[SIZE];       //store all user input from the keyboard
	int pathcounter = 0;	     //Counter for the number of path being checked
	int argumentcounter = 0;     //Counts number of arguments passed in    
	char *full_path;                     //points to entire path fetched from system. Includes all paths commands may be stored in. 
	char *paths[SIZE];           //this is a 2D array that holds pointer. Pointers point to the cstrings paths that will be checked. 
	char *temp;                  //store one path temp
	full_path = getenv("PATH");  //get enviorment then path// so after this p will point to the path string and one of them will be bin
                                 //PATH simply returns all directories, seperated by a : that executible can be stored in. 
                                 //For example, PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin
	
	temp = strtok(full_path, ":");         //Break up the fullpath into individual paths and parse into 2D array called paths
    while(temp)                            //parsing path into 2_d array
	{
		paths[pathcounter] = temp;          //store Individual paths in 2D paths array 
		pathcounter++;				//keep increasing through the paths
		temp = strtok(NULL, ":");	// : is the deliminator that you are specifiying. In this case it is a colon.
	}	                            //on the last one temp will be null and then the while will stop 
	
	for(;;)                         //service loop until the user quits
	{
		printf("\nRachel's Shell %s >>> ", getenv("HOME"));
		fgets(User_Input,80,stdin);                                         //Reads in user command and any arguments
		User_Input[strlen(User_Input)-1] = '\0';	                        //need to remove the /n from pressing enter \0 will overwrite this character. 

		if(strcmp(User_Input, "exit") == 0)        //If at any time the user types exit, then exit the  program.
        {                                           //THIS IS in the infinite for loop so will work at any time. 
            printf("\n Goodbye. \n");
            exit(0);
        }

		if(fork() == 0)
		{
			char *Local_Command; 					            //Make a cstring to hold local command
		 	Local_Command = strtok(User_Input, " ");	        //Parse first word/command entered into the local command cstring
            
            char *Local_Argument[3];                  //Make a 2D array to hold the pointer that point to the cstring arguments
			while(argumentcounter < 3 )               //parsing up to 3 args into 2D array
			{
                Local_Argument[argumentcounter]  = strtok(NULL, " ");;//Store any arguments into the 2D local argument array
                argumentcounter++;	                                  //Incriment the counter
			}

            int k;
            for(k = 0; k < pathcounter; k++)
            {
                //Iterate through all paths in 2D array called paths,to check if 
                //users command is located in any of the avaiable paths
                char path[SIZE]; 			//Make a new 1D array of size 80 to store one path at a time	
                strcpy(path, paths[k]);		//Copy the kth array in the 2D array to this new 1D array
                strcat(path, "/");			//Put a / after each path added 
                strcat(path, User_Input);	//add the command that the user typed to the path
                //Execute the command and any arguments. NULL is accepted is no arguments are placed.
                //If execute statement is successful then if statment is ended meaning
                //The print and exit commands below will not run if execl is run properly. 
                execl(path, Local_Command,Local_Argument[0],Local_Argument[1],Local_Argument[2], 0);
            }
            printf("\nError. Command not found.\n");
            exit(1);//terminate child process
				}
				else
				{
					wait(&status);//wait for child process term this gives a value based on how child process is terminated
					//The operating system passes a number and stores it in status location. 
					printf("The child process ended with a status of: %d",status);
				}
	}//End the infinte service loop.

exit(0);
} //end of main
	