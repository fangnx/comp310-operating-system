Manual test 1 - basic commands, cd/chdir, history
=================================================
Execution: `$ ./tshell < manual_test001.txt`

Common mistakes: 
  - redirecting manual_test001.txt to stdin results in INF LOOP
    - should terminate shell on EOF
  - cd/chdir implementation does not support RELATIVE AND ABSOLUTE PATHS 
  - invalid commands crash the shell
  - history implementation does not behave similarly to bash's history

Manual test 2 - limit 
=====================
Execution: 
  - `$ gcc malloc_hog.c -o malloc_hog`
  - `$ ./tshell`
  - `> limit 10000000`
  - `> ls`
  - `> ./malloc_hog`
  - `> ls`

Common mistakes: 
  - first ls does not execute (try RLIMIT_DATA)
  - second ls does not execute (tshell crashed?)
  - malloc_hog does not trigger limit 

Manual test 3 - signal handling 
===============================
Execution: 
  - `$ ./tshell`
  - "CTRL C" -> "n"
  - `ls -l`
  - "CTRL Z"
  - `ls -l`
  - "CTRL C" -> "y"

Expected: 
  - both `ls -l` works fine
  - instructions say to ignore CTRL Z signal
    - if you did implement this, good job!
  - tshell terminates after confirming CTRL C

Note: sometimes getting a line the in signal handler
      makes the shell "mess up" for a couple of commands
      however, it should still work afterwards

Manual test 4 - fifo
====================
Execution:
  - `$ mkfifo testfifo`
  - `$ ./tshell testfifo < manual_test002.txt`

Expected:
  - non piped commands should still work 
  - all children run in the foreground
    - ie. 1 after the other in order
  - after cd/chdir, piped commands should still work (try realpath()) 
  - invalid commands do not crash the parent 