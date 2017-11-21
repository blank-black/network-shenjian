#include <stdio.h>

int read_file()
{
	FILE *fp;
	fp=fopen("/etc/1551265.conf", "r");
	char buff[50]="";
	fscanf(fp, "%s", buff);
	int num=0;

	if(buff[13]=='0'&&buff[13]=='0'&&buff[11]=='1')
		num=100;
	else if(!buff[13]&&buff[12])
		num=(buff[11]-'0')*10+buff[12]-'0';
	else if(!buff[12]&&buff[11]>='5')
		num=buff[11]-'0';
	else
		num=5;
	return num;
}