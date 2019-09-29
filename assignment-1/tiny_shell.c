/**
 * COMP 310 / ECSE 427 (Fall 2019) - Assignment 1
 *
 * @author Naxin Fang
 * @github https://github.com/fangnx
 * @created 2019-09-21 15:42:16
 * @last-modified 2019-09-27 23:42:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
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
 * Parse the raw input line to shell commands.
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
    commands[i] = token;
    token = strtok(NULL, delim);
    i++;
  }
}

/**
 *
 */
void my_system(char line[], char *args[]) {
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

  else if (strcmp(args[0], "exit") == 0) {
    exit(0);
  } else {
    pid_t pid = fork();
    if (pid < 0) {
      exit(1);
    }
    // Child process.
    else if (pid == 0) {
      if (execvp(args[0], args) < 0) {
        exit(1);
      }
    }
    // Parent process.
    else {
      waitpid(-1, NULL, 0);  // Wait for any child.
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
