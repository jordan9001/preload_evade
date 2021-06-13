#include "write.h"

unsigned int la_version(unsigned int current)
{
    litwrite("MAIN_AUDIT_VER");
    return current;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    litwrite("MAIN");
}

__attribute__((constructor))
void init(void)
{
    litwrite("MAIN_INIT");
}
