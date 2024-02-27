#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"

// Given a message as input, print it to the screen followed by a
// newline ('\n'). If the message contains the two-byte escape sequence
// "\\n", print a newline '\n' instead. No other escape sequence is
// allowed. If the sequence contains a '$', it must be an environment
// variable or the return code variable ("$?"). Environment variable
// names must be wrapped in curly braces (e.g., ${PATH}).
//
// Returns 0 for success, 1 for errors (invalid escape sequence or no
// curly braces around environment variables).
int
echo (char *message)
{
  if (message != NULL)
    message[strlen (message) - 1] = '\0';
  char *token = strtok (message, "\\n");

  do
    {
      printf ("%s\n", token);
      token = strtok (NULL, "\\n");
    }
  while (token != NULL);

  return 0;
}

// Given a key-value pair string (e.g., "alpha=beta"), insert the mapping
// into the global hash table (hash_insert ("alpha", "beta")).
//
// Returns 0 on success, 1 for an invalid pair string (kvpair is NULL or
// there is no '=' in the string).
int export(char *kvpair) { return 0; }

// Prints the current working directory (see getcwd()). Returns 0.
int
pwd (void)
{
  char *buffer;
  size_t bufferSize = 1024;
  buffer = (char *)malloc (bufferSize * sizeof (char));
  if (buffer == NULL)
    {
      perror ("Unable to allocate buffer for current working directory.");
      return -1;
    }

  if (getcwd (buffer, bufferSize) == NULL)
    {
      perror ("Error getting current working directory");
      free (buffer);
      return -1;
    }

  printf ("%s\n", buffer);
  free (buffer);

  return 0;
}

// Removes a key-value pair from the global hash table.
// Returns 0 on success, 1 if the key does not exist.
int
unset (char *key)
{
  return 0;
}

// Given a string of commands, find their location(s) in the $PATH global
// variable. If the string begins with "-a", print all locations, not just
// the first one.
//
// Returns 0 if at least one location is found, 1 if no commands were
// passed or no locations found.
// Function to check if a command is a built-in command
int
which (char *cmdline)
{
  if (cmdline == NULL || strlen (cmdline) == 0)
    {
      return 1;
    }

  // Handle built-in commands
  if (is_builtin (cmdline))
    {
      return 0;
    }

  // Handle commands starting with ./
  if (strncmp (cmdline, "./", 2) == 0)
    {
      if (access (cmdline, X_OK) == 0)
        {
          printf ("%s\n", cmdline);
          return 0;
        }
      else
        {
          return 1;
        }
    }

  // Handle commands in the $PATH
  char *path = getenv ("PATH");
  if (path == NULL)
    {
      return 1;
    }

  char *pathCopy = strdup (path);
  if (pathCopy == NULL)
    {
      perror ("Failed to duplicate PATH string");
      return 1;
    }

  int found = 0;
  char *dir = strtok (pathCopy, ":");
  while (dir != NULL)
    {
      size_t fullPathLength = strlen (dir) + 1 + strlen (cmdline) + 1;
      char *fullPath = malloc (fullPathLength);
      if (fullPath == NULL)
        {
          perror ("Failed to allocate memory for full path");
          free (pathCopy);
          return 1;
        }

      snprintf (fullPath, fullPathLength, "%s/%s", dir, cmdline);
      if (access (fullPath, X_OK) == 0)
        {
          printf ("%s\n", fullPath);
          found = 1;
          free (fullPath);
          break;
        }
      free (fullPath);
      dir = strtok (NULL, ":");
    }

  free (pathCopy);
  return found ? 0 : 1;
}

int
cd (char *path)
{
  char *buffer;
  size_t bufferSize = 1024;
  buffer = (char *)malloc (bufferSize * sizeof (char));
  if (buffer == NULL)
    {
      perror ("Unable to allocate buffer for current working directory.");
      return -1;
    }

  if (getcwd (buffer, bufferSize) == NULL)
    {
      perror ("Error getting current working directory");
      free (buffer);
      return -1;
    }
    
  // checks for absolute or relative path
  if (path[0] == '/')
    {
      if (chdir (path) == -1)
        {
          free (buffer);
          perror ("Error changing directory");
          return -1;
        }
    }
  else
    {
      char buf[256];
      snprintf (buf, sizeof (buf), "%s/%s", buffer, path);
      if (chdir (buf) == -1)
        {
          free (buffer);
          perror ("Error changing directory");
          return -1;
        }
    }

  free (buffer);
  return 0;
}
