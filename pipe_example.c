#include <stdio.h>
#include <unistd.h>

int main() {
  int pid, fd[2];
  char *catArgs[] = {"cat", "hello_world.c", NULL};
  char *grepArgs[] = {"grep", "#inclu", NULL};

  pipe(fd);
  pid = fork();

  if (pid == 0) {
    dup2(fd[0], fileno(stdin));
    close(fd[1]);
    execvp(grepArgs[0], grepArgs);
  } else {
    dup2(fd[1], fileno(stdout));
    close(fd[0]);
    execvp(catArgs[0], catArgs);
  }

  return 0;
}