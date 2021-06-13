#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

int modify_elf(char* base)
{
    Elf64_Ehdr* hdr = (Elf64_Ehdr*)base;
    Elf64_Phdr* phdr;
    Elf64_Dyn* dyn;
    void* end;
    int i;

    // not using lots of consitancy checks that we should here

    // get to dynamic section to clear the flag

    phdr = (Elf64_Phdr*)(base + hdr->e_phoff);
    for (i = 0; i < hdr->e_phnum; i++) {
        if (phdr[i].p_type != PT_DYNAMIC) {
            continue;
        }

        dyn = (Elf64_Dyn*)(base + phdr[i].p_offset);
        end = (void*)(((char*)dyn) + phdr[i].p_filesz);

        for (; (void*)dyn < end; dyn++) {
            //printf("%016lx %016lx\n", dyn->d_tag, dyn->d_un.d_ptr);
            if (dyn->d_tag == DT_FLAGS_1) {
                printf("Found DT_FLAGS_1 = %16lx\n", dyn->d_un.d_ptr);
                dyn->d_un.d_ptr = 0;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int fd;
    struct stat st;
    char* base;

    if (argc < 2) {
        printf("Require elf file as argument");
        exit(-1);
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        printf("Error opening %s: %m\n", argv[1]);
        exit(-1);
    }

    if (fstat(fd, &st)) {
        printf("Error stating %s: %m\n", argv[1]);
        exit(-1);
    }

    base = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (base == (char*)-1) {
        printf("Error mapping %s: %m\n", argv[1]);
        exit(-1);
    }

    if (modify_elf(base)) {
        printf("Error modifying %s\n", argv[1]);
        exit(-1);
    }

    munmap(base, st.st_size);
    close(fd);

    printf("Modified %s\n", argv[1]);
    return 0;
}
