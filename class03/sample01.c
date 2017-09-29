#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(int argc, char** argv)
{
	int fd;
    
    // printf("%s\n", argv[argc-1]);

    // exit(0);

	while(argc > 1) {
		// fd = open(argv[argc-1], "r");
		fd = open(argv[argc-1], O_RDONLY);
		if (fd < 0) {
            // printf("argv[argc] = %d", argv[argc]);
			printf("%s が見つかりません\n", argv[argc-1]);
			continue;
		}
		argc--;
		char buf[4096];
		while(1) {
			int count = read(fd, buf, 4096);
			if (count <= 0) break;
			write(1, buf, count);
		}
	}
}

