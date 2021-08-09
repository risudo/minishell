#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>

//-lreadline
int main(void)
{
	char *command;

	using_history();
	read_history(".my_history"); // [ToDo]historyファイルが無いときの動作の検証
	while (1)
	{
		command = readline(">> ");
		add_history(command);

		/* コマンドの処理 */

		free(command);
	}
	write_history(".my_history");

	return 0;
}
