#ifndef RAKIYAMA_H
# define RAKIYAMA_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include "minishell.h"
# include "parse.h"

char	*ft_xstrjoin(char *str1, char *str2);
char	**ft_xsplit(char *src_str, char cut_char);
void	xclose(int fd);
void	xdup2(int oldfd, int newfd);
void	xwaitpid(pid_t pid, int *wstatus, int options);
void	xpipe(int *pipefd);
pid_t	xfork(void);

#endif