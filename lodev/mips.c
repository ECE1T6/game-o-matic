#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *mips(void *args) {
  unsigned long long *mips = (unsigned long long*) args;
  while (1) {
    *mips += 1;
  }

  return NULL;
}


int main (void){
	unsigned long long armmips = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, mips, &armmips);
	usleep(1000000);
	pthread_cancel(tid);
	armmips *= 0.00001;
	//printf("mips: %llu\n", armmips); //uncomment to print results to terminal.
	return (int) armmips;
}
