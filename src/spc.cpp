#include <bhtool/spc.hpp>

/*
Adapted from `Secure Programming Cookbook`
https://resources.oreilly.com/examples/9780596003944
*/

//popen
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//files
#include <limits.h>
#include <sys/stat.h>
#include <paths.h>

//privs
#include <sys/param.h>

#include <iostream>

namespace
{
    //Functions in this namespace are not from the SPC book
    //these are helpers I made to clean things up

    const int PIPE_DES_READ = 0;
    const int PIPE_DES_WRITE = 1;

    void spc_fd_close_safe(int fd)
    {
        if (fd >= 0)
        {
            close(fd);
        }
    }

    void spc_uninit(SPC_PIPE *p)
    {
        spc_fd_close_safe(p->stdin_pipe[PIPE_DES_WRITE]);
        spc_fd_close_safe(p->stdin_pipe[PIPE_DES_READ]);
        spc_fd_close_safe(p->stdout_pipe[PIPE_DES_WRITE]);
        spc_fd_close_safe(p->stdout_pipe[PIPE_DES_READ]);
        spc_fd_close_safe(p->stderr_pipe[PIPE_DES_WRITE]);
        spc_fd_close_safe(p->stderr_pipe[PIPE_DES_READ]);

        if (p->stdin_fd)
            fclose(p->stdin_fd);
        if (p->stdout_fd)
            fclose(p->stdout_fd);
        if (p->stderr_fd)
            fclose(p->stderr_fd);

        free(p);
    }

    SPC_PIPE *spc_init()
    {
        SPC_PIPE *p;

        if (!(p = (SPC_PIPE *)malloc(sizeof(SPC_PIPE))))
        {
            std::cerr << "spc_init malloc failed\n";
            return 0;
        }

        p->stdin_pipe[PIPE_DES_WRITE]  = p->stdin_pipe[PIPE_DES_READ]  =
        p->stdout_pipe[PIPE_DES_WRITE] = p->stdout_pipe[PIPE_DES_READ] =
        p->stderr_pipe[PIPE_DES_WRITE] = p->stderr_pipe[PIPE_DES_READ] = -1;

        p->stdout_fd = p->stdin_fd = p->stderr_fd = 0;

        p->child_pid = -1;

        if (pipe(p->stdin_pipe) == -1 ||
            pipe(p->stdout_pipe) == -1 ||
            pipe(p->stderr_pipe) == -1)
        {
            std::cerr << "spc_init pipes failed\n";

            spc_uninit(p);
            return 0;
        }

        if (
            !(p->stdin_fd  = fdopen(p->stdin_pipe[PIPE_DES_WRITE], "w")) ||
            !(p->stdout_fd = fdopen(p->stdout_pipe[PIPE_DES_READ], "r")) ||
            !(p->stderr_fd = fdopen(p->stderr_pipe[PIPE_DES_READ], "r")))
        {
            std::cerr << "spc_init fds failed\n";

            spc_uninit(p);
            return 0;
        }

        return p;
    }

    void spc_pipe_plug_dup(int pipe_des[2], int des_to_close, int des_to_dup, int fd_to_dup)
    {
        dup2(pipe_des[des_to_dup], fd_to_dup);
        close(pipe_des[des_to_close]);
    }

}

SPC_PIPE *spc_popen(const char *path, char *const argv[], char *const /*envp*/[])
{

    auto p = spc_init();
    if (!p)
    {
        return 0;
    }

    if ((p->child_pid = spc_fork()) == -1)
    {
        spc_uninit(p);
        std::cerr << "spc_popen fork failed\n";
        return 0;
    }

    if (0 == p->child_pid)
    {

        /* this is the child process */
        //close input side of child's stdout, stderr
        //close the parent side of stdin
        //dup our pipes into the things so we consume that data instead of terminal
        spc_pipe_plug_dup(p->stdin_pipe, PIPE_DES_WRITE, PIPE_DES_READ, STDIN_FILENO);
        spc_pipe_plug_dup(p->stdout_pipe, PIPE_DES_READ, PIPE_DES_WRITE, STDOUT_FILENO);
        spc_pipe_plug_dup(p->stderr_pipe, PIPE_DES_READ, PIPE_DES_WRITE, STDERR_FILENO);

        //using execvp because I choose to trust my current environment, see book for notes why this is NOT secure.
        // execve(path, &(argv[0]), env);
        execvp(path, &(argv[0]));

        //execvp should not return unless the world ends
        exit(127);
    }

    //this is the parent, close the pipe sides the parent will not use
    //the files are still open
    close(p->stdin_pipe[PIPE_DES_READ]);
    close(p->stdout_pipe[PIPE_DES_WRITE]);
    close(p->stderr_pipe[PIPE_DES_WRITE]);

    return p;
}

