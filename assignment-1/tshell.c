/**
 * COMP 310 (Fall 2019) - Assignment 1
 *
 * @author Naxin Fang
 * @github https://github.com/fangnx
 * @created 2019-09-21 15:42:16
 * @last-modified 2019-09-30 13:31:37
 */

#include <fcntl.h>
#include <signal.h>
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

#define PROMPT "\e[96m(naxin.fang)\e[0m \e[92m->\e[0m "
#define MAX_LINE_LENGTH 120
#define MAX_HISTORY_AMOUNT 100
#define FIFO_PATH "/tmp/tshell_fifo"

struct hist {
  char line[MAX_LINE_LENGTH];
};
struct hist HISTORY[MAX_HISTORY_AMOUNT];
int HIST_COUNT = 0;

/**
 * Read the raw stdin by the user.
 */
int read_line(char line[]) { return read(0, line, MAX_LINE_LENGTH); }

/**
 * Tokenize the input line by splitting it by space.
 *
 * E.g, "ls -l" -> {"ls", "-l", NULL}.
 */
void parse_line(char line[], int length, char *commands[]) {
  const char delim[] = " \t\n";
  char *token;
  int i;

  if (length == 0) {  // Exit if input is empty.
    exit(0);
  }
  token = strtok(line, delim);
  i = 0;
  while (token != NULL) {
    commands[i] = token;
    token = strtok(NULL, delim);
    i++;
  }
}

/**
 * Record the input line to history.
 */
void record_history(char args[]) {
  strcpy(HISTORY[HIST_COUNT % MAX_HISTORY_AMOUNT].line, args);
  HIST_COUNT++;
}

/**
 * Implement "chdir" internal command.
 */
void change_dir(char *args[]) { chdir(args[1]); }

/**
 * Implement "history" internal command.
 */
void print_history() {
  int i =
      HIST_COUNT <= MAX_HISTORY_AMOUNT ? 0 : HIST_COUNT % MAX_HISTORY_AMOUNT;
  int end_i = min(HIST_COUNT, i + MAX_HISTORY_AMOUNT);
  while (i < end_i) {
    printf("%3d %s\n", i + 1, HISTORY[i % MAX_HISTORY_AMOUNT].line);
    i++;
  }
}

/**
 * Implement "limit" internal command.
 */
void set_limit(char *args[]) {
  struct rlimit old_lim, new_lim;
  getrlimit(RLIMIT_DATA, &old_lim);

  new_lim.rlim_cur = atoi(args[1]);
  new_lim.rlim_max = old_lim.rlim_max;

  if (setrlimit(RLIMIT_NOFILE, &new_lim) == -1) {
    printf("Error: an error occurred during setting RLIMIT.");
  }
  printf("New limits have been set.\nSoft limit: %llu\nHard limit: %llu\n",
         new_lim.rlim_cur, new_lim.rlim_max);
}

/**
 * Execute a shell command. Display error message if the command is invalid.
 */
void execute_command(char *args[]) {
  if (execvp(args[0], args) < 0) {
    printf("tshell: command not found: %s\n", args[0]);
    exit(1);
  }
}

/**
 * Auxiliary function to check if the tokenized input line contains the pipe
 * character '|'.
 *
 * If true, return the index of '|'. Else, return -1.
 */
int find_pipe(char *args[]) {
  int i;
  i = 0;
  while (args[i] != NULL) {
    if (strcmp(args[i], "|") == 0) {
      return i;
    }
    i++;
  }
  return -1;
}

/**
 * Implemented FIFO pipe.
 */
void run_pipe(char *args[], int pipe_index) {
  mkfifo(FIFO_PATH, 0777);
  char *abs_path = realpath(FIFO_PATH, NULL);
  args[pipe_index] = NULL;
  pid_t pid_in, pid_out;
  int fd[2], fdp;
  pipe(fd);
  // Forking a child process to handle pipe_in.
  pid_in = fork();
  if (pid_in < 0) {
    exit(1);
  } else if (pid_in == 0) {  // Child process.
    // Forking a granchild process to handle pipe_out.
    pid_out = fork();
    if (pid_out < 0) {
      exit(1);
    } else if (pid_out == 0) {  // Grandchild process.
      // Implement pipe_out.
      fdp = open(abs_path, O_RDONLY);
      dup2(fdp, fileno(stdin));  // Redirect stdin to pipe.
      close(fd[1]);
      if (execvp(args[pipe_index + 1], &args[pipe_index + 1]) < 0) {
        printf("tshell: command not found: %s\n", args[0]);
        exit(1);
      }
    } else {
      // Implement pipe_in.
      fdp = open(abs_path, O_WRONLY);
      dup2(fdp, fileno(stdout));  // Redirect stdout to pipe.
      close(fd[0]);
      if (execvp(args[0], args) < 0) {
        printf("tshell: command not found: %s\n", args[0]);
        exit(1);
      }
      waitpid(pid_out, NULL, 0);
    }
  } else {  // Parent process.
    waitpid(pid_in, NULL, 0);
  }
}

/**
 * Run commands in the t_shell.
 */
void shell_system(char *args[]) {
  if (strcmp(args[0], "chdir") == 0 || strcmp(args[0], "cd") == 0) {
    change_dir(args);
  } else if (strcmp(args[0], "history") == 0) {
    print_history();
  } else if (strcmp(args[0], "limit") == 0) {
    set_limit(args);
  } else if (strcmp(args[0], "exit") == 0) {
    exit(0);
  } else {
    int pipe_index = find_pipe(args);
    if (pipe_index > -1) {
      run_pipe(args, pipe_index);
    }
    // General case.
    else {
      pid_t pid = fork();
      if (pid < 0) {
        exit(1);
      } else if (pid == 0) {  // Child process.
        execute_command(args);
      } else {                 // Parent process.
        waitpid(-1, NULL, 0);  // Wait for any child.
      }
    }
  }
}

// Handle SIGINT signal.
void handle_sigint(int sig) {
  char line[MAX_LINE_LENGTH];
  printf("\nDo you want to exit tshell (y/n)?\n");
  fgets(line, sizeof(line), stdin);
  if (line[0] == 'y') {
    exit(0);
  }
  printf(PROMPT);
  fflush(stdout);
}

// Handle STGTSTP signal.
void handle_sigtstp(int sig) { return; }

int main(int argc, char *argv[]) {
  int line_length;
  char line[MAX_LINE_LENGTH];
  char *args[MAX_LINE_LENGTH];

  signal(SIGINT, handle_sigint);
  signal(SIGTSTP, handle_sigtstp);

  while (1) {
    memset(line, 0, MAX_LINE_LENGTH);
    memset(args, 0, MAX_LINE_LENGTH);

    printf(PROMPT);
    fflush(stdout);

    line_length = read_line(line);
    if (line_length > 1) {
      parse_line(line, line_length, args);
      record_history(*args);
      shell_system(args);
    }
  }
  return 0;
}
