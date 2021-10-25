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
	int shared_table = shm_open("table", O_CREAT | O_RDWR, 0666);
	ftruncate(shared_table, sizeof(int));
	int* table = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_table, 0);
	sem_t* fill      = sem_open("fill",      O_CREAT, 0666, 0);
	sem_t* available = sem_open("available", O_CREAT, 0666, 2);
	sem_t* mutex     = sem_open("mutex",     O_CREAT, 0666, 1);
	
	// Loop 10 times to produce 10 items in shared memory table with mutual exclusion
	int loop_counter = 10;
	printf("\nProducer ready to create %d items.\n", loop_counter);
	for (int i=1; i<loop_counter; ++i) {
		sem_wait(available);
		int ms = rand() % 2 + 1;
		sleep(ms);
		sem_wait(mutex);
		++(*table);
		sem_post(mutex);
		printf("Item produced, there are now %d item(s) in the table.\n", *table);
		sem_post(fill);
	}
	
	// Cleanup and Exiting Program
	sem_close(fill);
	sem_close(available);
	sem_close(mutex);
	sem_unlink("fill");
	sem_unlink("available");
	sem_unlink("mutex");
	munmap(table, sizeof(int));
	close(shared_table);
	shm_unlink("table");
	printf("Producer cleaned up!\n");
	return 0;
}
