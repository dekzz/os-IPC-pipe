#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define MSGSIZE 20

int getRand(int max)
{
    int num = rand() % max;
    return num;
}

int main(void)
{
    srand(time(NULL));
    char op, oprt[4] = {'+', '-', '*', '/'};
    int n1, n2, r, ur, i, grade = 0;

    int pfd1[2];
    int pfd2[2];
    char message[MSGSIZE];
    char result[MSGSIZE];

    if (pipe(pfd1) == -1)
  	exit(1);

    if (pipe(pfd2) == -1)
	exit(1);

    switch (fork()) {
		case -1:
			exit(1);
		case 0:
			close(pfd1[1]); 		/* close the writing side  */
			close(pfd2[0]); 		/* close the reading side  */
			dup2(pfd1[0], 0);		/* make read side of pipe a standard input */
			dup2(pfd2[1], 1);		/* make write side of pipe a standard output */
			//dup2(pfd1[0], STDIN_FILENO);   /* make read side of pipe a standard input */
			//dup2(pfd2[1], STDOUT_FILENO);  /* make write side of pipe a standard output */

			execl("bc", "bc", NULL);

			//exit(0);
		default:
			//close(pfd1[0]); /* closes the reading side */
			//close(pfd2[1]); /* closes the writing side */

			printf("\n-----------------------\n");
		        for(i = 0; i < 5; i++)
		        {
		            n1 = getRand(10);
		            n2 = getRand(10);
		            op = oprt[getRand(4)];

		   	    sprintf(message, "%i %c %i", n1, op, n2);

		            printf("%s = ", message);

			    (void) write(pfd1[1], message, strlen(message) + 1);

			    scanf("%i", &ur);

			    (void) read(pfd2[0], result, MSGSIZE);

			    r = atoi(result);
	   	            if(ur == r)
		            {
		                printf("Correct! \n");
	                        grade++;
		            }
		            else
			    {
	     	                printf("Incorrect, answer is %i! \n", r);
			    }

		            printf("-----------------------\n");
       			}

		        printf("\n-----------------------\n");
		        printf("Your final grade is %i! \n", grade);
		        printf("-----------------------\n\n");

			wait(NULL);
		}

		exit(0);

    return 0;
}
