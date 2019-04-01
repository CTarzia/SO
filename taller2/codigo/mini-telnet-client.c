#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

int main(int argc, char **argv)
{
	char *buf = NULL;
	struct sockaddr_in name;
	ssize_t w, len, r;
	int s;
	len = 0;

	if (argc != 2) {
		fprintf(stderr, "uso: %s ip\n", argv[0]);
		exit(1);
	}

	//Creo el socket de cliente de tipo internet (SOCK_STREAM) para TCP (SOCK_STREAM)
	if ((s = socket(SOCK_STREAM, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	//Convierto un string de una dirección IP en la estructura in_addr
	if (!inet_aton(argv[1], &name.sin_addr)) {
		fprintf(stderr, "%s: ip desconocida\n", argv[1]);
		exit(1);
	}

	//Tipo internet
	name.sin_family = AF_INET;

	//Puerto al que me conecto
	name.sin_port = htons(PORT);

	//Me conecto al socket del servidor que está en name
	if (connect(s, (struct sockaddr *)&name, sizeof(name)) == -1) {
		perror("connect");
		exit(1);
	}

	for (;;) {
		printf("[%s]> ", argv[1]);

		//Tomo el comando por stdin
		if ((w = getline(&buf, &len, stdin)) == -1) {
			if (!feof(stdin))
				perror("getline");
			break;
		}

		//Verifico que no sea la cadena final
		if (strncmp(buf, ENDMSG, w) == 0)
			break;

		//Lo envio por el socket
		if (send(s, buf, w, 0) == -1) {
			perror("send");
			exit(1);
		}
	}

	free(buf);
	close(s);

	return 0;
}
