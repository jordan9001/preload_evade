#include "write.h"

__attribute__((constructor))
void preload_init(void)
{
    litwrite("preload.c: preload_init");
}

__attribute__((constructor))
void init(void)
{
    litwrite("preload.c: init");
}
