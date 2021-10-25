Producer generates items and puts the items into a shared memory table while the consumer picks up the items. The table can only hold 2 items at the same time. When the table is full, the producer will wait, and when there are no items, the consumer will wait. Semaphores are used to synchronize the producer and consumer. Pthreads are used.

See documentation.txt for information on how to compile and run the program.
