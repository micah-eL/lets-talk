# lets-talk

lets-talk is a multi-threaded CLI chat utility for locally connected machines. 
It was written in C and it uses the Unix sockets API, as well as extensive use of mutex locks for deadlock prevention. 


## To run:
Step 1
```bash
make
```

Step 2
```bash
./lets-talk <local port number> <remote machine name> <remote port number>
```
Note: Computer 1's input for <remote port number> should match Computer 2's input for <local port number> (and vice versa for the Computer 2).
