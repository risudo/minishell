#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define PATHNAME_SIZE 512

int main(void)
{
    // 変数定義
    char pathname[PATHNAME_SIZE];  // ファイルパス
	char *pwd_list[3] = {"pwd", NULL};
	char *ls_list[3] = {"ls", NULL};

    // 変数初期化
    memset(pathname, '\0', PATHNAME_SIZE); 

    // カレントディレクトリ取得
    getcwd(pathname, PATHNAME_SIZE);
    fprintf(stdout,"現在のファイルパス:%s\n", pathname);

    // カレントディレクトリ変更  
	chdir("/Users/ryojiroakiyama/Desktop/icloud/42_cursus/pipex_dir"); // チェンジディレクトリ
	getcwd(pathname, PATHNAME_SIZE);
	fprintf(stdout,"現在のファイルパス:%s\n", pathname);
	execve("/bin/ls", ls_list, NULL);
	execve("/bin/pwd", pwd_list, NULL);

    return 0;
}
