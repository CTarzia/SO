#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
  int foo = 0;
  int pid = fork();
  if (pid == 0) {
    printf("%d: Hello world\n", getpid());
    foo = 1;
  } else {
    printf("%d: %d creado\n", getpid(), pid);
    int s;
    (void)waitpid(pid, &s, 0);
    printf("%d: %d finalizado(%d)\n", getpid(), pid, s);
  }
  printf("%d: foo(%p) = %d\n", getpid(), &foo, foo);
}
