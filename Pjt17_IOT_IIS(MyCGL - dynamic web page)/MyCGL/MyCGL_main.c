// MYCGL_main.c

#include <stdio.h>
#include "MyCGL_web.h"

int main()
{
	char data[64];

	sprintf_s(data, 64, "Current temperature is 10 degree.");
	MyCGL_web(data);

	return 0;
}