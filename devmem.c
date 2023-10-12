/* dump /dev/mem with certain offset and length 
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main (void)
{
    int i;
    int fd;
    unsigned int *start;
    off_t offset = 0x200000af0000;
    size_t length_uint32 = 16;  // dump 16 uint32
    // 0xfd6e0000 EBG 13.4
	// 0xfdad0000 LBG

    fd = open ("/dev/mem", O_RDWR);
    if (fd < 0)
    {
        printf("cannot open /dev/mem.");
        return -1;
    }

    // void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    //map physical memory 0-length bytes
	// PROT_WRITE
    start = (unsigned int *)mmap(0, 0x10000, PROT_READ, MAP_SHARED, fd, offset);
    // 0x201000000000
    // 0xfdad0000
    // 0x9f000 to match w/ /proc/iomem
    if(start < 0)
    {
        printf("mmap failed.");
        return -1;
    }

    // dump in uint32
    for (i = 0; i < length_uint32; i++)
    {
        printf("%lx: %08x\n", offset + i*sizeof(start[0]), start[i]);
    }
    for (i = 0x100; i < 0x100+length_uint32; i++)
    {
        printf("%lx: %08x\n", offset + i*sizeof(start[0]), start[i]);
    }

    munmap(start, 0x10000);
    close(fd);
    return 0;
}
