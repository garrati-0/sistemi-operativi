#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 

int main(void){
    execlp("ls", "ls", "-l","20gennaio2021", NULL);
}