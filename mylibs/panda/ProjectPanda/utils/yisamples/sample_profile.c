#include "../yistd.h"

// -------------------------------
int main(int argc, char** argv)
{
	/*char** p;
	int cnt=0;
	p = yistrSplit("A= yoo", "=", &cnt);
	
	printf("cnt:%d\n", cnt);//p[1]);
	printf("1: %s\n", yistrTrim(p[0], False));
	printf("2: %s\n", yistrTrim(p[1], False));*/

	yiProfileWriteString("dongho", "yoo", "abc.profile");	
	yiProfileWriteString("A", "1", "abc.profile");	
	yiProfileWriteString("ABC", "2", "abc.profile");	
	yiProfileWriteString("Japan", "nihon", "abc.profile");	
	yiProfileWriteString("xxxx", "kkkk", "abc.profile");	
}

