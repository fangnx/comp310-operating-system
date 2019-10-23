# COMP 310 Assignment 1

### ltrace output
```
nfang5@teaching:~/assignment-1$ ltrace ./tshell testfifo < input.txt

signal(SIGINT, 0x55fbc87fc7be)                                        = 0
signal(SIGTSTP, 0x55fbc87fc875)                                       = 0
realpath(0x7ffdbd26cd16, 0x55fbc89fe040, 0, 0)                        = 0x55fbc89fe040
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = 6
fflush(0x7f7c693dda00)                                                = 0
strcpy(0x55fbc89fe140, "ls -l")                                       = 0x55fbc89fe140
strtok("ls -l", " \t\n")                                              = "ls"
strtok(nil, " \t\n")                                                  = "-l"
strtok(nil, " \t\n")                                                  = nil
strcmp("ls", "chdir")                                                 = 9
strcmp("ls", "cd")                                                    = 9
strcmp("ls", "history")                                               = 4
strcmp("ls", "limit")                                                 = 10
strcmp("ls", "exit")                                                  = 7
strcmp("ls", "|")                                                     = -16
strcmp("-l", "|")                                                     = -79
fork()                                                                = 4497
waitpid(4497, 0, 0total 93
-rw-r--r-- 1 nfang5 nogroup 17913 Oct  1  2019 a1-doc.md
-rw-r--r-- 1 nfang5 nogroup    50 Oct  1  2019 input.txt
drwxr-xr-x 2 nfang5 nogroup     7 Oct  1 15:55 manual-testing
-rwxr-xr-x 1 nfang5 nogroup 18736 Oct  1  2019 mshell
-rw-r--r-- 1 nfang5 nogroup 17504 Oct  1  2019 mshell.c
prwxr-xr-x 1 nfang5 nogroup     0 Oct  1  2019 t
prw-r--r-- 1 nfang5 nogroup     0 Oct  1 15:52 testfifo
-rwxr-xr-x 1 nfang5 nogroup 18512 Oct  1  2019 tshell
-rw-r--r-- 1 nfang5 nogroup  6722 Oct  1  2019 tshell.c
 <no return ...>
--- SIGCHLD (Child exited) ---
<... waitpid resumed> )                                               = 4497
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = 4
fflush(0x7f7c693dda00)                                                = 0
strcpy(0x55fbc89fe1b8, "pwd")                                         = 0x55fbc89fe1b8
strtok("pwd", " \t\n")                                                = "pwd"
strtok(nil, " \t\n")                                                  = nil
strcmp("pwd", "chdir")                                                = 13
strcmp("pwd", "cd")                                                   = 13
strcmp("pwd", "history")                                              = 8
strcmp("pwd", "limit")                                                = 4
strcmp("pwd", "exit")                                                 = 11
strcmp("pwd", "|")                                                    = -12
fork()                                                                = 4498
waitpid(4498, 0, 0/home/2018/nfang5/assignment-1
 <no return ...>
--- SIGCHLD (Child exited) ---
<... waitpid resumed> )                                               = 4498
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = 20
fflush(0x7f7c693dda00)                                                = 0
strcpy(0x55fbc89fe230, "echo hello | grep h")                         = 0x55fbc89fe230
strtok("echo hello | grep h", " \t\n")                                = "echo"
strtok(nil, " \t\n")                                                  = "hello"
strtok(nil, " \t\n")                                                  = "|"
strtok(nil, " \t\n")                                                  = "grep"
strtok(nil, " \t\n")                                                  = "h"
strtok(nil, " \t\n")                                                  = nil
strcmp("echo", "chdir")                                               = 2
strcmp("echo", "cd")                                                  = 2
strcmp("echo", "history")                                             = -3
strcmp("echo", "limit")                                               = -7
strcmp("echo", "exit")                                                = -21
strcmp("echo", "|")                                                   = -23
strcmp("hello", "|")                                                  = -20
strcmp("|", "|")                                                      = 0
mkfifo("/home/2018/nfang5/assignment-1/t"..., 0777)                   = -1
pipe(0x7ffdbd26c110)                                                  = 0
fork()                                                                = 4499
close(4)                                                              = 0
fdopen(3, 0x55fbc87fcb4a, 0, 0x7f7c691028d4)                          = 0x55fbc9200680
__isoc99_fscanf(0x55fbc9200680, 0x55fbc87fcb47, 0x7ffdbd26c0f0, 0x55fbc9200770 <no return ...>
--- SIGCHLD (Child exited) ---
hello
<... __isoc99_fscanf resumed> )                                       = 1
waitpid(4500, 0, 0)                                                   = -1
waitpid(4499, 0, 0)                                                   = 4499
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = 5
fflush(0x7f7c693dda00)                                                = 0
strcpy(0x55fbc89fe2a8, "cd .")                                        = 0x55fbc89fe2a8
strtok("cd .", " \t\n")                                               = "cd"
strtok(nil, " \t\n")                                                  = "."
strtok(nil, " \t\n")                                                  = nil
strcmp("cd", "chdir")                                                 = -4
strcmp("cd", "cd")                                                    = 0
chdir(".")                                                            = 0
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = 8
fflush(0x7f7c693dda00)                                                = 0
strcpy(0x55fbc89fe320, "ls | wc")                                     = 0x55fbc89fe320
strtok("ls | wc", " \t\n")                                            = "ls"
strtok(nil, " \t\n")                                                  = "|"
strtok(nil, " \t\n")                                                  = "wc"
strtok(nil, " \t\n")                                                  = nil
strcmp("ls", "chdir")                                                 = 9
strcmp("ls", "cd")                                                    = 9
strcmp("ls", "history")                                               = 4
strcmp("ls", "limit")                                                 = 10
strcmp("ls", "exit")                                                  = 7
strcmp("ls", "|")                                                     = -16
strcmp("|", "|")                                                      = 0
mkfifo("/home/2018/nfang5/assignment-1/t"..., 0777)                   = -1
pipe(0x7ffdbd26c110)                                                  = 0
fork()                                                                = 4505
close(5)                                                              = 0
fdopen(4, 0x55fbc87fcb4a, 0, 0x7f7c691028d4)                          = 0x55fbc92018c0
__isoc99_fscanf(0x55fbc92018c0, 0x55fbc87fcb47, 0x7ffdbd26c0f0, 0x55fbc92019b0      9       9      78
 <no return ...>
--- SIGCHLD (Child exited) ---
<... __isoc99_fscanf resumed> )                                       = 1
waitpid(4506, 0, 0)                                                   = -1
waitpid(4505, 0, 0)                                                   = 4505
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = 7
fflush(0x7f7c693dda00)                                                = 0
strcpy(0x55fbc89fe398, "history")                                     = 0x55fbc89fe398
strtok("history", " \t\n")                                            = "history"
strtok(nil, " \t\n")                                                  = nil
strcmp("history", "chdir")                                            = 5
strcmp("history", "cd")                                               = 5
strcmp("history", "history")                                          = 0
printf("%3d %s\n", 1, "ls -l"  1 ls -l
)                                        = 10
printf("%3d %s\n", 2, "pwd"  2 pwd
)                                          = 8
printf("%3d %s\n", 3, "echo hello | grep h"  3 echo hello | grep h
)                          = 24
printf("%3d %s\n", 4, "cd ."  4 cd .
)                                         = 9
printf("%3d %s\n", 5, "ls | wc"  5 ls | wc
)                                      = 12
printf("%3d %s\n", 6, "history"  6 history
)                                      = 12
memset(0x7ffdbd26c540, '\0', 120)                                     = 0x7ffdbd26c540
memset(0x7ffdbd26c180, '\0', 120)                                     = 0x7ffdbd26c180
printf("\033[96m(naxin.fang)\033[0m \033[92m->\033[0"...)             = 34
fflush(0x7f7c693de760(naxin.fang) -> )                                                = 0
__sigsetjmp(0x55fbc89fe060, 1, 0, 0x7f7c69102154)                     = 0
getline(0x7ffdbd26c138, 0x7ffdbd26c140, 0x7f7c693dda00, 0x7ffdbd26c140) = -1
fflush(0x7f7c693dda00)                                                = 0
exit(0 <no return ...>
+++ exited (status 0) +++
```

