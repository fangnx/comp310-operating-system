/**
 * COMP 310 / ECSE 427 (Fall 2019) - Assignment 1
 *
 * @author Naxin Fang
 * @github https://github.com/fangnx
 * @created 2019-09-21 15:42:16
 * @last-modified 2019-09-29 18:36:49
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#define PROMPT "(naxin.fang) -> "
#define MAX_LINE_LENGTH 120
#define MAX_HISTORY_AMOUNT 10

struct hist {
  char line[MAX_LINE_LENGTH];
};
struct hist HISTORY[MAX_HISTORY_AMOUNT];
int HIST_COUNT = 0;

/**
 */
int get_a_line(char line[]) { return read(0, line, MAX_LINE_LENGTH); }

/**
 * Tokenize the raw input, delimited by space.
 */
void parse_commands(char line[], int length, char *commands[]) {
  const char delim[] = " \t\n";
  char *token;
  int i;

  if (length == 0) {  // Exit if input is empty.
    exit(0);
  }
  token = strtok(line, delim);
  i = 0;
  while (token != NULL) {
    // printf("%s\n", token);
    // fflush(stdout);
    commands[i] = token;
    token = strtok(NULL, delim);
    i++;
  }
}

int find_pipe(char *args[]) {
  int i;
  i = 0;
  while (args[i] != NULL) {
    // printf("%s\n", args[i]);
    // fflush(stdout);
    if (strcmp(args[i], "|") == 0) {
      return i;
    }
    i++;
  }
  return -1;
}

/**
 *
 */
void my_system(char line[], char *args[]) {
  char *fifo_path =
      "/Users/fnx/Dropbox/Academics/COMP "
      "310/comp310-operating-system/assignment-1/fifo";
  mkfifo(fifo_path, 0777);

  // Update history.
  strcpy(HISTORY[HIST_COUNT % MAX_HISTORY_AMOUNT].line, line);
  HIST_COUNT++;
  // Implement "chdir" internal command.
  if (strcmp(args[0], "chdir") == 0 || strcmp(args[0], "cd") == 0) {
    chdir(args[1]);
  }
  // Implement "history" internal command.
  else if (strcmp(args[0], "history") == 0) {
    int i =
        HIST_COUNT <= MAX_HISTORY_AMOUNT ? 0 : HIST_COUNT % MAX_HISTORY_AMOUNT;
    int end_i = min(HIST_COUNT, i + MAX_HISTORY_AMOUNT);
    while (i < end_i) {
      printf("%3d %s\n", i + 1, HISTORY[i % MAX_HISTORY_AMOUNT].line);
      i++;
    }
  }
  // Implement "limit" internal command.
  else if (strcmp(args[0], "limit") == 0) {
    struct rlimit old_lim, new_lim;
    getrlimit(RLIMIT_DATA, &old_lim);

    new_lim.rlim_cur = atoi(args[1]);
    new_lim.rlim_max = old_lim.rlim_max;

    if (setrlimit(RLIMIT_NOFILE, &new_lim) == -1) {
      printf("Error occurred during setting RLIMIT_DATA.");
    }
    printf("New limits have been set:\nsoft limit: %llu\nhard limit: %llu\n",
           new_lim.rlim_cur, new_lim.rlim_max);
  }
  //
  else if (strcmp(args[0], "exit") == 0) {
    exit(0);
  }
  // Implement FIFO pipe.
  else {
    int j = find_pipe(args);
    if (j > -1) {
      // printf("Found a pipe at %d!\n", j);
      args[j] = NULL;
      int BUFFER_SIZE = 256;
      char buffer[BUFFER_SIZE];
      char *abs_path = realpath(fifo_path, NULL);

      int pid_in, pid_out, fd[2];
      pipe(fd);
      // Forking a child process to handle pipe_in.
      pid_in = fork();
      if (pid_in < 0) {
        exit(1);
      } else if (pid_in == 0) {  // Child process (pipe_in).
        // Forking a granchild process to handle pipe_out.
        pid_out = fork();
        if (pid_out < 0) {
          exit(1);
        } else if (pid_out == 0) {  // Grandchild process (pipe_out).
          fd = open(abs_path, O_RDONLY);
          read(fd, buffer, sizeof(buffer));
          printf("In the buffer: %s\n", buffer);
          close(fd);
        } else {
          fd = open(abs_path, O_WRONLY);
          printf("Enter sth: ");
          fgets(buffer, BUFFER_SIZE, stdin);
          write(fd, buffer, strlen(buffer));
          close(fd);
        }
      } else {  // Parent process.
        close(fd[1]);
        wait(NULL);
      }
    }
    // General case.
    else {
      pid_t pid = fork();
      if (pid < 0) {
        exit(1);
      } else if (pid == 0) {  // Child process.
        if (execvp(args[0], args) < 0) {
          exit(1);
        }
      } else {                 // Parent process.
        waitpid(-1, NULL, 0);  // Wait for any child.
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int length;
  char line[MAX_LINE_LENGTH];
  char *commands[MAX_LINE_LENGTH];

  while (1) {
    memset(line, 0, MAX_LINE_LENGTH);
    memset(commands, 0, MAX_LINE_LENGTH);

    printf(PROMPT);  // Print shell line prompt.
    fflush(stdout);

    length = get_a_line(line);

    if (length > 1) {
      parse_commands(line, length, commands);
      fflush(stdout);
      my_system(line, commands);
    }
  }
  return 0;
}
