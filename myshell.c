#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 100
#define MAXINSTRACTIONSIZE 1000
#define FORKFAILED -1
#define CHILDFORKID 0

/* bool define */
typedef enum {false, true} bool;

/* Make an array to save the command history in order */
char CommandHistoryArray[BUFFER_SIZE][MAXINSTRACTIONSIZE];
int currentNumberOfCommands = 0;

int main(void)
{
    close(2);
    dup(1);
    char command[BUFFER_SIZE];
    char *args[BUFFER_SIZE];
    bool runInBackGround;
    int j1=0;

    while (1) {

        fprintf(stdout, "my-shell> ");
        fflush(stdout);
        memset(command, 0, BUFFER_SIZE);
        fgets(command, BUFFER_SIZE, stdin);
        if (strncmp(command, "exit", 4) == 0) {
            break;
        }

        /* Check if we need to copy a command */
        if('!' == command[0])
        {
            if('!' == command[1])
            {
                strcpy(command,CommandHistoryArray[currentNumberOfCommands-1]);
            }
            else
            {
                int numOfErrors=0; // this is for illegal input
                if(command[1]!='\0')
                {
                    if(command[1]>'9' || command[1]<'0')numOfErrors=1;
                int num=command[1]-'0';
                j1=2;
                while(command[j1]>='0' && command[j1]<='9')
                {
                    num*=10;
                    if(command[j1]>'9' || command[j1]<'0')numOfErrors++;
                    num+=command[j1++]-'0';
                }
                if(numOfErrors==0)
                {
                      if(num>currentNumberOfCommands)
                      {
                       fprintf(stdout, "No History\n");
                       strcpy(command, "\n");
                      }
                      else
                      {
                        if(num!=0)
                        {
                            strcpy(command,CommandHistoryArray[num-1]);
                        }
                      }
                }
                }
            }
        }

        if (!strcmp(command, "history\n")) {
            strcpy(CommandHistoryArray[currentNumberOfCommands++], command);
            int i;
            for (i = currentNumberOfCommands-1; i >=0; i--) {
                fprintf(stdout, "%d      %s", i + 1, CommandHistoryArray[i]);
            }
        }
        else
        {
            /* Check if the command is not empty */
            if (!(command[0] == '\n')) {

            /* Insert the current command to the command history array */
           strcpy(CommandHistoryArray[currentNumberOfCommands++], command);

            /* Remove '\n' at the end of command (because it dont help us)*/
            command[strlen(command) - 1] = '\0';


            /* Get the string before the first space. The string will represent the Linux command */
            args[0] = strtok(command, " ");
            int i = 0;
            while (args[i] != NULL) {
                /* this will help us get all the arguments in the string*/
                args[++i] = strtok(NULL, " ");
            }

            /* Check if the last argument is  &  */
            i--;
            if(!strcmp(args[i],"&"))
            {
                runInBackGround=true;
            }
            else
            {
                runInBackGround=false;
            }

            if (runInBackGround) {
                args[i] = NULL;
            }/*if we are going to run in background we dont need the "&" for executing the comand*/

            /* saving the return value of fork */
            pid_t pid = fork();

            switch (pid) {

            /* Checking if fork failed */
            case FORKFAILED:
                perror("error");
                exit(FORKFAILED);

            /* Checking if the current process is the child */
            case CHILDFORKID:
                if (execvp(args[0], args) == FORKFAILED) {
                    perror("error");
                    exit(FORKFAILED);
                }
                break;

            /* default case is the father process */
            default:
                /* Check if there's '&' as the last argument */
                if (!runInBackGround) {
                    /* Wait for child to finish his work */
                    while (wait(NULL) != pid);
                }
            }


        }

        }
    }
    return 0;
}

