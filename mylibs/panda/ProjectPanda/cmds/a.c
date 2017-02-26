#include <unistd.h>

main()
{
	int fd[2];
	int fd2[2];
	char buffer[10];
	int i=0;

	pipe(fd);
	pipe(fd2);

	printf("%d, %d, %d, %d\n", fd[0], fd[1], fd2[0], fd2[1]);

	for (i=0; i<1000; i++)
	{
		int n;
		n = write(fd[1], "alksdlaksjdf", 4);
		printf("write: %d\n", n);
		n=read(fd[0], buffer, 10);
		printf("read: %d\n", n);
	}

	close(fd[0]);
	close(fd[1]);
	close(fd2[0]);
	close(fd2[1]);
}

