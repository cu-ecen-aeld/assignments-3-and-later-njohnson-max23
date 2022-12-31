#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    return (system(cmd) == 0);

    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    pid_t child_pid = fork();

    if(child_pid == -1)
    {
        /* In parent, fork call failed. */
        perror("fork call failed");
        return false;
    }
    else if(child_pid == 0)
    {
        /* In child */
        int retval = execv(command[0], command);
        if(retval == -1)
        {
            perror("execv failed");
            exit(-1);
        }
    }
    else
    {
        /* In parent */
        int child_ret_status = 0;
        pid_t terminated_pid = waitpid(child_pid, &child_ret_status, 0);

        if(terminated_pid == child_pid)
        {
            if(WIFEXITED(child_ret_status))
            {
                printf("Child exited with status %d\n", WEXITSTATUS(child_ret_status));

                if(WEXITSTATUS(child_ret_status) == 0)
                {
                    return true;
                }
            }
        }
    }

    va_end(args);

    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    int fd = open(outputfile, 
                  O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    pid_t child_pid = fork();

    if(child_pid == -1)
    {
        /* In parent, fork call failed. */
        perror("fork call failed");
        close(fd);
        return false;
    }
    else if(child_pid == 0)
    {
        /* In child */
        if(dup2(fd, 1) < 0)
        {
            perror("stdout redirect failed");
            close(fd);
            exit(-1);
        }

        int retval = execv(command[0], command);
        if(retval == -1)
        {
            perror("execv failed");
            exit(-1);
        }
    }
    else
    {
        /* In parent */
        close(fd);
        int child_ret_status = 0;
        pid_t terminated_pid = waitpid(child_pid, &child_ret_status, 0);

        if(terminated_pid == child_pid)
        {
            if(WIFEXITED(child_ret_status))
            {
                printf("Child exited with status %d\n", WEXITSTATUS(child_ret_status));

                if(WEXITSTATUS(child_ret_status) == 0)
                {
                    return true;
                }
            }
        }
    }

    va_end(args);

    return false;
}