int spc_pclose(SPC_PIPE *p)
{
    int status;
    pid_t pid = -1;

    if (p->child_pid != -1)
    {
        do
        {
            pid = waitpid(p->child_pid, &status, 0);
        } while (pid == -1 && errno == EINTR);
    }

    if (p->stdin_fd)
        fclose(p->stdin_fd);
    if (p->stdout_fd)
        fclose(p->stdout_fd);
    if (p->stderr_fd)
        fclose(p->stderr_fd);

    free(p);

    if (pid != -1 && WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return (pid == -1 ? -1 : 0);
}

pid_t spc_fork(void)
{
    pid_t childpid;

    if ((childpid = fork()) == -1)
        return -1;

    /* Reseed PRNGs in both the parent and the child */
    /* See Chapter 11 for examples */

    /* If this is the parent process, there's nothing more to do */
    if (childpid != 0)
        return childpid;

    /* This is the child process */
    //   spc_sanitize_files(  );   /* Close all open files.  See Recipe 1.1 */
    spc_drop_privileges(true); /* Permanently drop privileges.  See Recipe 1.3 */

    return 0;
}

#ifndef OPEN_MAX
#define OPEN_MAX 256
#endif

static int open_devnull(int fd)
{
    FILE *f = nullptr;

    const char* mode = nullptr;
    FILE* file_ptr_to_reopen = nullptr;

    switch(fd) {
    case 0: mode = "rb"; file_ptr_to_reopen = stdin ; break;
    case 1: mode = "wb"; file_ptr_to_reopen = stdout; break;
    case 2: mode = "wb"; file_ptr_to_reopen = stderr; break;
    default:
        return false;
    }

    f = freopen(_PATH_DEVNULL, mode, file_ptr_to_reopen);

    return (f && fileno(f) == fd);
}

void spc_sanitize_files(void)
{

    /* Make sure all open descriptors other than the standard ones are closed */
    int fds;
    if ((fds = getdtablesize()) == -1)
        fds = OPEN_MAX;

    for (int fd = 3; fd < fds; fd++)
        close(fd);

    /* Verify that the standard descriptors are open.  If they're not, attempt to
   * open them using /dev/null.  If any are unsuccessful, abort.
   */
    for (int fd = 0; fd < 3; fd++)
    {
        struct stat st;
        if (fstat(fd, &st) == -1 && (errno != EBADF || !open_devnull(fd)))
            abort();
    }
}

static int orig_ngroups = -1;
static gid_t orig_gid = -1;
static uid_t orig_uid = -1;
static gid_t orig_groups[NGROUPS_MAX];

void spc_drop_privileges(bool permanent)
{
    gid_t newgid = getgid(), oldgid = getegid();
    uid_t newuid = getuid(), olduid = geteuid();

    if (!permanent)
    {
        /* Save information about the privileges that are being dropped so that they
     * can be restored later.
     */
        orig_gid = oldgid;
        orig_uid = olduid;
        orig_ngroups = getgroups(NGROUPS_MAX, orig_groups);
    }

    /* If root privileges are to be dropped, be sure to pare down the ancillary
   * groups for the process before doing anything else because the setgroups()
   * system call requires root privileges.  Drop ancillary groups regardless of
   * whether privileges are being dropped temporarily or permanently.
   */
    if (olduid == 0)
        setgroups(1, &newgid);

    if (newgid != oldgid)
    {
#if !defined(linux)
        setegid(newgid);
        if (permanent && setgid(newgid) == -1)
            abort();
#else
        if (setregid((permanent ? newgid : -1), newgid) == -1)
            abort();
#endif
    }

    if (newuid != olduid)
    {
#if !defined(linux)
        seteuid(newuid);
        if (permanent && setuid(newuid) == -1)
            abort();
#else
        if (setregid((permanent ? newuid : -1), newuid) == -1)
            abort();
#endif
    }

    /* verify that the changes were successful */
    if (permanent)
    {
        if (newgid != oldgid && (setegid(oldgid) != -1 || getegid() != newgid))
            abort();
        if (newuid != olduid && (seteuid(olduid) != -1 || geteuid() != newuid))
            abort();
    }
    else
    {
        if (newgid != oldgid && getegid() != newgid)
            abort();
        if (newuid != olduid && geteuid() != newuid)
            abort();
    }
}

void spc_restore_privileges(void)
{
    if (geteuid() != orig_uid)
        if (seteuid(orig_uid) == -1 || geteuid() != orig_uid)
            abort();
    if (getegid() != orig_gid)
        if (setegid(orig_gid) == -1 || getegid() != orig_gid)
            abort();
    if (!orig_uid)
        setgroups(orig_ngroups, orig_groups);
}
