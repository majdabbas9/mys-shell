# mys-shell
myShell is a basic implementation of a Shell, written in C, demonstrating basics of how a shell works: read, parse, fork, execute and wait. this was a homework in the operating system course.
to compile ----> gcc –Werror –std=c99 myshell.c –o myshell
then you type in the linux cmd ---> ./myshell   and you are ready to go
you can call any instruction inside the linux such that  : date , echo hi (which just print hi) ..
to call it you just type the name of the instruction
and i added a custom instruction called history (n)  , which just print the last n instruction you executed.
 if u want your instruction to execute concurrently you add & at the end of the instruction like that "date &"
