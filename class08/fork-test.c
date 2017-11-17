#include <unistd.h>
#include <stdio.h>

int
main()
{
        int i;

        for(i=0; i<3; i++) {
                sleep(1);
                printf("fork test%d\n", i);
                fork();
        }
        return(0);
}

