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
#include <sys/wait.h>

#define sleep_time 2.5

bool isnum(char fullnumber[]){
    for(int i=0; i<strlen(fullnumber);i++){
        if(!isdigit(fullnumber[i])){
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    if (argc < 6) {
        printf("Incorect number of arguments\n");
        return 1;
    }
       
    if ( strlen(argv[1]) > 2008){
        printf("Key name to big\n");
        return 1;
    }

    if ((!isnum(argv[2]))||(!isnum(argv[3]))||(!isnum(argv[4]))||(!isnum(argv[5]))){
        printf("One or more of the arguments that should be an number, isn't\n");
        return 1;
    }

    char klucz[strlen(argv[1])];
    strcpy(klucz,argv[1]);
    if (mkfifo(klucz,0600) == -1){
        perror("Making FIFO\n");
        return 1;
    }

    char goldFIFO[40] = "GOLD_";
    strcat(goldFIFO,klucz);
    if(mkfifo(goldFIFO,0600) == -1){
        perror("Making FIFO\n");
        return 1;
    }



    int liczba_zamównień = atoi(argv[2]);
    int max_A_per_zam = atoi(argv[3]);
    int max_B_per_zam = atoi(argv[4]);
    int max_C_per_zam = atoi(argv[5]);

    printf("Program z kluczem %s, liczba zamowien %d, i maksymalnymi wartosciami %d %d %d.\n",klucz, liczba_zamównień,max_A_per_zam,max_B_per_zam,max_C_per_zam);
    
    int A, B, C;
    
    int pdesk = open(klucz,O_WRONLY);
    if (pdesk == -1){
        perror("Opening FIFO\n");
        return 1;
    }

    int goldDesk = open(goldFIFO,O_RDONLY);
    if (goldDesk == -1){
        perror("Opening second FIFO\n");
        return 1;
    }

    for (int i = 1; i<=liczba_zamównień; i++){
        sleep(sleep_time);
        
        A = rand() % max_A_per_zam;
        B = rand() % max_B_per_zam;
        C = rand() % max_C_per_zam;
        
        printf("Zamowienie %d -> A:%d , B:%d , C:%d \n",i,A,B,C);
        
        int message[4];
        message[0] = i;
        message[1] = A;
        message[2] = B;
        message[3] = C;
        write(pdesk,message,sizeof(message)); 
    }

    printf("Wszystkie zamowienia wyslane\n\n");
    close(pdesk);
    
    sleep(sleep_time);
    
    int goldTotal = 0;
    while(1){
        int gold;
        if(read(goldDesk,&gold,sizeof(gold)) <= 0 ){
            break;
        }
        else{
            goldTotal += gold;
            printf("Zapłacono Gold'a: %d \n",gold);
        }
        
    }
    
    printf("\nOstatecznie wszystkie zamowienia kostowaly => %d Gold'a\n",goldTotal);


    unlink(klucz);
    unlink(goldFIFO);
    return 0;
}