malloc537
=========

Repository for malloc537 - memory-checking malloc replacement!

Use:
Using the makefile should produce a malloc537.o file!
If you wanto to call the program 537malloc - just rename the final .o
or edit the makefile. It just seemed odd to have the project and the functions
with different names.

If desired, use print_func() to print the current tree. print needs the root,
and in our implementation, that's internal to the tree, so the wrapper function
must be used.

ERRATA:
Our function to detect overlapped nodes doesn't seem to work. So we
don't actually end up deleting nodes. Not good for memory usage, but
we have successfully tested our library with 537ps - runs just fine.

Tested on various mumble lab machines, including:
mumble-15, mumble-38
