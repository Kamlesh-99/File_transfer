

Kamalesh Garnayak(19CS10074)

To run this program:

In Server side:
$ gcc ftpS.c -o server
$./server


In Client side:
$ gcc ftpC.c -o client
$ ./client

NOTE:
1) For the first input in client side The 'open' command should be 
like (open 127.0.0.1 20000) for our case.
2) The flow in which we tested the server-client connection:
open -> user->pass-> cd -> get -> put -> mget -> lcd -> mput->dir -> quit
3) From the second time onwards "dir" command may get stuck
4) "mput" can be stuck sometimes or send some files.
