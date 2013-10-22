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
    
    printf("Hi. welcome to chell. You look fat\n");
    
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

            printf("%s\n", first);
            if(second){
                printf("%s\n", second);
            }

            if(!strcmp(first, "quit")){
                printf("Good riddance!\n");
                exit = 1;
            }

            else if(!strcmp(first, "type")){
                if(second){
                    size_t last = strlen (second) - 1;
                    input[last] = 0;
                    printf("%s\n", second);
                    type(second);
                }
                
            }
            else if(!strcmp(first, "copy")){
                if(second && third){
                    size_t last = strlen (second) - 1;
                    input[last] = 0;
                    printf("%s\n", second);
                    copy(second, third);
                }
               // copy(src, dest);
            }
            else if(!strcmp(first, "delete")){
                if(second){
                    size_t last = strlen (second) - 1;
                    input[last] = 0;
                    printf("%s\n", second);
                    delete(second);
                }
            }
            else{
                int success = execute(first);
                if(success){
                    printf("Command not recognized: %s \n", input);
                }
                
            }


            free(first);
            free(second);
        }


        
    }

    return (EXIT_SUCCESS);
}


void copy(char* src, char * dest){
    printf("executing copy command\n");
    
    char c;
    FILE * fileIn;
    FILE * fileOut;
    fileIn = fopen(src, "r");
    fileOut = fopen(dest, "w");

    if(src == NULL){
        printf("Could not open file %s \n", src);
        return;
    }
    if(fileOut == NULL){
        printf("Could not create/open file %s \n", dest);
        return;
    }
    if(fileIn) {
        while ((c = getc(fileIn)) != EOF)
                putc(c, fileOut);
        fclose(fileIn);
        fclose(fileOut);
    }
    printf("\n");
}

void delete(char* filename){
    remove(filename);
    
}

int execute(char* filename){
    char args[1][1];
    args[0][0]='\0';

    if (execvp(filename, args) < 0) {     /* execute the command  */
       printf("Execution of %s failed\n", filename);
       return 0;
    }
    return 1;

}
void type(char* filename){
    printf("executing type command on %s \n", filename);
    
    char c;
    FILE * file = NULL;
    file = fopen(filename, "r");


    if(file) {
        while ((c = getc(file)) != EOF)
            putchar(c);
        fclose(file);
    }
    printf("\n");
    
}