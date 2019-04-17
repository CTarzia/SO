#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

#define cant_pendientes 1;

int main(int argc, char **argv)
{
	char buf[MSGLEN + 1];
	struct sockaddr_in local, remote;

	//Ejemplo de dirección de internet (AF_INET) para el servidor donde indica que va a
	//a recibir paquetes de cualquier IP (INADDR_ANY) en el puerto PORT.
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);


	//TODO: Crear el socket
	//int socket(int domain, int type, int protocol);
	int s = socket(AF_INET, SOCK_STREAM, 0);

	//TODO: Hacer el bind a la dirección de local
	//int bind(int s, sockaddr* a, socklen_t len);
	bind(s, (sockaddr_in*) local, sizeof(local));

	//TODO: Completar conexión del socket para TCP (¿qué otros pasos hacen falta)
	//int listen(int s, int backlog);
	int listen(s, cant_pendientes);

	//int accept(int s, sockaddr* a, socklen_t* len);
	int nuevo_s = accept(s, (sockaddr_in*) local, sizeof(local));

	while(1) {
		memset(&buf, 0, MSGLEN+1);

		//TODO: Recibir mensaje en buf
		//ssize_t recv(int s, void *buf, size_t len, int flags);
		recv(nuevo_s, *buf, sizeof(buf), 0);


		//Imprimir comando
		printf("comando: %s", buf);

		if (strncmp(buf, ENDMSG, sizeof(ENDMSG)) == 0)
			break;
		//Ejectuar comando
		if(system(buf) == -1){
			perror("error ejecución comando");
			break;
		}
	}

    //TODO: Cerrar el socket
    close(nuevo_s);
	return 0;
}
