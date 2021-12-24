# minishell  
![render1632878809870](https://user-images.githubusercontent.com/76856052/135187498-f8a922a5-38c3-41a5-b368-81309fc1da9a.gif)

# About  
minishell is a simple shell program that implements the minimum functionality of bash.  
Using the C programmming language.  

# Building and Running    
```
git clone "URL"
cd minishell
make
./minishell
```

# Description  
- Special characters to be interpreted:
    - quotes : '', ""
    - pipe : |
    - redirect and heredocument : <, <<, >, >>
- Implemented builtins:
    - echo with option -n
    - cd with only a relative or absolute path
    - pwd with no options
    - export with no options
    - unset with no options
    - env with no options or arguments
    - exit with no options
- Environment variables ($ followed by characters) expand to their values.
- $? expand to the exit status of the most recently executed foreground pipeline.
- ctrl-C ctrl-D ctrl-\ work like in bash.
- When interactive:
    - ctrl-C print a new prompt on a newline
    - ctrl-D exit the shell
    - ctrl-\ do nothing
# For more information
- rsudo's Hatena Blog: [bash再実装課題の振り返り](https://rio-1.hatenablog.com/entry/2021/10/05/182916?_ga=2.192044130.99751359.1640076943-702415775.1516097932)
- rakiyama's Hatena Blog: [シェルの再実装しましたというお話。](https://rakiyama0229.hatenablog.jp/entry/2021/10/05/120920)


