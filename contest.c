#include <stdio.h>
#include <stdlib.h>
/* for puts() */
#include <string.h>
/* for usleep() function*/
#include <unistd.h>
/* for toupper() function */
#include <ctype.h>
/* for threads*/
#include <pthread.h>

#define MAXLINE 100
#define QUESTEND '?'
#define QUESTSLEEP 1000000
#define TIMESLEEP 1000000
#define TIME 20

void *Input(void *t);
void draw();

/* global, so threads can reach*/
char youranswer='_';

int main(int argc, char **argv)
{
	/* input file*/
	FILE *file = fopen("questions.txt", "r");
	/* Question lines*/
	char question[MAXLINE];
	/* which question */
	int quest_num=atoi(argv[1])+1;
	/* real answer, start*/
	char answer[2],qnum[2]="1";
	/* YES, it is against time */
	int time=TIME;
	pthread_t thread;

	/* read line until we reach the quest_num*/
	while (atoi(qnum)!=quest_num)
		fgets(qnum,MAXLINE,file);

	printf("------------------------------------------------------\n");
	/* read line until we reach end of the question (?) */
	while (question[0]!=QUESTEND)
	{
		fgets(question,MAXLINE,file);
		/* Who wants to be a millionaire effect*/
		usleep(QUESTSLEEP);
		puts(question);
	}
	/* read the answer line*/
	fgets(answer,2,file);

	/* we are done with our file */
	fclose(file);

	/* users answer */
	printf("Your answer : \n");

	while (1)
	{
		/* This thread will wait for the input */
		pthread_create(&thread,NULL,Input,NULL);
		/* Who wants to be a millionaire effect on answers*/
		usleep(TIMESLEEP);

		/* Right? */
		if (toupper(youranswer)==answer[0])
		{
			/* Wait for the thread */
			pthread_join(thread,NULL);
			printf("Your answer : %c",toupper(youranswer));
			printf("\n\t\t** ! RIGHT ! **\n");
			/* I could use a loop for printing this but holding down is easier */
			printf("------------------------------------------------------\n");
			/* That was fun, yeey! ---------------------------- :)  */
			exit(0);
		}

		if (toupper(youranswer)!=answer[0] && youranswer!='_')/* Wrong? */
		{
			pthread_join(thread,NULL);
			printf("Your answer : %c",toupper(youranswer));
			printf("\n\t\t** ! WRONG ! **\n");
			/* This time i just copied it. */
			printf("------------------------------------------------------\n");
			exit(1);
		}

		/* Cancel, if we dont cancel, 'while' will open a new thread which we don't need */
		pthread_cancel(thread);
		/* Countdown */
		draw(time);
		if (--time<0)
		{
			/* Time's up! */
			pthread_join(thread,NULL);
			printf("\n** ! TIME IS UP ! **\n");
			printf("------------------------------------------------------\n");
			exit(1);
		}
	}
}
/* For warning the user elegantly */
void draw(time)
{
	printf("|%d| ",time);
}

void *Input(void *t)
{
	scanf("%c",&youranswer);
	pthread_exit(NULL);
}
