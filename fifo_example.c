#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *fifo_path =
      "/Users/fnx/Dropbox/Academics/COMP "
      "310/comp310-operating-system/fifo_example_fifo";
  mkfifo(fifo_path, 0777);

  int pid = fork();
  int BUFFER_SIZE = 256;
  int fd;
  char buffer[BUFFER_SIZE];

  if (pid == 0) {
    fd = open(fifo_path, O_WRONLY);
    printf("Enter something: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    write(fd, buffer, strlen(buffer));
    close(fd);
    printf("Done!\n");
  } else {
    fd = open(fifo_path, O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("You have typed: %s\n", buffer);
    close(fd);
  }

  return 0;
}