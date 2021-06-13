#include "write.h"

unsigned int la_version(unsigned int current)
{
    litwrite("main.c: la_version");
    return current;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    litwrite("main.c: main");
}

__attribute__((constructor))
void init(void)
{
    litwrite("main.c: init");
}
