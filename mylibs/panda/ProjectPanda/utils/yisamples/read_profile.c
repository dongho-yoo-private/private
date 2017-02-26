#include "../yistd.h"

int main(int argc, char** argv)
{
	char buffer[256];
	char value[256];

	//printf("Enter tag...> ");
	//scanf("%s", buffer);
	//
	if (argv[1]==0)
		return 0;
	strcpy(buffer, argv[1]);

	if (yiProfileReadString(buffer, value, "abc.profile")==True)
	{
		printf("value: %s\n", value);
		return 0;
	}
	
	printf("value: no value! \n");
	return 0;
}

