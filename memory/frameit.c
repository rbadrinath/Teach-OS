// Modified from 
// http://fivelinesofcode.blogspot.com/2014/03/how-to-translate-virtual-to-physical.html
// Added some code to find out more about the physical frame
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#define PAGEMAP_ENTRY 8
#define GET_BIT(X,Y) (X & ((uint64_t)1<<Y)) >> Y
#define GET_PFN(X) X & 0x7FFFFFFFFFFFFF

const int __endian_bit = 1;
#define is_bigendian() ( (*(char*)&__endian_bit) == 0 )

int i, c, pid, status;
unsigned long virt_addr; 
uint64_t read_val, file_offset;
char path_buf [0x100] = {};
FILE * f;
char *end;

int read_pagemap(char * path_buf, unsigned long virt_addr);
int read_frameinfo(uint64_t pfn);

int main(int argc, char ** argv){
   //printf("%lu\n", GET_BIT(0xA680000000000000, 63));
   //return 0;
   if(argc!=3){
      // printf("Argument number is not correct!\n pagemap PID VIRTUAL_ADDRESS\n");
      printf("Argument number is not correct!\n"
	     "Usage:   %s <pid> <virtual_address>\n",argv[0]);
      return -1;
   }
   if(!memcmp(argv[1],"self",sizeof("self"))){
      sprintf(path_buf, "/proc/self/pagemap");
      pid = -1;
   }
   else{
         pid = strtol(argv[1],&end, 10);
         if (end == argv[1] || *end != '\0' || pid<=0){ 
            printf("PID must be a positive number or 'self'\n");
            return -1;
            }
       }
   virt_addr = strtol(argv[2], NULL, 16);
   if(pid!=-1)
      sprintf(path_buf, "/proc/%u/pagemap", pid);
   long pfn=read_pagemap(path_buf, virt_addr);
   if (pfn > 0 )
	   read_frameinfo(pfn);
   return 0;
}

int read_pagemap(char * path_buf, unsigned long virt_addr){
   printf("Big endian? %d\n", is_bigendian());
   f = fopen(path_buf, "rb");
   if(!f){
      printf("Error! Cannot open %s\n", path_buf);
      return -1;
   }
   
   //Shifting by virt-addr-offset number of bytes
   //and multiplying by the size of an address (the size of an entry in pagemap file)
   file_offset = virt_addr / getpagesize() * PAGEMAP_ENTRY;
   printf("Vaddr: 0x%lx, Page_size: %d, Entry_size: %d\n", virt_addr, getpagesize(), PAGEMAP_ENTRY);
   printf("Reading %s at 0x%llx\n", path_buf, (unsigned long long) file_offset);
   status = fseek(f, file_offset, SEEK_SET);
   if(status){
      perror("Failed to do fseek!");
      return -1;
   }
   errno = 0;
   read_val = 0;
   unsigned char c_buf[PAGEMAP_ENTRY];
   for(i=0; i < PAGEMAP_ENTRY; i++){
      c = getc(f);
      if(c==EOF){
         printf("\nReached end of the file\n");
         return 0;
      }
      if(is_bigendian())
           c_buf[i] = c;
      else
           c_buf[PAGEMAP_ENTRY - i - 1] = c;
      printf("[%d]0x%x ", i, c);
   }
   for(i=0; i < PAGEMAP_ENTRY; i++){
      //printf("%d ",c_buf[i]);
      read_val = (read_val << 8) + c_buf[i];
   }
   printf("\n");
   printf("Result: 0x%llx\n", (unsigned long long) read_val);
   //if(GET_BIT(read_val, 63))
   if(GET_BIT(read_val, 63))
      printf("PFN: 0x%llx\n",(unsigned long long) GET_PFN(read_val));
   else
      printf("Page not present\n");
   if(GET_BIT(read_val, 62))
      printf("Page swapped\n");
   fclose(f);

   return GET_PFN(read_val);
}

int read_frameinfo(uint64_t pfn){
   unsigned char c_buf[PAGEMAP_ENTRY];
   if(GET_BIT(read_val, 63)) {
	   // if there is a frame
	   uint64_t pfn=GET_PFN(read_val);
	   f=fopen("/proc/kpageflags","rb");
	   if( ! f ){
		   perror("Error opening kpageflags:");
		   exit(1);
	   }
	   // get the flags info for the PFN
	   uint64_t loc=8*pfn; // because file has 64 bits of info for each PFN
	   int status = fseek(f, loc, SEEK_SET );
   	   if(status){
     		perror("Failed to do fseek!");
      		exit(1);
	   }
	   uint64_t fread_val = 0;
	   // lets reuse c_buf luckily PAGEMAP_ENTRY is also 8
	   for(i=0; i < PAGEMAP_ENTRY; i++){
	      c = getc(f);
	      if(c==EOF){
	         printf("\nReached end of the file\n");
	         return 0;
	      }
	      if(is_bigendian())
	           c_buf[i] = c;
	      else
	           c_buf[PAGEMAP_ENTRY - i - 1] = c;
	      printf("[%d]0x%x ", i, c);
	   }
	   for(i=0; i < PAGEMAP_ENTRY; i++){
	      //printf("%d ",c_buf[i]);
	      fread_val = (fread_val << 8) + c_buf[i];
	   }
	   printf("\n");
	   printf("PageFlags: 0x%llx\n", (unsigned long long) fread_val);
   }
}
