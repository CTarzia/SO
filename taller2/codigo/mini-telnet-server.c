#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

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

	//TODO: Hacer el bind a la dirección de local

	//TODO: Completar conexión del socket para TCP (¿qué otros pasos hacen falta)


	while(1) {
		memset(&buf, 0, MSGLEN+1);

		//TODO: Recibir mensaje en buf


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
	return 0;
}
