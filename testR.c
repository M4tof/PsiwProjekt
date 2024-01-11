#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    char klucz[strlen(argv[1])];
    strcpy(klucz,argv[1]);

    int pdesk = open(klucz,O_RDONLY);
    if (pdesk == -1){
        perror("Opening FIFO\n");
        return 1;
    }
    
    //main loop
    while(1){
        int data[3];
        read(pdesk,data,sizeof(data));
        printf("Zamówienie z %d A, %d B, %d C\n",data[0],data[1],data[2]);
    }

    return 0;
}
