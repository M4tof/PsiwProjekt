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
#include <errno.h>

#define sleep_time 1.5

int main(int argc, char* argv[]){
    int pdesk;

    mkfifo("/tmp/fifo",0600);
    if(fork()){
        pdesk = open("/tmp/fifo",O_WRONLY);
        
        for(int i = 1; i < 10; i++){
            printf("Sending %d\n",i);
            write(pdesk,&i,sizeof(i));
            sleep(sleep_time);  
        }
        
        close(pdesk);
    }

    else{
        pdesk = open("/tmp/fifo",O_RDONLY|O_NDELAY);
        int buf = 0;
        int g;
        int h=0;

        while(1){
            g = read(pdesk,&buf,sizeof(buf));
            if (g == -1){
                if (errno != EAGAIN){
                    printf("Wronmg\n");
                }
                else if(buf!=0){
                    printf("%d\n",g);
                }
            }
            if ( g==0 ){
                break;
            }
        }
        close(pdesk);
    }
    return 0;
}