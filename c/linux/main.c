#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int const dirBufSize = 1024;

char const *const usage = "Usage: wd DIR CMD [ARGS]\n";

#define LOG_ERROR(stream, message) \
  (logError(stream, message, errno, __FILE__, __LINE__))

void logError(FILE *const stream, char message[], int const errorNo, char file[], int const line)
{
  fprintf(stream, "Error (%s:%d): %s: %s (%d)\n", file, line, message, strerror(errorNo), errorNo);
}

int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    fprintf(stderr, "Error: At least 2 arguments are necessary.\n");
    fprintf(stderr, usage);
    return EXIT_FAILURE;
  }
  {
    char originalDir[dirBufSize];
    char *dir = argv[1];
    char *cmd = argv[2];
    char *newArgs[argc];
    {
      int newIndex;
      newArgs[0] = cmd;
      {
        int index;
        for (index = 3, newIndex = 1; index < argc; index++, newIndex++)
          newArgs[newIndex] = argv[index];
      }
      newArgs[newIndex] = NULL;
    }
    if (NULL == getcwd(originalDir, dirBufSize))
    {
      LOG_ERROR(stderr, "getcwd");
      return EXIT_FAILURE;
    }
    if (-1 == chdir(dir))
    {
      LOG_ERROR(stderr, "chdir");
      return EXIT_FAILURE;
    }
    if (-1 == execvp(cmd, newArgs))
    {
      LOG_ERROR(stderr, "execvp");
      if (-1 == chdir(originalDir))
        LOG_ERROR(stderr, "chdir");
      return EXIT_FAILURE;
    }
    if (-1 == chdir(originalDir))
    {
      LOG_ERROR(stderr, "chdir");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
