/*

  This test recreates a deadlock in glibc's dynamic loader.
  
  When a thread calls dl_iterate_phdr() the dynamic loader aquires the lock
  _dl_load_write_lock. If the process forks while this lock is held then
  the child process will get a copy of the lock that is already locked
  and because the child does not have the thread that will unlock it
  in the parent process, the lock will never be released and further calls
  into the dynamic loader from the child process will deadlock.

  Run as follows:

  $ make
  $ ./test
  iter start
  .parent end
  child start

  Note "child end" is never printed, the child has deadlocked:

  $ ps | grep test
  21915 pts/0    00:00:00 test

*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <link.h>

volatile int ready = 0;

int callback(struct dl_phdr_info* info, size_t size, void* data)
{
	ready = 1;
	printf(".");
	fflush(stdout);
	sleep(1);
}

void* thread(void* arg)
{
	printf("iter start\n");
	dl_iterate_phdr(callback, NULL);
	printf("iter end\n");
	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t t;
	char* testlib = argc == 2 ? argv[1] : "./testlib.so";

	// start a new thread that will lock _dl_load_write_lock
	// by calling dl_iterate_phdr() and then sleeping in the callback
	t = pthread_create(&t, NULL, thread, NULL);

	// wait for the thread to get aquire the lock
	while(!ready)
		;

	// fork... the child will get a copy of _dl_load_write_lock that is
	// already locked by the thread that called dl_iterate_phdr, because
	// the child doesn't have that thread the lock will never be unlocked.
	// The child will deadlock when it next uses the loader.
	if(fork() == 0)
	{
		// call into the loader and deadlock
		printf("child start\n");
		if(dlopen(testlib, RTLD_NOW) == NULL)
			printf("load failed: %s\n", dlerror());
		printf("child end\n");
	}
	else
		printf("parent end\n");

	return 0;
}

