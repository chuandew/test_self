#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct emp {
  char name[15];
  int age;
  int salary;
  char address[30];
};

int main(void) {
  char const filename[] = "sampledata.bin";
  struct emp emp1[5] = {
      {"Yoda", 23, 45000, "Asia"},   {"Darth", 34, 2344, "N America"},
      {"Jabba", 22, 5566, "Africa"}, {"Luke", 33, 3399, "S America"},
      {"Leia", 44, 6677, "Europe"},
  };
  struct emp emp2[5];
  int i;

  int ofd = open(filename, O_WRONLY | O_CREAT, 0777);
  ssize_t w = pwrite(ofd, emp1, sizeof(emp1), 1);
  fprintf(stdout, "success to write to %s %ld\n", filename, w);
  close(ofd);

  int ifd = open(filename, O_RDONLY);
  ssize_t r = pread(ifd, emp2, sizeof(emp2), 1);
  fprintf(stdout, "success to read from %s %ld\n", filename, r);
  close(ifd);

  for (i = 0; i < 5; i++) {
    if (emp1[i].age != emp2[i].age || emp1[i].salary != emp2[i].salary ||
        strcmp(emp1[i].name, emp2[i].name) != 0 ||
        strcmp(emp1[i].address, emp2[i].address) != 0)
      printf("Difference in record %d\n", i);
  }

  return 0;
}