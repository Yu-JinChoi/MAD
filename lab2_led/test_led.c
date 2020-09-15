#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
				int fd, ret;
				char buf = atoi(argv[1]);

				fd=open("/dev/csemad_led", O_RDWR);

				if(fd==-1) return -1;

				write(fd, &buf ,1);

				read(fd, &buf, 1);

				printf("Current LED Value : %d\n", buf);

				ret=close(fd);

				return ret;
}
