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

bool isnum(char fullnumber[]){
    for(int i=0; i<strlen(fullnumber);i++){
        if(!isdigit(fullnumber[i])){
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]){
    //setup
    srand(time(NULL));
    int gold = 0;

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

    if ((!isnum(argv[2]))||(!isnum(argv[3]))||(!isnum(argv[4]))||(!isnum(argv[5]))){
        printf("One or more of the arguments that should be an number, isn't\n");
        return 1;
    }

    int liczba_zamównień = atoi(argv[2]);
    int max_A_per_zam = atoi(argv[3]);
    int max_B_per_zam = atoi(argv[4]);
    int max_C_per_zam = atoi(argv[5]);

    printf("Program z kluczem %s, liczba zamowien %d, i maksymalnymi wartosciami %d %d %d.\n",klucz, liczba_zamównień,max_A_per_zam,max_B_per_zam,max_C_per_zam);
    
    int A, B, C;
    
    //main loop
    for (int i = 1; i<=liczba_zamównień; i++){
        sleep(sleep_time);
        A = rand() % max_A_per_zam;
        B = rand() % max_B_per_zam;
        C = rand() % max_C_per_zam;
        printf("Zamowienie %d -> A:%d , B:%d , C:%d \n",i,A,B,C);
    }

    //Closing statement


    //wait for all mag to close
    // print out payed out money
    return 0;
}