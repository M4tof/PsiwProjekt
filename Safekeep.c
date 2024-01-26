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

    if ( strlen(argv[2]) > 2008){
        printf("Key name to big\n");
        return 1;
    }
    char klucz[strlen(argv[2])];
    strcpy(klucz,argv[2]);
    
    int nrOfA = 0, nrOfB = 0, nrOfC = 0;
    int costOfA = 0, costOfB = 0, costOfC = 0;

    int Earnings;

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
    printf("nrOfC: %d, costOfC: %d\n\n", nrOfC, costOfC);
    
    int pdesk = open(klucz,O_RDONLY);
    if (pdesk == -1){
        perror("Opening FIFO\n");
        return 1;
    }

    char goldFIFO[40] = "GOLD_";
    strcat(goldFIFO,klucz);

    int goldDesk = open(goldFIFO,O_WRONLY);
    if (goldDesk == -1){
        perror("Opening second FIFO\n");
        return 1;       
    }

    char OrderFIFO[40];
    strcpy(OrderFIFO,argv[1]);
    OrderFIFO[strlen(OrderFIFO)-10] = '\0';
    strcat(OrderFIFO,"ORDERS");

    if(mkfifo(OrderFIFO,0600) == -1){
        perror("Making order FIFO\n");
        return 1;
    }

    int pipe1[2];

    if(pipe(pipe1) == -1){
        perror("Making pipe kurier->magazyn\n");
        return 1;
    }

    if(fork()==0){
        close(pipe1[1]);

        int OrderDesk = open(OrderFIFO,O_WRONLY);
        if (OrderDesk == -1){
            perror("Opening order fifo on 1 curier\n");
            return 1;
        }

        while(1){
            int recived[4];
            if(read(pdesk,recived,sizeof(recived))<=0){
                break;
            }

            printf("Kurier 1:Zamówienie nr:%d z %d A, %d B, %d C\n",recived[0],recived[1],recived[2],recived[3]);
            
            recived[0] = 1;

            write(OrderDesk,recived,sizeof(recived));
            
            int cost;
            read(pipe1[0],&cost,sizeof(cost));
            
            if(cost >=0){
                write(goldDesk,&cost,sizeof(cost));
            }
            
            else{
                printf("Kurier 1:Nie mozna wykonac zamowienia\n");
                break;
            }

        }

        close(pipe1[0]);
        printf("Kurier 1:konczy prace\n");
        return 0;
    }

    else{
        close(pipe1[0]);

        int OrderDesk = open(OrderFIFO,O_RDONLY);
        if (OrderDesk == -1){
            perror("Opening order fifo on 1 curier\n");
            return 1;
        }

        while(1){
            int reciveddata[4];
            if(read(OrderDesk,reciveddata,sizeof(reciveddata)) <=0 ){
                break;
            }

            if(nrOfA >= reciveddata[2] && nrOfB >= reciveddata[3] && nrOfC >= reciveddata[4]){
                nrOfA -= reciveddata[2];
                nrOfB -= reciveddata[3];
                nrOfC -= reciveddata[4];

                int goldCost = 0;
                goldCost += (reciveddata[2]*costOfA);
                goldCost += (reciveddata[3]*costOfB);
                goldCost += (reciveddata[4]*costOfC);

                write(pipe1[1],&goldCost,sizeof(goldCost));
                printf("Zamowienie kuriera %d mozna wykonac\n",reciveddata[0]);
            }
            else{
                int cannot = -1;
                write(pipe1[1],&cannot,sizeof(cannot));
                printf("Zamowienie kuriera %d jest niewykonywalne\n",reciveddata[0]);
            }

        }
    }

    printf("\nTen magazyn zarobił %d Gold'a\n",Earnings);
    write(goldDesk,&Earnings,sizeof(Earnings));
    
    close(pdesk);
    close(goldDesk);
    
    return 0;
}
