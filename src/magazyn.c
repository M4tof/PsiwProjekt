#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#define Kurier2Magazyn 4
#define Kurier1 1
#define Kurier2 2
#define Kurier3 3
struct msgb{
    long t;
    int from;
    int A;
    int B;
    int C;
    int gold;
};

bool isnum(char fullnumber[]){
    for(int i=0; i<strlen(fullnumber);i++){
        if(!isdigit(fullnumber[i])){
            return false;
        }
    }
    return true;
}

void timeout(){
    sleep(150);
    kill(getppid(),SIGKILL);
    printf("JEDEN Z KURIEROW DOSZEDL DO LIMITU CZASU\n");
    exit(0);
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

    int Earnings = 0;

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

    key_t qid = msgget(IPC_PRIVATE,IPC_CREAT|0660);

    if(fork()==0){ //Kurier 1
        struct msgb w;

        while(1){
            int recived[4];

            int cpid = fork();
            if(cpid == 0){
                timeout();
            }
            int child_pid = cpid;

            if(read(pdesk,recived,sizeof(recived))<=0){
                break;
            }
            
            kill(child_pid, SIGKILL);

            printf("Kurier 1: Zamówienie nr:%d z %d A, %d B, %d C\n",recived[0],recived[1],recived[2],recived[3]);
            
            w.t = Kurier2Magazyn;
            w.from = Kurier1;
            w.A = recived[1];
            w.B = recived[2];
            w.C = recived[3];
            w.gold = 0;

            msgsnd(qid,&w,sizeof(struct msgb),0);

            msgrcv(qid,&w,sizeof(struct msgb),Kurier1,0);
            if(w.gold == -1){
                break;
            }
            else{
                int goldCost2 = w.gold;
                write(goldDesk,&goldCost2,sizeof(goldCost2));
                printf("Kurier 1: Zamowienie %d wykonane\n\n",recived[0]);
            }
        }
        printf("Kurier 1: Konczy prace\n");
        w.t = Kurier2Magazyn;
        w.from = -1;
        msgsnd(qid,&w,sizeof(struct msgb),0);
        return 0;
    }

    if(fork()==0){ //Kurier 2
        struct msgb w;

        while(1){
            int recived[4];
            
            int cpid = fork();
            if(cpid == 0){
                timeout();
            }
            int child_pid = cpid;

            if(read(pdesk,recived,sizeof(recived))<=0){
                break;
            }
            
            kill(child_pid, SIGKILL);

            printf("Kurier 2: Zamówienie nr:%d z %d A, %d B, %d C\n",recived[0],recived[1],recived[2],recived[3]);
            
            w.t = Kurier2Magazyn;
            w.from = Kurier2;
            w.A = recived[1];
            w.B = recived[2];
            w.C = recived[3];
            w.gold = 0;

            msgsnd(qid,&w,sizeof(struct msgb),0);

            msgrcv(qid,&w,sizeof(struct msgb),Kurier2,0);
            if(w.gold == -1){
                break;
            }
            else{
                int goldCost2 = w.gold;
                write(goldDesk,&goldCost2,sizeof(goldCost2));
                printf("Kurier 2: Zamowienie %d wykonane\n\n",recived[0]);
            }
        }
        printf("Kurier 2: Konczy prace\n");
        w.t = Kurier2Magazyn;
        w.from = -1;
        msgsnd(qid,&w,sizeof(struct msgb),0);
        return 0;
    }

    if(fork()==0){ //Kurier 3
        struct msgb w;

        while(1){
            int recived[4];
            
            int cpid = fork();
            if(cpid == 0){
                timeout();
            }
            int child_pid = cpid;

            if(read(pdesk,recived,sizeof(recived))<=0){
                break;
            }
            
            kill(child_pid, SIGKILL);

            printf("Kurier 3: Zamówienie nr:%d z %d A, %d B, %d C\n",recived[0],recived[1],recived[2],recived[3]);
            
            w.t = Kurier2Magazyn;
            w.from = Kurier3;
            w.A = recived[1];
            w.B = recived[2];
            w.C = recived[3];
            w.gold = 0;

            msgsnd(qid,&w,sizeof(struct msgb),0);

            msgrcv(qid,&w,sizeof(struct msgb),Kurier3,0);
            if(w.gold == -1){
                break;
            }
            else{
                int goldCost2 = w.gold;
                write(goldDesk,&goldCost2,sizeof(goldCost2));
                printf("Kurier 3: Zamowienie %d wykonane\n\n",recived[0]);
            }
        }
        printf("Kurier 3: Konczy prace\n");
        w.t = Kurier2Magazyn;
        w.from = -1;
        msgsnd(qid,&w,sizeof(struct msgb),0);
        return 0;
    }

    else{ //Magazyn
        struct msgb w0;
        int active = 3;
        while(1){
            msgrcv(qid,&w0,sizeof(struct msgb),Kurier2Magazyn,0);
            if (w0.from == -1){
                active--;
                if(active == 0){
                    printf("Magazyn: Brak kurierow\n");
                    break;
                }
            }

            else{
                w0.t = w0.from;

                if(nrOfA >= w0.A && nrOfB >= w0.B && nrOfC >= w0.C){
                    nrOfA -= w0.A;
                    nrOfB -= w0.B;
                    nrOfC -= w0.C;

                    int goldCost = 0;
                    goldCost += (w0.A*costOfA);
                    goldCost += (w0.B*costOfB);
                    goldCost += (w0.C*costOfC);
                    Earnings += goldCost;
                    
                    printf("Magazyn: Zarobiono: %d\n",goldCost);
                    printf("Magazyn: uznaje za możliwe zadanie kuriera%d \n",w0.from);
                    w0.gold = goldCost;
                    
                }
                else{
                    printf("Magazyn: nie moze wykonac zamowienia kuriera%d \n",w0.from); 
                    w0.gold = -1;
                }
                msgsnd(qid,&w0,sizeof(struct msgb),0);
            }    
        }

    }


    printf("\nMagazyn: zarobił %d Gold'a\n",Earnings);
    printf("Magazyn: pozostało A:%d B:%d C:%d \n",nrOfA,nrOfB,nrOfC);
    
    close(pdesk);
    close(goldDesk);

    kill(-getpid(),SIGTERM);
    
    return 0;
}
