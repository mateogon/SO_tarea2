
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <pthread.h> 

#define SERVER_FIFO "/tmp/myfifo"

int fd, fd_server, bytes_read;
char buf1 [512], buf2 [1024];
char newstring[128];
char my_fifo_name [128];
int nuevo_cliente = 0; // 0 = nuevo

void *enviar(void *pthread)
{
    while(1)
    {
        if(nuevo_cliente == 0)
        {
            strcat (newstring, my_fifo_name);
            fd = open (SERVER_FIFO, O_WRONLY);

            if (write (fd, newstring, strlen (newstring)) != strlen (newstring)) {
                perror ("write");
                break;
            }   

            close (fd);
            nuevo_cliente = 1;
        }

        else if(nuevo_cliente ==1)
        {
            printf("\n");
            if (fgets(buf1, sizeof (buf1), stdin) == NULL)
                break;
            strcpy (buf2, my_fifo_name);
            strcat (buf2, " ");
            strcat (buf2, buf1);

            fd= open (SERVER_FIFO, O_WRONLY);
            write(fd_server, buf2, strlen (buf2));
            close (fd_server);
        }
    }
    pthread_exit(NULL);
    return NULL;
}

void *recibir(void *pthread)
{ 
    while(1)
    {
        char read_data[20][100];
        for(int i =0;i<20;i++)
        {
            strcpy(read_data[i],"");
        }
        
        fd = open (SERVER_FIFO, O_RDONLY);

        memset (buf2, '\0', sizeof (buf2));

        if ((bytes_read = read (fd, buf2, sizeof (buf2))) == -1)
            perror ("read");

        if (bytes_read > 0) 
        {
            char* token;
            char* rest = buf2; 
            int i = 0;
            while ((token = strtok_r(rest, " ", &rest)))
            { 
                strcpy(read_data[i],token);
                i++;
            }
            
            // printf("%s",buf2);
            for(int i = 0;i<20;i++)
            {
                printf("%s ",read_data[i]);
            }
        }

        close (fd);
    }
    pthread_exit(NULL);
    return NULL;

}


int main (int argc, char **argv)
{
    
    // nombre del cliente
    sprintf (my_fifo_name, "user%ld", (long) getpid ());

    printf("Bienvenido al chat ! ");
    strcpy(newstring,"new ");

    // creando threads
    pthread_t thread_enviar,thread_recibir;

    pthread_create(&thread_enviar, NULL, recibir, (void *)&thread_enviar);

    pthread_create(&thread_recibir, NULL, enviar, (void *)&thread_recibir);

    pthread_join(thread_enviar, NULL);

    pthread_join(thread_recibir, NULL);
    return 0;
}
