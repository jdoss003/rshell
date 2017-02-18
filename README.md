# rshell
This code implements a super simplified bash clone through the terminal window. Programmed in C++ and does system calls using execvp


####Input
Input is entered in the following format:

*cmd = executable [ argumentList ] [ connector cmd ]*

#####There are three accepted connectors: ;, &&, and ||.

If a command is followed by a connector ;, then the next command is always executed.

* *EXAMPLE:*
````
$ ls -a; echo hello there; mkdir test
````

If a command is followed by &&, then the next command is executed only if the first one succeeds.  If a command is followed by ||, then the next command is executed only if the first one fails. 

* *EXAMPLE:*
````
$ ls -a; echo hello && mkdir test || echo world; git status
````
#####You can enter quotes around arguments. 

If you begin and ends with " the program will grab whatever is in between the quotations as a single argument.

* *EXAMPLE:*
````
$ echo "hello there"  
hello there
````
If you begin and end arguments with a forward slash and a quote \\" the program will keep the quotes in the argument.

* *EXAMPLE:*
````
$ echo \"hello\" 
"hello"
````
#####You can enter comments using \#
* *EXAMPLE:*
````
$ echo hello #this is a comment
hello
````
#####To end the program enter exit
* *EXAMPLE:*
````
$ exit
Process finished with exit code 0
````
###Installation

````
$ git clone https://github.com/jdoss003/rshell.git
$ cd rshell
$ git checkout hw2
$ make 
$ bin/rshell
````

###Bugs/Limitations
1. Windows is not supported (WIP)

###Author Contact
Derek A. Sayler 
* Email: dsayl001@ucr.edu

Justin Doss: 
* Email: jdoss003@ucr.edu