// Much of this code was generated from Gemini AI and then updated for purposes of this experiment
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define FILE_NAME "for_fault_experiment"

// Read Time-Stamp Counter with serialization
static inline uint64_t rdtsc_start(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ (
        "lfence\n\t"          // Serialize: wait for prior instructions to finish
        "rdtsc\n\t"           // Read 64-bit timestamp counter into EDX:EAX
        : "=a" (lo), "=d" (hi)
        :
        : "memory"
    );
    return ((uint64_t)hi << 32) | lo;
}

static inline uint64_t rdtsc_end(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ (
        "rdtscp\n\t"          // Read counter and wait for prior instructions
        "lfence\n\t"          // Prevent subsequent instructions from executing early
        : "=a" (lo), "=d" (hi)
        :
        : "rcx", "memory"     // rdtscp also writes processor ID to ECX/RCX
    );
    return ((uint64_t)hi << 32) | lo;
}
static inline void flush_cpu_cache(volatile void *addr) {
    __asm__ __volatile__ (
        "clflush (%0)\n\t"    // Evict the 64-byte cache line containing addr
        "mfence\n\t"          // Wait for the flush to complete before continuing
        :
        : "r" (addr)
        : "memory"
    );
}

int main(void) {
    long page_size = sysconf(_SC_PAGESIZE);

    // 1. Reserve virtual address space (no physical page allocated yet!)
    char *page = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (page == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 2. Measure the first access (triggers a minor page fault - must allocate physical page)
    uint64_t start_fault = rdtsc_start();
    page[0] = 42; 
    uint64_t end_fault = rdtsc_end();

    // 3. Measure a second access to the same page (no page fault, cache hit)
    uint64_t start_hit = rdtsc_start();
    page[1] = 43;
    uint64_t end_hit = rdtsc_end();

    // 4. Measure a third access to the same page and uncached data (no page fault, cache miss)
	flush_cpu_cache(page); // explicitly uncache the data
    uint64_t start_cd = rdtsc_start();
    int t2 = page[0];
    uint64_t end_cd = rdtsc_end();


    munmap(page, page_size);

    // 5. Measure a fourth access to a un-present file backed (page fault, page on disk only)
	int fd = open(FILE_NAME,O_RDONLY,0);
	if (fd <= 2 ) {
		perror("open file failed\n");
		exit(1);
	}
	// - minor experiment - try with and without posix_fadvise
	if(posix_fadvise(fd,0,sysconf(_SC_PAGESIZE),POSIX_FADV_DONTNEED)!=0){
		perror("posix_fadvise\n");
		exit(1);
	}


	char * mem=mmap(NULL,sysconf(_SC_PAGESIZE),PROT_READ,MAP_PRIVATE,fd,0);
	if (! mem ) {
		perror("mmap failed\n");
		exit(1);
	}
    uint64_t start_dsk = rdtsc_start();
    char t3 = mem[0];
    uint64_t end_dsk = rdtsc_end();

    printf("Page fault access cycles   : %lu\n", end_fault - start_fault);
    printf("Cached memory hit cycles   : %lu\n", end_hit - start_hit);
    printf("Uncached memory hit cycles : %lu\n", end_cd - start_cd);
    printf("page from disk cycles      : %lu\n", end_dsk - start_dsk);
	
    return 0;
}
