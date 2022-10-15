#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define SERVER_FIFO "/tmp/myfifo"

int main (int argc, char **argv)
{
    
    int fd, fd_client, bytes_read, i;
    int counter = 0;
    char return_fifo[20][100];
    char buf [4096];

    printf("Server Online\n");


    // making fifo
    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) {
        perror ("mkfifo");
        exit (1);
    }
    // opening fifo
    if ((fd = open (SERVER_FIFO, O_RDONLY)) == -1)
        perror ("open");

     while(1)
     {

        strcpy(return_fifo[0],"\0");
        
        for(int i =0;i<20;i++)
        {
            strcpy(return_fifo[i],"");
        }

         memset (buf, '\0', sizeof (buf));

         bytes_read = read (fd, buf, sizeof (buf));
         if (bytes_read > 0) 
         {
            // string parsing
            char* token;
            char* rest = buf; 
            int i = 0;
            while ((token = strtok_r(rest, " ", &rest)) && strlen(token)!=0)
            { 
                strcpy(return_fifo[i],token);
                i++;
            }

            // mensaje de usuario nuevo entrado al chat
            if(strcasecmp(return_fifo[0],"new")==0)
            {
                printf("\nNuevo usuario con nombre: %s", return_fifo[1]);                
       
                char string_nuevo_usuario[100];

                strcpy(string_nuevo_usuario,"Nuevo usuario ");

                fd_client = open (SERVER_FIFO, O_WRONLY);

                write (fd_client, string_nuevo_usuario, strlen (string_nuevo_usuario));
                close (fd_client);
            }
            

            // enviar mensaje a usuarios
            else{
                //imprimir mensaje en servidor
                for ( int i=0;i<20;i++){
                    printf("%s ",return_fifo[i]);
                }
                
                char mensaje[500]="";
                // añadiendo mensaje
                for(int i =0;i<20;i++)
                {
                    if(strlen(return_fifo[i])!=0)
                    {
                        strcat(mensaje," ");
                        strcat(mensaje,return_fifo[i]);
                    }
                }
                //enviando mensaje
                fd_client = open (SERVER_FIFO, O_WRONLY);
                write (fd_client, mensaje, strlen (mensaje));
                close (fd_client);

            }
            
        }

     }
}