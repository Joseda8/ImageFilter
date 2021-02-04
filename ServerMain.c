#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

#include "Configurator.c"
#include "misc.c"
#include "Logger.c"

#include <strings.h>

     
int main(int argc , char *argv[])   
{   

    createFiles();

    srand((unsigned int)(time(NULL))); // Sets seed.

    struct Conf config = get_conf(); 

    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    int running = 1;

    create_log_file(config.dir_log);
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = inet_addr("192.168.1.6"); 
    address.sin_port = htons(config.port); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    write_log("ImageServer Started");

    while(running) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } else {
            write_log("New conection established");
        }


        //Read Picture Size
        printf("Reading Picture Size\n");
        int size;
        read(new_socket, &size, sizeof(int));

        write_log("New Image Incoming");

        //Read Picture Byte Array
        printf("Reading Picture Byte Array\n");
        char p_array[size];
        read(new_socket, p_array, size);

        //Convert it Back into Picture
        printf("Converting Byte Array to Picture\n");
        FILE *image;
        image = fopen("img.jpg", "w");
        fwrite(p_array, 1, sizeof(p_array), image);
        fclose(image);

        write_log("Image Received Successfully");

        make_img_conversion();
        filters();

        close(new_socket);
    }

   return 0;   
}   
