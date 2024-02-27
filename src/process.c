#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// The contents of this file are up to you, but they should be related to
// running separate processes. It is recommended that you have functions
// for:
//   - performing a $PATH lookup
//   - determining if a command is a built-in or executable
//   - running a single command in a second process
//   - running a pair of commands that are connected with a pipe

const char *builtins[]
    = { "echo", "cd", "pwd", "export", "unset", "which", "quit", NULL };

int
is_builtin (char *command)
{
  if (strncmp (command, "which", 5) == 0)
    {
      printf ("%s: dukesh built-in command\n", "which");
      return 1;
    }

  for (int i = 0; builtins[i] != NULL; i++)
    {
      if (strncmp (command, builtins[i], 6) == 0)
        {
          printf ("%s: dukesh built-in command\n", command);
          return 1;
        }
    }
  return 0; // Command is not a built-in
}
