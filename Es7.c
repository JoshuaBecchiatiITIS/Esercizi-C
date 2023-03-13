#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Numero argomenti sbagliato\n");
        exit(1);
    }

    int p1p2[2], p2p3[2], p3p0[2], pid;

    pipe(p1p2);
    pid = fork();

    //P1 Cat
    if(pid == 0)
    {
        close(p1p2[READ]);
        close(WRITE);
        dup(p1p2[WRITE]);
        close(p1p2[WRITE]);

        execl("/bin/traceroute", "traceroute", argv[1], NULL);
        return -1;
    }

    pipe(p2p3);
    pid = fork();
    //P2
    if (pid == 0)
    {
        close(p1p2[WRITE]);
        close(READ);
        dup(p1p2[READ]);
        close(p1p2[READ]);

        close(p2p3[READ]);
        close(WRITE);
        dup(p2p3[WRITE]);
        close(p2p3[WRITE]);

        execl("/bin/awk", "awk", "{print $4}", NULL);
        return -1;
    }

    pipe(p3p0);
    pid = fork();
    //P3
    if (pid == 0)
    {
        close(p1p2[READ]);
        close(p1p2[WRITE]);

        close(p2p3[WRITE]);
        close(READ);
        dup(p2p3[READ]);
        close(p2p3[READ]);

        close(p3p0[READ]);
        close(WRITE);
        dup(p3p0[WRITE]);
        close(p3p0[WRITE]);

        execl("/bin/tail", "tail", "-n", "+2",NULL);
        return -1;
    }
    close(p1p2[READ]);
    close(p1p2[WRITE]);

    close(p2p3[READ]);
    close(p2p3[WRITE]);

    close(p3p0[WRITE]);
    
    char buffer[1], strtempi[100], *ptr;
    double totale;
    while (read(p3p0[READ], buffer, sizeof(buffer)) > 0)
    {
        strncat(strtempi, &buffer[0], sizeof(buffer[0]));
        if (buffer[0] == '\n')
        {
            printf("Ricevuto tempo d'attesa: %s", strtempi);
            totale = totale + strtod(strtempi, &ptr);
            strtempi[0] = '\0';
        }
    }

    close(p3p0[WRITE]);
    printf("\nIl totale dei tempi di attesa è: %.2lf\n", totale);
    return 0;
}