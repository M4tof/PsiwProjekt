#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define sleep_time 2

int main(int argc, char* argv[]){
    //setup pt 1
    srand(time(NULL));
    int gold = 0;

    //Error check and setup pt2 
    if (argc < 6) {
        printf("Incorect number of arguments\n");
        return 1;
    }
       
    if ( strlen(argv[1]) > 2048){
        printf("Key name to big\n");
        return 1;
    }
    char klucz[strlen(argv[1])];
    strcpy(klucz,argv[1]);
    //if failed to open klucz

    // if numbers not numbers
    int liczba_zamównień = atoi(argv[2]);
    int max_A_per_zam = atoi(argv[3]);
    int max_B_per_zam = atoi(argv[4]);
    int max_C_per_zam = atoi(argv[5]);

    printf("Program z kluczem %s, liczba zamowien %d, i maksymalnymi wartosciami %d %d %d.\n",klucz, liczba_zamównień,max_A_per_zam,max_B_per_zam,max_C_per_zam);
    
    int A, B, C;
    
    //main loop
    for (int i = 0; i<liczba_zamównień; i++){
        sleep(sleep_time);
        A = rand() % max_A_per_zam;
        B = rand() % max_B_per_zam;
        C = rand() % max_C_per_zam;
        printf("Chce A:%d , B:%d , C:%d \n",A,B,C);
    }

    //Closing statement


    //wait for all mag to close
    // print out payed out money
    return 0;
}