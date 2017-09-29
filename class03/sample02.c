#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

main()
{
        int fd;
        int count;
        char buf[1024];

        fd = open("/proc/version", O_RDONLY); // 権限のチェックを行っている

        if (fd < 0) exit(0); // error -> end

        count = read(fd, buf, 1024); 

        if (count <= 0) exit(0);

        write(1, buf, count);

        close(fd);
}

