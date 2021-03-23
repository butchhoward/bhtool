#ifndef SPC_H_INCLUDED
#define SPC_H_INCLUDED

#include <sys/types.h>
#include <stdio.h>

typedef struct SPC_PIPE {
  FILE  *read_fd;
  FILE  *write_fd;
  pid_t child_pid;
} SPC_PIPE;


SPC_PIPE *spc_popen(const char *path, char *const argv[], char *const envp[]);
int spc_pclose(SPC_PIPE *p);

pid_t spc_fork(void);

void spc_sanitize_files(void);

void spc_drop_privileges(int permanent);
void spc_restore_privileges(void);
#endif
