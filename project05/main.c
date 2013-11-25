/* 
 * File:   main.c
 * Author: derek
 *
 * Created on October 21, 2013, 10:55 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>


void copy(char* src, char * dest);
void delete(char* filename);
int execute(char* filename);
void type(char* filename);



/*
 * 
 */
int main(int argc, char** argv) {
    
    char* input;
    int exit = 0;
    int BUFFER_LENGTH = 100;
    char line[BUFFER_LENGTH];
    int thirteen;

    thirteen = (int)("\r" - '\0');
    
    printf("%d\n", thirteen );
    printf("Hi. Welcome to chell. What do you want\n");
    
    while(!exit){
        memset(line, 0, BUFFER_LENGTH);
        printf("$ ");
        input = fgets (line, BUFFER_LENGTH, stdin);

        //also need to check for just spaces
        if(input && strcmp(input, "\n")){

            if (input != NULL) {
                size_t last = strlen (input) - 1;
                if (input[last] == '\n') input[last] = 0;
            }
            
            char* first = NULL;
            char* second = NULL;
            char* third = NULL;
            char* result = NULL;

            char* arg[3];

            int i = 0;
            result = strtok(input," ");
            while(result != NULL){
                arg[i] =(char*) malloc(100 * sizeof(char));
                strcpy(arg[i], result);
                result = strtok(NULL, " ");
                i++;
            }

            first = arg[0];
            if(i>1)
                second = arg[1];
            if(i>2)
                third = arg[2];

            /*printf("%s\n", first);
            if(second){
                printf("%s\n", second);
            }*/

            if(!strcmp(first, "quit") || !strcmp(first, "exit")){
                printf("Good riddance!\n");
                exit = 1;
            }

            else if(!strcmp(first, "type")){
                if(second){
                    size_t last = strlen (second) - 1;
                    input[last] = 0;
                    type(second);
                }
                else{
                    printf("Hey bud, you have to follow 'type' with a filename\n");
                }
                
            }
            else if(!strcmp(first, "copy")){
                if(second && third){
                    size_t last = strlen (second) - 1;
                    input[last] = 0;
                    copy(second, third);
                }
                else{
                    printf("Good one, Champ. You're missing argument(s)\n");
                }
               // copy(src, dest);
            }
            else if(!strcmp(first, "delete")){
                if(second){
                    size_t last = strlen (second) - 1;
                    input[last] = 0;
                    delete(second);
                }
                else{
                    printf("Really? Delete what? There's nothing there\n");
                }
            }
            else{
                int success = execute(first);
                if(!success){
                    printf("Your command <%s> is bad and you should feel bad\n", input);
                }
                
            }


            free(first);
            free(second);
        }


        
    }

    return (EXIT_SUCCESS);
}


void copy(char* src, char * dest){
    char c;
    FILE * fileIn;
    FILE * fileOut;
    fileIn = fopen(src, "r");
    fileOut = fopen(dest, "w");

    if(src == NULL){
        printf("I can't open this file <%s> \n", src);
        return;
    }
    if(fileOut == NULL){
        printf("I can't open and/or make this file <%s> \n", dest);
        return;
    }
    if(fileIn) {
        while ((c = getc(fileIn)) != EOF)
                putc(c, fileOut);
        fclose(fileIn);
        fclose(fileOut);
    }
    else{
        printf("I couldn't even copy that if I tried\n");
    }
}

void delete(char* filename){
    if(remove(filename)<0){
        printf("that stupid file couldn't be deleted\n");
    }
}

int execute(char* filename){

    pid_t  pID;
    int status;

    char args[1][1];
    args[0][0]='\0';
    int forked;
    pID = fork();
    if (pID < 0) {    //fork
         printf("Failed to execute %s", filename);
          return 0;
    }
    else if (pID == 0) {          //child
        if (execvp(filename, args) < 0) {    
            //printf("Execution of %s failed\n", filename);
            return 0;
        }
    }
    else { 
        while (wait(&status) != pID){
            //Wait and do nothing
        } 
               
    }


    return 1;

}
void type(char* filename){

    char c;
    FILE * file = NULL;
    file = fopen(filename, "r");

    if(file) {
        while ((c = getc(file)) != EOF)
            putchar(c);
        fclose(file);
    }
    else{
        printf("Yea, %s ain't gonna work. Try again\n", filename);
    }
    printf("\n");
    
}