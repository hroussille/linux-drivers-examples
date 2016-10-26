#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stropts.h>
#include <fcntl.h>
#include <unistd.h>
#include "../module_ioctl.h"

int main(int argc, char const *argv[])
{
	int fd;

	fd = open("/dev/example_device", O_RDWR);

	if (fd < 0) {
		printf("%s\n", strerror(errno));
		return -1;
	}

	ioctl(fd, EXAMPLE_IOCTL, NULL);
	ioctl(fd, EXAMPLE_IOCTL_W, NULL);
	ioctl(fd, EXAMPLE_IOCTL_R, NULL);
	ioctl(fd, EXAMPLE_IOCTL_WR, NULL);

	if (close(fd) != 0) {
		printf("%s\n", strerror(errno));
		return -1;
	}

	return 0;
}
