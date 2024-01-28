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
    int pdesk = open(1234,O_RDONLY);
    if (pdesk == -1){
        perror("Opening FIFO\n");
        return 1;
    }

    while(1){
        int recived[4];
        if(read(pdesk,recived,sizeof(recived))<=0){
            break;
        }
        printf("Zamówienie nr:%d z %d A, %d B, %d C\n",recived[0],recived[1],recived[2],recived[3]);
    }


    close(pdesk);


    return 0;
}
