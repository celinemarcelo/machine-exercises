#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>

int main(void) {
	pid_t pid;												//process id variable
	int i = 0;												//counter (will handle the delay after the printing of the pid's of each process)
	int rand_no = 0;										//random number to be passed by the parent and received by the child
	int fifofd = 0;											//pipe for communication involving rand_no
	int child_done = 0;										//pipe for termination signal from child process
	char bufin[10] = {};									//buffer for child process
	char bufout[10] = {};									//buffer for parent process
	char parent[2] = {};									//holder of child_done's return value
	time_t t0;												//will always be set to time(NULL), which is the number of seconds since Epoch
	struct tm *tm_now;										//struct containing the details of time based on t0

	pid = fork();											//forking

	mkfifo("fifofd", S_IRUSR | S_IWUSR);					//making the fifo for rand_no communication
	fifofd = open("fifofd", O_RDWR);

	mkfifo("child_done", S_IRUSR | S_IWUSR);				//making the fifo for termination signal communication
	child_done = open("child_done", O_RDWR);


	if (pid > 0) {											//parent process

		t0 = time(NULL);
		tm_now = localtime(&t0);

		printf("[%.2d:%.2d:%.2d]Parent with pid %d\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());	//printing of parent pid

		do {
			if (!i) {										//sleep only 2 seconds for first iteration (after printing of pid's)
				sleep(2);
			} else {
				sleep(4);									//sleep 4 seconds otherwise (interval between the ouput of rand_no is supposed to be 5 seconds, but child sleeps 1 second)
			}

			i++;
			srand(time(NULL));								//seeding the rand() function
			rand_no = rand() % 10 + 1;						//generation of rand_no

			sprintf(bufout, "%d", rand_no);

			t0 = time(NULL);
			tm_now = localtime(&t0);

			printf("[%.2d:%.2d:%.2d]Parent(%d): Sent %s\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid(), bufout);

			write(fifofd, bufout, sizeof(bufout));			//write rand_no to fifofd, which will be read by child process
			read(child_done, parent, sizeof(parent));		//verify if child process is about to be terminated

		} while (atoi(parent) != 0);

		t0 = time(NULL);									//at this point, child_done should contain 0
		tm_now = localtime(&t0);							//however, the child process sets it to 1 for error handling

		printf("[%.2d:%.2d:%.2d]Parent(%d): Waiting for child process to terminate\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());

		read(child_done, parent, sizeof(parent));			//verify if child process has terminated

		if (!strcmp(parent, "1")) {
			sleep(1);										//required delay

			t0 = time(NULL);
			tm_now = localtime(&t0);

			printf("[%.2d:%.2d:%.2d]Parent(%d): Terminated\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());

		} else {
			printf("[%.2d:%.2d:%.2d]Parent(%d): Unknown signal\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());	//handles if child_done was unchanged
		}

		close(fifofd);										//cleaning up
		close(child_done);
		unlink("fifofd");
		unlink("child_done");

	} else if (pid == 0) {

		t0 = time(NULL);
		tm_now = localtime(&t0);

		printf("[%.2d:%.2d:%.2d]Child with pid %d\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());	//printing of child pid

		while (atoi(bufin) != 10) {
			read(fifofd, bufin, sizeof(bufin));				//reading rand_no from the parent process

			sleep(1);										//required delay

			if (atoi(bufin) != 10) {
				t0 = time(NULL);
				tm_now = localtime(&t0);

				printf("[%.2d:%.2d:%.2d]Child(%d): Received %s\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid(), bufin);
				write(child_done, "1", 2);					//tells the parent that the child has not yet received 10
			} else {
				t0 = time(NULL);
				tm_now = localtime(&t0);

				printf("[%.2d:%.2d:%.2d]Child(%d): Received %s\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid(), bufin);
				printf("[%.2d:%.2d:%.2d]Child(%d): Child process is terminating\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());
				write(child_done, "0", 2);					//tells the parent that the child has receieved 10, and thus, is terminating

				sleep(1);									//required delay

				t0 = time(NULL);
				tm_now = localtime(&t0);

				write(child_done, "1", 2);					//the child sets child_done to 1 to handle errors

				printf("[%.2d:%.2d:%.2d]Child(%d): Terminated\n", tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, getpid());

				close(fifofd);								//cleaning up
				close(child_done);
				exit(0);
			}

		}
	} else {
		printf("Error\n");									//for invalid pid (-1)
	}

	exit(0);
}