#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Argomenti insufficenti\n");
        exit(0);
    }

    int p1p2[2], pid;
    char stringa[50];

    pipe(p1p2);

    pid = fork();
    if(pid == 0)
    {
        close(p1p2[0]);

        while(1)
        {
            printf("Inserisci un nome:\n");
            scanf("%s", stringa);

            if(strcmp(stringa, "esci") == 0)
            {
                close(p1p2[1]);
                printf("Terminazione del lavoro\n");
                exit(0);
            }

            strcat(stringa, "\n");
            write(p1p2[1], stringa, strlen(stringa));
        }
    }
    
    close(p1p2[1]);
    pid = fork();

    if(pid == 0)
    {
        close(0);
        dup(p1p2[0]);
        close(p1p2[0]);

        close(1);
        open(argv[1], O_WRONLY|O_CREAT, 0777);

        execl("/usr/bin/sort", "sort", NULL);
        return -1;
    }
    close(p1p2[0]);
    wait(&pid);

    return 0;
}