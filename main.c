/* 
 * File:   530shell.c
 * Author: Nikki
 *  Creating a simple linux shell.This program is designed to take in user input through stdin 
 * and forks a new process then it parses the input into arguments(commands ) to be executed.  
 * Created on September 16, 2014, 2:16 PM
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#define PROMPT "% " //this is the constant used for the prompt it is a % followed by a space for clarity
#define LINE 81  //max amount of chars per line
#define WORDS 11  //max amount of commands "words" or combos of letters seperated by spaces
#define WHITESPACE " \t\r\n\f"  //every form of whitespace accounted as a delimiter
#define ARGS 1 // number of arguments to be checked in main shell
/*
 * 
 */

void parse(char userInput[]); //function to parse user input into commands and words
void exec(char* userInputParsed[]);//function to execute parsed input

int main(int num_args, char* arg_strings) {//processing command line arguments
    if (num_args != ARGS) {//only can input one argument
        fprintf(stderr,"Error, Program takes %d argument\n", ARGS);
        exit(-1);
    }

    int status;//  pid status
    pid_t childPID; // initialize processes

    fputs(PROMPT,stdout); //prints % to stdout
    char commands[LINE]; //initializes string for user input with max amount LINE

    
    while (fgets(commands, LINE, stdin)!= NULL) { //keep getting commands till null terminator
        
        if (strlen(commands) > LINE-2) {//error handling if command is too long check length 
            
           fprintf(stderr,"Error, You have entered too many characters\n");// then print error message
          
          while (getchar() != '\n' && !feof(stdin));// iterate through but do nothing in order to only print once
          
          
        }
       else
        {
        
       
        childPID = fork(); //forks process

        if (childPID < 0) {// if child process is -1 or less print error and exit child process
          
            fprintf(stderr,"child was not forked");

            exit(-1);
        }
        if (childPID == 0) { 

            parse(commands);// send input to parse() function when child process success

            exit(0);//successfully close
        } else {
            
            if (waitpid(childPID, &status, 0) !=childPID) {// parent process wait for child to terminate and stop zombie process
                fprintf(stderr,"waitpid is not responding");// send error 
            } else
                if (!WIFEXITED(status)) { //checks if child process ended normally 
                fprintf(stderr,"Did not end normally, check for zombies\n");//if not print error
            }

           


        }
      
       
        }
        putc('\n', stdout);//flush child
       fputs(PROMPT,stdout); //prompt again
    }
    exit(0);//successful exit


}

void parse(char userInput[]) {//parses user input into whitespace split tokens
    int len, i;
    char* token[WORDS];
    i = 0;
    token[i] = strtok(userInput, WHITESPACE);//splits userinput into words based on whitespace constant
    while (token[i] != NULL) { // repeat till a null terminator is seen
        token[i] = strdup(token[i]);//copy of  tokens
        if (i == WORDS) {//if two many commands/space seperated tokens/paths
           
            fprintf(stderr,"Error,You entered too many arguments. Please enter less than %d arguments\n", WORDS - 1);
            //exit child and print error message
            exit(-1);
        }
        len = strlen(token[i]);//assign strlen of token to len dynamically
        if (strlen(token[i]) > 0) //if theres more than one item
            if (token[i][len - 1] == '\n') //last item is a newline
                token[i][len - 1] = '\0';//assign to null
        token[++i] = strtok(NULL, WHITESPACE);//iterate ++
    }


    exec(token);//send tokens to execute function
    int j;
    for (j = 0; j <LINE ; j++) {// free up mem
        free(token[j]);
    }
}

void exec(char *userInputParsed[]) {//takes in parsed input 

    int statement;

    statement = execvp(userInputParsed[0], userInputParsed);// takes first pointer as command(filename) and rest as helpers and NULL
    if (statement < 0) {//if execvp returns negative
      
        perror("execvp");//catch print error exit child
        exit(-1);

    }
}