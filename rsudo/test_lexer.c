#include "lexer.h"

void	put_list(t_token *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("[%d] [%s]\ntype: %d\n\n", i, list->str, list->type);
		list = list->next;
		i++;
	}
}

int	main(void)
{
	char	*cmd1 = "echo   'test hoge hoge'  \"fuga\" |   cat|cat>>file";
	// char	*cmd2 = "echo test | cat";
	// char	*cmd3 = " >>file1 echo aaa>> file2";
	// char	*cmd4 = " >>>>file1 echo aaa>> file2";

	printf("cmd1: [%s]\n", cmd1);
	parse_cmd(cmd1);
	if (system("leaks a.out > /dev/null"))
		system("leaks a.out");
}
