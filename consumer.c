#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

int main() {
	// Startup and variable declarations
	int shared_file  = shm_open("table", O_RDWR, 0666);
	int* table       = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_file, 0);
	sem_t* fill      = sem_open("fill",      O_CREAT, 0666, 0);
	sem_t* available = sem_open("available", O_CREAT, 0666, 2);
	sem_t* mutex     = sem_open("mutex",     O_CREAT, 0666, 1);
	
	// Loop 10 times to consume 10 items from shared memory table with mutual exclusion
	int loop_counter = 10;
	printf("\nConsumer ready to receive %d items.\n", loop_counter);
	for (int i=1; i<loop_counter; ++i) {
		sem_wait(fill);
		int ms = rand() % 2 + 1;
		sleep(ms);
		sem_wait(mutex);
		--(*table);
		sem_post(mutex);
		printf("Item consumed, there are now %d item(s) in the table.\n", *table);
		sem_post(available);
	}
	
	// Cleanup and Exiting Program
	sem_close(fill);
	sem_close(available);
	sem_close(mutex);
	sem_unlink("fill");
	sem_unlink("available");
	sem_unlink("mutex");
	munmap(table, sizeof(int));
	close(shared_file);
	shm_unlink("table");
	printf("Consumer cleaned up!\n");
	return 0;
}
