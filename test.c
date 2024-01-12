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
    
    char OrderFIFO[40];
    strcpy(OrderFIFO,argv[1]);

    OrderFIFO[strlen(OrderFIFO)-10] = '\0';
    strcat(OrderFIFO,"ORDERS");
    printf("%s\n",OrderFIFO);

    // KYS
    return 0;
}
