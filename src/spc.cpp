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

namespace {
void spc_fd_close_safe(int fd)
{
    if ( fd >= 0 )
    {
        close(fd);
    }
}

void spc_uninit(SPC_PIPE* p)
{
    spc_fd_close_safe(p->stdin_pipe[0]);
    spc_fd_close_safe(p->stdin_pipe[1]);
    spc_fd_close_safe(p->stdout_pipe[0]);
    spc_fd_close_safe(p->stdout_pipe[1]);
    spc_fd_close_safe(p->stderr_pipe[0]);
    spc_fd_close_safe(p->stderr_pipe[1]);

    if (p->stdin_fd) fclose(p->stdin_fd);
    if (p->stdout_fd) fclose(p->stdout_fd);
    if (p->stderr_fd) fclose(p->stderr_fd);

    free(p);
}

SPC_PIPE *spc_init()
{
    SPC_PIPE *p;

    if ( !(p = (SPC_PIPE *)malloc(sizeof(SPC_PIPE))) )
    {
        std::cerr << "spc_init malloc failed\n";
        return 0;
    }

    p->stdin_pipe[0] = p->stdin_pipe[0] =
    p->stdout_pipe[0] = p->stdout_pipe[0] =
    p->stderr_pipe[0] = p->stderr_pipe[0] = -1;

    p->stdout_fd = p->stdin_fd = p->stderr_fd = 0;

    p->child_pid = -1;


    if ( pipe(p->stdin_pipe) == -1 ||
         pipe(p->stdout_pipe) == -1 ||
         pipe(p->stderr_pipe) == -1 ||
         !(p->stdin_fd = fdopen(p->stdin_pipe[1], "w")) ||
         !(p->stdout_fd = fdopen(p->stdout_pipe[0], "r")) ||
         !(p->stderr_fd = fdopen(p->stderr_pipe[0], "r"))
    )
    {
        std::cerr << "spc_init failed\n";

        spc_uninit(p);
        return 0;
    }

    return p;
}

void spc_pipe_plug_dip(int piper[2], int to_close, int to_dup, int fd_to_dup)
{
    close(piper[to_close]);
    if (piper[to_dup] != fd_to_dup) {
        dup2(piper[to_dup], fd_to_dup);
        close(piper[to_dup]);
    }
}


}

SPC_PIPE *spc_popen(const char *path, char *const argv[], char *const /*envp*/[])
{

    auto p = spc_init();
    if (!p)
    {
        return 0;
    }

    if ((p->child_pid = spc_fork()) == -1) {
        spc_uninit(p);
        std::cerr << "spc_popen fork failed\n";
        return 0;
    }

  if (!p->child_pid) {

    /* this is the child process */
    //close input side of child's stdout, stderr
    //close the parent side of stdin
    spc_pipe_plug_dip(p->stdin_pipe,  1, 0, 0);
    spc_pipe_plug_dip(p->stdout_pipe, 0, 1, 1);
    spc_pipe_plug_dip(p->stderr_pipe, 0, 1, 2);

    //using execvp because I choose to trust my current environment, see book for notes why this is NOT secure.
    execvp(path, &(argv[0]));

    //execvp should not return unless the world ends
    exit(127);
  }


    //this is the parent, close the pipe sides the parent will not use
    //the files are still open
    close(p->stdout_pipe[1]);
    close(p->stdin_pipe[0]);
    close(p->stderr_pipe[1]);

    return p;
}

int spc_pclose(SPC_PIPE *p) {
  int   status;
  pid_t pid;

  if (p->child_pid != -1) {
    do {
      pid = waitpid(p->child_pid, &status, 0);
    } while (pid == -1 && errno == EINTR);
  }
  if (p->stdin_fd) fclose(p->stdin_fd);
  if (p->stdout_fd) fclose(p->stdout_fd);
  if (p->stderr_fd) fclose(p->stderr_fd);

  free(p);

  if (pid != -1 && WIFEXITED(status))
    return WEXITSTATUS(status);
  else
    return (pid == -1 ? -1 : 0);
}


pid_t spc_fork(void) {
  pid_t childpid;

  if ((childpid = fork(  )) == -1) return -1;

  /* Reseed PRNGs in both the parent and the child */
  /* See Chapter 11 for examples */

  /* If this is the parent process, there's nothing more to do */
  if (childpid != 0) return childpid;

  /* This is the child process */
  spc_sanitize_files(  );   /* Close all open files.  See Recipe 1.1 */
  spc_drop_privileges(1); /* Permanently drop privileges.  See Recipe 1.3 */

  return 0;
}


#ifndef OPEN_MAX
#define OPEN_MAX 256
#endif

static int open_devnull(int fd) {
  FILE *f = 0;

  if (!fd) f = freopen(_PATH_DEVNULL, "rb", stdin);
  else if (fd == 1) f = freopen(_PATH_DEVNULL, "wb", stdout);
  else if (fd == 2) f = freopen(_PATH_DEVNULL, "wb", stderr);
  return (f && fileno(f) == fd);
}

void spc_sanitize_files(void) {
  int         fd, fds;
  struct stat st;

  /* Make sure all open descriptors other than the standard ones are closed */
  if ((fds = getdtablesize()) == -1) fds = OPEN_MAX;
  for (fd = 3;  fd < fds;  fd++) close(fd);

  /* Verify that the standard descriptors are open.  If they're not, attempt to
   * open them using /dev/null.  If any are unsuccessful, abort.
   */
  for (fd = 0;  fd < 3;  fd++)
    if (fstat(fd, &st) == -1 && (errno != EBADF || !open_devnull(fd))) abort();
}



static int   orig_ngroups = -1;
static gid_t orig_gid = -1;
static uid_t orig_uid = -1;
static gid_t orig_groups[NGROUPS_MAX];

void spc_drop_privileges(int permanent) {
  gid_t newgid = getgid(), oldgid = getegid();
  uid_t newuid = getuid(), olduid = geteuid();

  if (!permanent) {
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
  if (!olduid) setgroups(1, &newgid);

  if (newgid != oldgid) {
#if !defined(linux)
    setegid(newgid);
    if (permanent && setgid(newgid) == -1) abort();
#else
    if (setregid((permanent ? newgid : -1), newgid) == -1) abort();
#endif
  }

  if (newuid != olduid) {
#if !defined(linux)
    seteuid(newuid);
    if (permanent && setuid(newuid) == -1) abort();
#else
    if (setregid((permanent ? newuid : -1), newuid) == -1) abort();
#endif
  }

  /* verify that the changes were successful */
  if (permanent) {
    if (newgid != oldgid && (setegid(oldgid) != -1 || getegid() != newgid))
      abort();
    if (newuid != olduid && (seteuid(olduid) != -1 || geteuid() != newuid))
      abort();
  } else {
    if (newgid != oldgid && getegid() != newgid) abort();
    if (newuid != olduid && geteuid() != newuid) abort();
  }
}

void spc_restore_privileges(void) {
  if (geteuid() != orig_uid)
    if (seteuid(orig_uid) == -1 || geteuid() != orig_uid) abort();
  if (getegid() != orig_gid)
    if (setegid(orig_gid) == -1 || getegid() != orig_gid) abort();
  if (!orig_uid)
    setgroups(orig_ngroups, orig_groups);
}
