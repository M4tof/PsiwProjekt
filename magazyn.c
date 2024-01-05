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

bool isnum(char fullnumber[]){
    for(int i=0; i<strlen(fullnumber);i++){
        if(!isdigit(fullnumber[i])){
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Incorrect number of arguments\n");
        return 1;
    }

    int desc_zrod = open(argv[1], O_RDONLY);
    if (desc_zrod == -1) {
        printf("Error with opening the file\n");
        return 1;
    }

    if ( strlen(argv[2]) > 2048){
        printf("Key name to big\n");
        return 1;
    }
    char klucz[strlen(argv[1])];
    strcpy(klucz,argv[1]);

    printf("Beginning configuration \n");
    
    // Configuration 
    int nrOfA = 0, nrOfB = 0, nrOfC = 0;
    int costOfA = 0, costOfB = 0, costOfC = 0;

    char buf;
    char data[64];
    int readData = 0;
    int sCase = 1;
    
    while(read(desc_zrod, &buf, 1) > 0){ //Error checking
        readData++;
        if (buf == '\n') {
            lseek(desc_zrod, -readData, SEEK_CUR);
            read(desc_zrod, &data, readData - 1);
            read(desc_zrod, &buf, 1);

            data[readData - 1] = '\0';
            int val = atoi(data);

            switch (sCase) {
                case 1:
                    nrOfA = val;
                    break;
                case 2:
                    costOfA = val;
                    break;
                case 3:
                    nrOfB = val;
                    break;
                case 4:
                    costOfB = val;
                    break;
                case 5:
                    nrOfC = val;
                    break;
                case 6:
                    costOfC = val;
                    break;
                default:
                    break;
            }
            sCase++;
            readData = 0;
        }
    }

    printf("nrOfA: %d, costOfA: %d\n", nrOfA, costOfA);
    printf("nrOfB: %d, costOfB: %d\n", nrOfB, costOfB);
    printf("nrOfC: %d, costOfC: %d\n", nrOfC, costOfC);

    //make 3 curiers
    //open klucz
    //main loop

    // KYS
    return 0;
}
