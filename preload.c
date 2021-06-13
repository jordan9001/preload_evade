#include "write.h"

__attribute__((constructor))
void init(void)
{
    litwrite("PRELOAD_INIT");
}

