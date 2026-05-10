# pipex
[![42 School](https://img.shields.io/badge/42-Paris-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=for-the-badge)](https://github.com/42School/norminette)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=for-the-badge)](LICENSE)

> A robust C implementation of Unix pipes and process management, demonstrating deep understanding of inter-process communication, file descriptor manipulation, and system-level programming.
C reimplementation of Unix shell pipes using raw POSIX syscalls — no `popen`, no `system`, no abstractions.

```bash
./pipex infile "cmd1" "cmd2" outfile
# equivalent to: < infile cmd1 | cmd2 > outfile
```

---

## How it works

Two child processes communicate through a kernel pipe. Each child redirects its I/O with `dup2`, then replaces itself via `execve`. The parent closes both pipe ends immediately after forking — this is what delivers EOF to `cmd2` once `cmd1` exits — then waits on both PIDs.

![Process architecture](https://github.com/user-attachments/assets/2ff8bdf0-40b1-4487-aef0-42d28976b880)

---

## Process lifecycle

![Process lifecycle](https://github.com/user-attachments/assets/23e6d84f-c861-4c72-a94d-d690dbc287df)

The order of operations in the parent matters: `close(pipe_fds)` must happen before `waitpid`. If the parent keeps the write end open, `cmd2` never sees EOF and hangs indefinitely.

---

## File descriptor discipline

Every child closes all pipe ends it does not use before calling `execve`. Failing to close even one unused end causes a hang or a descriptor leak inherited by the exec'd program.

```c
// Child 1 — reads from infile, writes to pipe
dup2(fd_in,     STDIN_FILENO);   // infile → stdin
dup2(pipefd[1], STDOUT_FILENO);  // stdout → pipe write end
close(fd_in);
close(pipefd[0]); // not using read end
close(pipefd[1]); // duplicated to stdout, original no longer needed
execve(cmd1, args, env);
```

---

## PATH resolution

Commands are resolved by splitting `PATH` on `:` and calling `access(candidate, X_OK)` on each concatenated path. Absolute paths bypass this. On failure, exits with code `127` (command not found) or `126` (not executable), matching shell behavior.

---

## Bonus

**Here-document (`<<`)** — reads stdin line by line until `DELIMITER` is matched, writes to a temp file in `/tmp`, uses it as `cmd1`'s stdin. Output is opened with `O_APPEND`. Temp file is unlinked after use.

```bash
./pipex here_doc DELIMITER "cmd1" "cmd2" outfile
```

**Multiple commands** — allocates `N−1` pipes dynamically for `N` commands. Each intermediate child gets `pipe[i][0]` as stdin and `pipe[i+1][1]` as stdout.

![Multi-command pipeline](https://github.com/user-attachments/assets/42f48375-4ab4-4e24-b509-effccd07a250)

```bash
./pipex infile "cmd1" "cmd2" "cmd3" ... "cmdN" outfile
```

---

## Error handling

| Exit code | Meaning |
|-----------|---------|
| `0` | Success |
| `1` | General / syscall failure |
| `126` | Command found, not executable |
| `127` | Command not found in PATH |

All syscall failures print to stderr via `perror`, free allocated memory, and exit — no silent failures.

---

## Build

```bash
make          # mandatory
make bonus    # here_doc + multi-command pipeline
make re       # clean rebuild
```

Requires GCC/Clang, Make, POSIX-compliant OS (Linux, macOS, *BSD).

---

## Memory

Zero leaks under Valgrind. All heap allocations are freed before `execve` or on any error path. File descriptors are tracked and closed in every exit branch.

```bash
valgrind --leak-check=full ./pipex infile "cat" "wc -l" outfile
# All heap blocks were freed -- no leaks are possible
```

---

## Benchmarks

```bash
# 2-command pipeline
$ time ./pipex large_file.txt "grep pattern" "wc -l" out.txt
real    0m0.023s   user    0m0.008s   sys     0m0.015s

# 5-command pipeline (bonus)
$ time ./pipex data.csv "cut -d, -f2" "sort" "uniq" "grep '^A'" "wc -l" result.txt
real    0m0.089s   user    0m0.045s   sys     0m0.044s
```

---

## References

- `fork(2)`, `pipe(2)`, `dup2(2)`, `execve(2)`, `waitpid(2)` — Linux man pages
- *Advanced Programming in the UNIX Environment*, Stevens & Rago
- POSIX.1-2017 — [pubs.opengroup.org](https://pubs.opengroup.org/onlinepubs/9699919799/)
