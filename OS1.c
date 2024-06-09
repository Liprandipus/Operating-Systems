/*
Write a C program with the appropriate fork() commands to create a total (including the main
program) seven processes, with the following kinship structure (tree):
P0
 / \
 P1 P2
 / \ / \
P3 P4 P5 P6
Then (after all the processes dictated by the above tree have been created), as
main part of its work each Pi process should print a message on the screen indicating the
its name (Pi), its PID and its PPID. In addition, your program should include the
appropriate wait-command commands to satisfy the following constraints-requirements:
(a) Process P0 must wait for process P2 to complete its execution before completing its execution.
Upon completion of its execution, P0 should also be replaced by the ps instruction.
(b) Process P1 before completing its execution should first wait to receive a message 'hello
from your child' from process P3 (and print it to the screen). Then it should also wait for
the completion of process P4 and print its exit value.
(c) Process P2 should wait for the completion of one of the two
of one of its children and print which one (i.e. the PID of the completed child process).
You also extend your program so that process P2 creates N processes/immediate children (instead of
the only two processes/immediate children P5,P6 that it appears to create in the above figure), where N would
is given by the user. Each of the N processes created should simply print a
diagnostic message with its PID and PPID and terminate.
Is it possible during the execution of your program that orphan or zombie processes will be created? And which ones
might they be? Comment / adequately document your answer.
*/

//Answer :

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h> 
 // Already running with the program execution the parent process 0
 // Before we get to the main program, I would like to make some comments since I don't want to repeat the comments in the code in order to make it not tedious
 // If fork() returns 0, then it means that a child is created. If it returns >0 then it means we are in the parent process. If it returns -1, then it's an error
 // The pid_t declaration is an integer which is better suited to the declaration and creation of fork()
 // waitpid() takes as first argument the process, its status and option. 
int main() {
    pid_t pid1, pid2, pid3, pid4, pid5, pid6; // declaration of the 6 processes requested by the call
    int status; // declaration of a status for the anonymous process 4 

    pid1 = fork(); //create the first process
    if (pid1 == 0) { // check if fork() returns child
        pid3 = fork(); //if it returns a child, then fork() for the children of 1
        if (pid3 == 0) { // if it returns a child, then
            printf("P3 with pid %d and ppid %d\n", getpid(), getppid()); // display pid and ppid of 3
        }
        else { // else 
            waitpid(pid3, NULL, 0); // wait for 3 to complete with waitpid 
            pid4 = fork(); // create process 4
            if (pid4 == 0) { // if fork() gives a child
                printf("P4 with pid %d and ppid %d\n", getpid(), getppid()); // display pid and ppid of p4
                exit(123); // and exit from 4 with a specific value in order to pass its value to waitpid()
            }
            else {
                waitpid(pid4, &status, 0); // wait for 4 to complete and the status will be passed the exit value of 4. (status = 123)
                printf("P1 with pid %d and ppid %d received message 'hello from your child '\n", getpid(), getppid()); // display the message received together
                // with the pid and ppid of 1
                printf("P1 with pid %d and ppid %d received exit status of P4: %d\n", getpid(), getppid(), WEXITSTATUS(status)); // display again 
                // the pid and ppid of 1 and the exit status of 4 ( from exit(123))
            }
        }
    }
    else if (pid1 > 0) { // else if we are still in the parent if 1 carries a value >1
        pid2 = fork(); // do fork() and create 2
        if (pid2 == 0) { // if fork() gives a child
            int n; // declared an integer 
            printf("Enter the number of processes to create for P2: "); // print 
            scanf("%d", &n); // and read from the standard input the number of processes the user wants to create
            for (int i = 0; i < n; i++) { // for each of the processes
                pid_t child_pid = fork(); // declare a variable that creates a child n times
                if (child_pid == 0) { // if fork() returns a child then
                    printf("Child %d of P2 with pid %d and ppid %d\n", i+1, getpid(), getppid()); // display the pid and ppid of each child
                    exit(9);;  
                }
            }
           
        }
        else if (pid2 > 0) { // else
            waitpid(pid2, NULL, 0); // wait for 2 to complete in order to run 0
            printf("P0 with pid %d and ppid %d\n", getpid(), getppid()); // printf("P0 with pid %d and ppid %d\n", getpid(), getppid()); // display pid and ppid of p0
            system("ps"); // with system("ps") we display the ps terminal command inside C. In essence, it allows us to 
            // to put linux commands as commands in C.
        }
    }

    return 0;
}

//When the program is run, we show that in the creation of N processes, orphan processes are created. This is because when creating
//all processes, the buffers may not be cleared. In other words, the "kill" of a process may not be smooth and thus the buffers may not be cleared. 
// the necessary resources resulting in the creation of orphans.
