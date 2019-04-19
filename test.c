#include"test.h"
#include<stdio.h>

int add(int a, int b, int c)  %add 
{
	int result = 0;
	printf("%s", "Please enter the number:");
	scanf("%d%d%d", &a, &b, &c);
	result = a + b ;
	return result;
}
