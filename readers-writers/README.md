# COMP 310 Assignment 2
## Question 1
Run the program with NUMBER_OF_WRITES=30 and NUMBER_OF_READS=60, we have:
```
Min reader wait time: 0.003000 ms
Max reader wait time: 2.649000 ms
Average reader wait time: 0.101555 ms

Min writer wait time: 0.003000 ms
Max writer wait time 3430.367000 ms
Average writer wait time: 114.106587 ms
```
From the result, it can be observed that there is a starvation problem. In this version of implementation of the readers-writers problem, readers are given the preference: multiple readers are allowed to read the shared resource concurrently, and the writer will not gain access to it until the last reader releases the rw_mutex semaphore. Thus, writers in this implementation are starved, and the average wait time of a writer is significantly larger than the average wait time of a reader.
## Question 2
The starvation is already observed.
Run the program with different parameters NUMBER_OF_WRITES=10 and NUMBER_OF_READS=50, we have:
```
Min reader wait time: 0.003000 ms
Max reader wait time: 1.787000 ms
Average reader wait time: 0.092604 ms

Min writer wait time: 0.003000 ms
Max writer wait time 2710.339000 ms
Average writer wait time: 270.737220 ms
```
A similar starvation problem can still be seen.
## Question 3
To solve the writer starvation problem, we created a modified version of the original implementation. In this version, a third semaphore queue_mutex is introduced to ensure fairness for both readers and writers. It is called in the beginning of the reader and writer function and released after the read/write operations. This way, readers and writers are treated equally and put in a FIFO queue, which prevents the starvation.
## Question 4
Run the program (modified implementation) with NUMBER_OF_WRITES=30 and NUMBER_OF_READS=60:
```
Min reader wait time: 0.003000 ms
Max reader wait time: 372.729000 ms
Average reader wait time: 3.930604 ms

Min writer wait time: 0.047000 ms
Max writer wait time 336.004000 ms
Average writer wait time: 6.748787 ms
```
It can be observed that the average wait time of a writer is significantly reduced. Without the starvation, the overall performance of the implementation is improved.