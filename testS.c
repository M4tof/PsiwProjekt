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

#define sleep_time 0.5

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    char klucz[strlen(argv[1])];
    strcpy(klucz,argv[1]);
    
    if (mkfifo(klucz,0600) == -1){
        perror("Making FIFO\n");
        return 1;
    }
    int A, B, C;
    
    //main loop
    int pdesk = open(klucz,O_WRONLY);
    if (pdesk == -1){
        perror("Opening FIFO\n");
        return 1;
    }

    int i=1;
    while(1){
        sleep(sleep_time);
        A = rand() % 2;
        B = rand() % 4;
        C = rand() % 5;
        printf("Zamowienie %d -> A:%d , B:%d , C:%d \n",i++,A,B,C);
        int message[3];
        message[0] = A;
        message[1] = B;
        message[2] = C;
        write(pdesk,message,3); 
    }

    unlink(klucz);
    return 0;
}