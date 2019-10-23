/**
 * A2Q3.c
 *
 * @author nxxinf
 * @github https://github.com/fangnx
 * @created 2019-10-22 19:14:36
 * @last-modified 2019-10-22 19:15:44
 */

#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

int NUMBER_OF_WRITES, NUMBER_OF_READS;
const int NUMBER_OF_WTHREADS = 10;
const int NUMBER_OF_RTHREADS = 500;
int shared_int, read_count;
double min_write_time, max_write_time, total_write_time, min_read_time,
    max_read_time, total_read_time;
static sem_t mutex, rw_mutex, queue_mutex;

void *reader(void *arg) {
  int temp, i, sleep_time;
  double time_count;
  i = 0;
  while (i++ < NUMBER_OF_READS) {
    clock_t t0 = clock();
    sem_wait(&queue_mutex);
    sem_wait(&mutex);
    read_count++;
    if (read_count == 1) {
      sem_wait(&rw_mutex);
    }
    sem_post(&mutex);
    sem_post(&queue_mutex);
    clock_t t1 = clock();
    time_count = (double)(t1 - t0) / CLOCKS_PER_SEC * 1000;
    min_read_time = MIN(min_read_time, time_count);
    max_read_time = MAX(max_read_time, time_count);
    total_read_time += time_count;

    sleep_time = rand() % 101 * 1000;
    usleep(sleep_time);
    temp = shared_int;

    sem_wait(&mutex);
    read_count--;
    if (read_count == 0) {
      sem_post(&rw_mutex);
    }
    sem_post(&mutex);
  }
}

void *writer(void *arg) {
  int temp, i, sleep_time;
  double time_count;
  i = 0;
  while (i++ < NUMBER_OF_WRITES) {
    clock_t t0 = clock();
    sem_wait(&queue_mutex);
    sem_wait(&rw_mutex);
    clock_t t1 = clock();
    time_count = (double)(t1 - t0) / CLOCKS_PER_SEC * 1000;
    min_write_time = MIN(min_write_time, time_count);
    max_write_time = MAX(max_write_time, time_count);
    total_write_time += time_count;

    sleep_time = rand() % 101 * 1000;
    usleep(sleep_time);
    temp += shared_int;
    temp += 10;
    shared_int = temp;

    sem_post(&rw_mutex);
    sem_post(&queue_mutex);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Insufficient arguments: NUM_OF_WRITES and NUM_OF_READS.\n");
    exit(1);
  }
  NUMBER_OF_WRITES = atoi(argv[1]);
  NUMBER_OF_READS = atoi(argv[2]);
  read_count = 0;

  if (sem_init(&mutex, 0, 1) == -1 || sem_init(&rw_mutex, 0, 1) == -1 ||
      sem_init(&queue_mutex, 0, 1) == -1) {
    printf("Error occurred during initializing semaphors.\n");
    exit(1);
  }

  pthread_t rthreads[NUMBER_OF_RTHREADS], wthreads[NUMBER_OF_WTHREADS];
  int i, j;
  max_write_time = 0;
  min_write_time = DBL_MAX;
  total_write_time = 0;
  max_read_time = 0;
  min_read_time = DBL_MAX;
  total_read_time = 0;
  srand(time(NULL));
  for (i = 0; i < NUMBER_OF_RTHREADS; i++) {
    pthread_create(&rthreads[i], NULL, reader, &shared_int);
  }
  for (j = 0; j < NUMBER_OF_WTHREADS; j++) {
    pthread_create(&wthreads[j], NULL, writer, &shared_int);
  }
  for (i = 0; i < NUMBER_OF_RTHREADS; i++) {
    pthread_join(rthreads[i], NULL);
  }
  for (j = 0; j < NUMBER_OF_WTHREADS; j++) {
    pthread_join(wthreads[j], NULL);
  }

  long total_read_count, total_write_count;
  total_read_count = NUMBER_OF_READS * NUMBER_OF_RTHREADS;
  total_write_count = NUMBER_OF_WRITES * NUMBER_OF_WTHREADS;
  printf("Min reader wait time: %f ms\n", min_read_time);
  printf("Max reader wait time: %f ms\n", max_read_time);
  printf("Average reader wait time: %f ms\n\n",
         total_read_time / total_read_count);
  printf("Min writer wait time: %f ms\n", min_write_time);
  printf("Max writer wait time %f ms\n", max_write_time);
  printf("Average writer wait time: %f ms\n",
         total_write_time / total_write_count);
  return 0;
}
