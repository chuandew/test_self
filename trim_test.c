#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

int main()
{
    int *m1,*m2,*m3,*m4;
    printf("%s\n","Test started");
    m1=(int*)malloc(20000);
    m2=(int*)malloc(40000);
    m3=(int*)malloc(80000);
    m4=(int*)malloc(10000);
    // check that all arrays are allocated on the heap and not with mmap
    printf("1:%p 2:%p 3:%p 4:%p\n", m1, m2, m3, m4);
    // free 40000 bytes in the middle
    free(m2);
    // call trim (same result with 2000 or 2000000 argument)
    malloc_trim(0);
    // call some syscall to find this point in the strace output
    sleep(1);
    free(m1);
    free(m3);
    free(m4);
    malloc_stats();
//malloc_info(0, stdout);
    return 0;
}