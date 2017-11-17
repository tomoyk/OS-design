#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("test exec: start\n");
    execlp("ls", "ls", NULL);
    printf("test exec: end\n");
}