### strace output
```
nfang5@teaching:~/assignment-1$ strace ./tshell testfifo < input.txt

execve("./tshell", ["./tshell", "testfifo"], 0x7ffed730d1d8 /* 22 vars */) = 0
brk(NULL)                               = 0x562c59bb6000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=303688, ...}) = 0
mmap(NULL, 303688, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fcc828d3000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260\34\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=2030544, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fcc828d1000
mmap(NULL, 4131552, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fcc82306000
mprotect(0x7fcc824ed000, 2097152, PROT_NONE) = 0
mmap(0x7fcc826ed000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7fcc826ed000
mmap(0x7fcc826f3000, 15072, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fcc826f3000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7fcc828d2500) = 0
mprotect(0x7fcc826ed000, 16384, PROT_READ) = 0
mprotect(0x562c5926a000, 4096, PROT_READ) = 0
mprotect(0x7fcc8291e000, 4096, PROT_READ) = 0
munmap(0x7fcc828d3000, 303688)          = 0
rt_sigaction(SIGINT, {sa_handler=0x562c590697be, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7fcc82344f20}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTSTP, {sa_handler=0x562c59069875, sa_mask=[TSTP], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7fcc82344f20}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
getcwd("/home/2018/nfang5/assignment-1", 4096) = 31
lstat("/home/2018/nfang5/assignment-1/testfifo", {st_mode=S_IFIFO|0644, st_size=0, ...}) = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 140), ...}) = 0
brk(NULL)                               = 0x562c59bb6000
brk(0x562c59bd7000)                     = 0x562c59bd7000
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
fstat(0, {st_mode=S_IFREG|0644, st_size=50, ...}) = 0
read(0, "ls -l\npwd\necho hello | grep h\ncd"..., 8192) = 50
lseek(0, -44, SEEK_CUR)                 = 6
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fcc828d27d0) = 7313
wait4(7313, total 93
-rw-r--r-- 1 nfang5 nogroup 17913 Oct  1  2019 a1-doc.md
-rw-r--r-- 1 nfang5 nogroup    50 Oct  1  2019 input.txt
drwxr-xr-x 2 nfang5 nogroup     7 Oct  1 15:55 manual-testing
-rwxr-xr-x 1 nfang5 nogroup 18736 Oct  1  2019 mshell
-rw-r--r-- 1 nfang5 nogroup 17504 Oct  1  2019 mshell.c
prwxr-xr-x 1 nfang5 nogroup     0 Oct  1  2019 t
prw-r--r-- 1 nfang5 nogroup     0 Oct  1 15:52 testfifo
-rwxr-xr-x 1 nfang5 nogroup 18512 Oct  1  2019 tshell
-rw-r--r-- 1 nfang5 nogroup  6722 Oct  1  2019 tshell.c
NULL, 0, NULL)              = 7313
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=7313, si_uid=26636, si_status=0, si_utime=0, si_stime=0} ---
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
read(0, "pwd\necho hello | grep h\ncd .\nls "..., 8192) = 44
lseek(0, -40, SEEK_CUR)                 = 10
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fcc828d27d0) = 7314
wait4(7314, /home/2018/nfang5/assignment-1
NULL, 0, NULL)              = 7314
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=7314, si_uid=26636, si_status=0, si_utime=0, si_stime=0} ---
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
read(0, "echo hello | grep h\ncd .\nls | wc"..., 8192) = 40
lseek(0, -20, SEEK_CUR)                 = 30
mknod("/home/2018/nfang5/assignment-1/t", S_IFIFO|0777) = -1 EEXIST (File exists)
pipe([3, 4])                            = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fcc828d27d0) = 7315
close(4)                                = 0
fcntl(3, F_GETFL)                       = 0 (flags O_RDONLY)
fstat(3, {st_mode=S_IFIFO|0600, st_size=0, ...}) = 0
read(3, "7316", 4096)                   = 4
read(3, 0x562c59bb88b0, 4096)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=7315, si_uid=26636, si_status=0, si_utime=0, si_stime=0} ---
read(3, hello
"", 4096)                       = 0
wait4(7316, NULL, 0, NULL)              = -1 ECHILD (No child processes)
wait4(7315, NULL, 0, NULL)              = 7315
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
read(0, "cd .\nls | wc\nhistory", 8192) = 20
lseek(0, -15, SEEK_CUR)                 = 35
chdir(".")                              = 0
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
read(0, "ls | wc\nhistory", 8192)       = 15
lseek(0, -7, SEEK_CUR)                  = 43
mknod("/home/2018/nfang5/assignment-1/t", S_IFIFO|0777) = -1 EEXIST (File exists)
pipe([4, 5])                            = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7fcc828d27d0) = 7317
close(5)                                = 0
fcntl(4, F_GETFL)                       = 0 (flags O_RDONLY)
fstat(4, {st_mode=S_IFIFO|0600, st_size=0, ...}) = 0
read(4, "7318", 4096)                   = 4
read(4,       9       9      78
0x562c59bb9af0, 4096)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=7317, si_uid=26636, si_status=0, si_utime=0, si_stime=0} ---
read(4, "", 4096)                       = 0
wait4(7318, NULL, 0, NULL)              = -1 ECHILD (No child processes)
wait4(7317, NULL, 0, NULL)              = 7317
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
read(0, "history", 8192)                = 7
read(0, "", 8192)                       = 0
write(1, "  1 ls -l\n", 10  1 ls -l
)             = 10
write(1, "  2 pwd\n", 8  2 pwd
)                = 8
write(1, "  3 echo hello | grep h\n", 24  3 echo hello | grep h
) = 24
write(1, "  4 cd .\n", 9  4 cd .
)               = 9
write(1, "  5 ls | wc\n", 12  5 ls | wc
)           = 12
write(1, "  6 history\n", 12  6 history
)           = 12
write(1, "\33[96m(naxin.fang)\33[0m \33[92m->\33[0"..., 34(naxin.fang) -> ) = 34
rt_sigprocmask(SIG_BLOCK, NULL, [], 8)  = 0
read(0, "", 8192)                       = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```
### Explanation 
The internal commands (e.g., history, limit) that use the system calls are called in the main process. For external commands (e.g., ls, pwd), the tiny shell would fork a child process, in which the corresponding system calls are invoked.