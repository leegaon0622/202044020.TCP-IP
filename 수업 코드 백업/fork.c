#include <stdio.h>
#include <unistd.h>
int gval=10;

int main(int argc, char *argv[])
{
	pid_t pid;
	int Ival=20;
	gval++, Ival+=5;

	pid=fork();
	if(pid==0)
		gval+=2, Ival+=2;
	else
		gval-=2, Ival-=2;

	if(pid==0)
		printf("Child Proc: [%d, %d] \n", gval, Ival);
	else
		printf("Parent Proc: [%d. %d] \n", gval, Ival);
	return 0;
}
