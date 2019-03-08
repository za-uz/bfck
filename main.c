#include <stdio.h>
#include <stdlib.h>
#include "bfck.h"

int main()
{
	char *code = "[++++++++++,.----------]";
	
	run(code, stdin, stdout);
	return EXIT_SUCCESS;
}

