## 0.1.0 (2025-02-16)

### Feat

- **heredoc**: add heredoc, SIGINT, env_variable_exists
- **execution**: redirect single builtin
- **execution**: detect builtins, scaffold runner

### Fix

- **heredoc.c-&-cmd_redir.c**: fix unlink and unclosed FDs with and without SIGINT in heredoc
- **Norminette-check-OK**: few glitches eliminated
- **builtins**: pass cmd to builtins
- **shell**: memory leaks on empty lines & errors
