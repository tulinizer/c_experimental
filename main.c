#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* For timing measurements */
#include <sys/time.h>
#include <time.h>

#define TOTALQUEST 10
#define ANSWERQUEST 5
#define QUESTSLEEP 3000000

int main(int argc, char **argv)
{
	srand(time(NULL));

	/* Question number */
	int quest_num, right = 0;
	/* String of number */
	char quest[2] = { '0', '\0' };
	/* Question array */
	int rand_array[ANSWERQUEST];
	/* Child */
	pid_t pid;
	/* Counter */
	int i, j;
	/* If random number already exist */
	int exist = 0;
	/* Exit status*/
	int status;

	/* Save random numbers into an array */
	for (i = 0; i < ANSWERQUEST; i++)
	{
		/* Random number generate */
		quest_num = rand() % TOTALQUEST;
		exist = 0;

		for (j = 0; j < i; j++)
			if (rand_array[j] == quest_num)
				exist = 1;

		/* If number didnt exist */
		if (!exist)
			rand_array[i] = quest_num;
		else
			i--;
	}

	for (i = 0; i < ANSWERQUEST; i++)
	{
		/* Quest int to string*/
		quest[0] = '0' + rand_array[i];

		/* Who wants to be a millionaire */
		if(i>0) usleep(QUESTSLEEP);

		/* Clear the screen so 1 question per page */
		system("clear");

		printf("-- Question No : %d\n",i+1);
		printf("-- Current Status : %d / %d\n",right,i);
		printf("- Parent process with id %d selects the question %d\n",getpid(),rand_array[i]+1);

		/* Switch fork */
		switch (pid = fork())
		{
			case -1:
			{
				/* if fork() error */
				perror("fork");
				exit(1);
			}break;

			case 0:
			{
				printf("- Child process with id %d asks the question: \n", getpid());
				if (execl("./contest", "contest", quest, NULL ) == -1)
					fprintf(stderr, " ** Oops,an exec error! **");
			}break;

			default:
			{
				wait(&status);
				if (WIFEXITED(status) && !WEXITSTATUS(status))
					right++;
			}break;
		}
	}

	printf("\nNumber of right questions: %d", right);
	printf("\nNumber of wrong questions: %d\n", 5 - right);
	printf("Thanks for playing!..\n");

	exit(EXIT_SUCCESS);
}

