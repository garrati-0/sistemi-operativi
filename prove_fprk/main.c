#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    
    int a=43566;
    if(fork()==0)
    {
       // printf("%d\n",a);
        a=5;
        printf("%d\n",a);
        exit(0);
    }

    printf("a nel processo padre %d\n",a);
    
}