#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void get_mem_usage() {
  FILE *fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) {
    perror("Error opening /proc/meminfo");
    return;
  }
  char line[256];
  unsigned long mem_total = 0, mem_available = 0;

  while (fgets(line, sizeof(line), fp)) {

    if (strncmp(line, "MemTotal:", 9) == 0) {
      sscanf(line,
             "MemTotal:"
             "%lu kB",
             &mem_total);
    }

    // mesma coisa com o mem_free bicho
    if (strncmp(line, "MemAvailable:", 13) == 0) {
      sscanf(line,
             "MemAvailable:"
             "%lu kB",
             &mem_available);
    }
  }
  fclose(fp);

  float percent =
      ((float)mem_total - (float)mem_available) / (float)mem_total * 100;
  const int blocks = 20;
  int filled = (int)((percent * blocks) / 100);
  if (filled < 0)
    filled = 0;
  if (filled > blocks)
    filled = blocks;

  printf("[");
  for (int i = 0; i < blocks; i++) {
    if (i <= filled)
      printf("|");
    else
      printf(" ");
  }
  printf("]");
  printf("%.2f%%", percent);
  usleep(100000);
  printf(" ");
}

int main() {
  while (1) {
    printf("\033[H");
    printf("---HolyMonitor---\n");
    printf("MEMORY USAGE:\n");
    printf("   ");
    get_mem_usage();
    fflush(stdout);
    usleep(100000);
    printf("\n");
  }
  return 0;
}
