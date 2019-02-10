/*
Get physical device and mount point of a file or directory.
By Zibri <zibri AT zibri DOT org>
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <libgen.h>

int get_device(char *name)
{
  struct stat fs;

  if (stat(name, &fs) < 0) {
    fprintf(stderr, "%s: No such file or directory\n", name);
    return -1;
  }

  FILE *f;
  char sline[256];
  char minmaj[128];

  sprintf(minmaj, "%d:%d ", (int) fs.st_dev >> 8, (int) fs.st_dev & 0xff);

  f = fopen("/proc/self/mountinfo", "r");

  if (f == NULL) {
    fprintf(stderr, "Failed to open /proc/self/mountinfo\n");
    exit(-1);
  }

  while (fgets(sline, 256, f)) {

    char *token;
    char *where;

    token = strtok(sline, "-");
    where = strstr(token, minmaj);
    if (where) {
      token = strtok(NULL, " -:");
      token = strtok(NULL, " -:");
      printf("%s", token);
      token = strtok(sline, " ");  // ID
      token = strtok(NULL, " ");   // Parent ID
      token = strtok(NULL, " ");   // Major:Minor # could be splitted with token = strtok(NULL, ":");
      token = strtok(NULL, " ");   // Parent mount point
      printf("test:>%s<\n",token);
      token = strtok(NULL, " ");   // Mount point
      printf(" (%s)\n", token);
      break;
    }

  }

  return -1;
  fclose(f);
}

int main(int argc, char **argv)
{

  if (argc != 2) {
    fprintf(stderr, "Usage:\n%s FILE OR DIRECTORY...\n", basename(argv[0]));
    return -1;
  }
  get_device(argv[1]);
  return 0;
}
