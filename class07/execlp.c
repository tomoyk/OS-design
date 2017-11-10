#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("hello");
    execlp("ls", "ls", NULL);
    printf("world");
}
