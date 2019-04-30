#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))

static int run(const char ***progs, size_t count)
{
	
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children;
	int i, status;
	int pipefd[2][2];

	for (int i = 0; i < count-1; ++i)	{ //para cada pipe
		// hacer la cosa
		pipe(pipefd[i]); 
	
	}

	if (!(children = malloc(sizeof(*children) * count))) {
		fputs("out of memory\n", stderr);
		goto end;
	}


	for (int j = 0; j < count; ++j)	{
		i = fork();
		if (i > 0){

			children[j] = i; 

		} else{

			if(j==0){

				dup2(pipefd[j][1], 1); //reemplazo escritura 1 
				close(pipefd[j][0]);
				close(pipefd[j+1][0]);
				close(pipefd[j+1][1]);

			} else if(j==count-1){

				dup2(pipefd[j-1][0],0); //reemplazo lectura
				close(pipefd[j-1][1]);
				close(pipefd[j-2][1]);
				close(pipefd[j-2][0]);

			} else {
				dup2(pipefd[j-1][0], 0); //reemplazo lectura 0
				dup2(pipefd[j][1], 1); //reemplazo escritura 1 
				close(pipefd[j-1][1]);
				close(pipefd[j][0]);//cierro los que no dup2
			}


			execvp(progs[j][0], progs[j]);
		}
	}

close(pipefd[0][1]);
close(pipefd[0][0]);
close(pipefd[1][0]);
close(pipefd[1][1]);

//dup2

	//TODO: Pensar cuantos procesos necesito
	//TODO: Pensar cuantos pipes necesito.

	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

	
	//El padre espera a que terminen todos los procesos hijos que ejecutan los programas
	for (i = 0; i < count; i++) {
		if (waitpid(children[i], &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}

end:
	free(children);

	return 0;
}

int main(int argc, char **argv)
{
	char *lscmd[] = { "ls", "-al", NULL };
	char *wccmd[] = { "wc", NULL };
	char *awkcmd[] = { "awk", "{ print $2 }", NULL };
	char **progs[] = { lscmd, wccmd, awkcmd };

	printf("status: %d\n", run(progs, NELEMS(progs)));
	fflush(stdout);
	fflush(stderr);

	return 0;
}
