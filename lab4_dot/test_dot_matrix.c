#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "dot_matrix_font.h"

int main(int argc, char *argv[])
{
				int dev=open("/dev/csemad_dot_matrix", O_RDWR);
				int number = atoi(argv[1]);
				
				if(dev<0)
				{
								printf("Failed to open!\n");
								return -1;
				}

				write(dev, dot_matrix_font[number], sizeof(dot_matrix_font[number]));

				close(dev);
				return 0;
}
