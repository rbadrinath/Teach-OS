// Modified from 
// http://fivelinesofcode.blogspot.com/2014/03/how-to-translate-virtual-to-physical.html
// Added some code to find out more about the physical frame
// best to see this along with https://www.kernel.org/doc/Documentation/vm/pagemap.txt
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
unsigned long virt_addr=0; 
uint64_t read_val, file_offset;
char path_buf [0x100] = {};
FILE * f;
char *end;

int read_pagemap(char * path_buf, unsigned long virt_addr);
int read_frameinfo();


int is_quit(char *s){
	if(strcmp(s,"N")==0 || strcmp(s,"n")==0 || strcmp(s,"quit")==0)
		return 1;
	return 0;
}
int print_usage(char *cmd){
	printf("Usage:   %s <pid|self> <virtual_address> (assumes hexadecimal)\n",cmd);
}
int main(int argc, char ** argv){
   char vaddr_string[20]="0x0";

   printf(" Make sure you are running as sudo***\n");
   printf(" Hit enter once you have the pmap\n");
   print_usage(argv[0]);



   //getchar() is so that you can pmap and check the memory addresses
   getchar();
   if(argc!=3){
      printf("Incorrect aruments!\n");
      print_usage(argv[0]);
      return 1;
   }
   char *pid_string=argv[1];



   while( 1 ) {  // repeat this loop for each virt_addr

      // set pathbuf to the correct file in /proc
      if(!memcmp(argv[1],"self",sizeof("self"))){
         sprintf(path_buf, "/proc/self/pagemap");
         pid = -1;
      }
      else{
            pid = strtol(argv[1],&end, 10);
            if (end == argv[1] || *end != '\0' || pid<=0){ 
               printf("PID must be a positive number or 'self'\n");
               return 1;
               }
            sprintf(path_buf, "/proc/%u/pagemap", pid);
      }

      // Determime the virt_addr from the argument (first iter) or the input string in vaddr_string (other iters)
      if (virt_addr==0) 
            // get the virtual address from the argument
            virt_addr = strtol(argv[2], NULL, 16);
      else
   	    // get the virtual address from the user 
            virt_addr = strtol(vaddr_string,NULL,16);


      // read the pagemap file and get the pfn.. also print some useful info along the way
      long pfn=read_pagemap(path_buf, virt_addr);
      // if it is a valid pfn, print some more frame information
      if (pfn > 0 )
           read_frameinfo();
      
      //  Now ask for another virt_addr
      printf("\n\n\n Give another address if you wish or just N/n/quit :");
      scanf("%s",vaddr_string);
      if ( is_quit(vaddr_string))
           exit(0);
   }
   return 0;
}

int read_pagemap(char * path_buf, unsigned long virt_addr){
   printf("Big endian? %d\n", is_bigendian());
   printf("Opening the file %s\n",path_buf);
   f = fopen(path_buf, "rb");
   if(f == NULL){
      printf("Error! Cannot open %s\n", path_buf);
      return -1;
   }
   
   printf("Using these parameters to seek into the file:\n");
   printf("    Vaddr: 0x%lx, Page_size: %d, Entry_size: %d\n", virt_addr, getpagesize(), PAGEMAP_ENTRY);

   //Shifting by virt-addr-offset number of bytes
   //and multiplying by the size of an address (the size of an entry in pagemap file)
   file_offset = virt_addr / getpagesize() * PAGEMAP_ENTRY;

   printf("Seeking to 0x%llx\n", (unsigned long long) file_offset);
   status = fseek(f, file_offset, SEEK_SET);
   if(status){
      perror("***Failed to do fseek!");
      return -1;
   }
   errno = 0;
   read_val = 0;
   unsigned char c_buf[PAGEMAP_ENTRY];
   printf("Bytes of data read: ");
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
      printf(" [%d]0x%x ", i, c);
   }
   for(i=0; i < PAGEMAP_ENTRY; i++){
      //printf("%d ",c_buf[i]);
      read_val = (read_val << 8) + c_buf[i];
   }
   printf("\n");
   printf("Resultant read value: 0x%llx\n", (unsigned long long) read_val);
   
   printf("Now looking for important PFN information:....\n");
   if(GET_BIT(read_val, 63)) {
      printf("   -Page is present so a frame is assigned :\n");
      printf("       PFN: 0x%llx\n",(unsigned long long) GET_PFN(read_val));
   }
   else
      printf("   -Page is not present so no frame is assigned\n");

   if(GET_BIT(read_val,55))
      printf("   -Page dirty\n");
   else
      printf("   -Page clean\n");
   if(GET_BIT(read_val, 62))
      printf("   -Page swapped\n");
   if(GET_BIT(read_val, 61))
      printf("   -Page is file-mapped\n");
   else
      printf("   -Page is not file-mapped\n");
   fclose(f);

   return GET_PFN(read_val);
}

int read_frameinfo(){
   unsigned char c_buf[PAGEMAP_ENTRY];
   if(GET_BIT(read_val, 63)) {
	   // if there is a frame
	   uint64_t pfn=GET_PFN(read_val);
	   printf("Opening /proc/kpageflags...\n");
	   f=fopen("/proc/kpageflags","rb");
	   if( ! f ){
		   perror("Error opening kpageflags:***");
		   exit(1);
	   }

	   // get the flags info for the PFN
	   printf("Seeking for the location in the file with the flags...\n");
	   uint64_t loc=8*pfn; // because file has 64 bits of info for each PFN
	   int status = fseek(f, loc, SEEK_SET );
   	   if(status){
     		perror("Failed to do fseek!***");
      		exit(1);
	   }
	   uint64_t fread_val = 0;

	   // lets reuse c_buf luckily PAGEMAP_ENTRY is also 8
	   printf("Reiading flags 8 bytes... ");
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
   } else {
	   printf("It has no frame!\n");
   }
}
