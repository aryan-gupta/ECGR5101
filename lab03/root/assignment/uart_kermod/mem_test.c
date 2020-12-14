#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() 
{ 
	char byte_written,*byte_read,dummy;
  	FILE * MEM;
	size_t result;

 	/* Opening the memory driver */
 	MEM=fopen("/dev/memory","wb");

	/* No buffering for file i/o */
  	setvbuf(MEM,&dummy,_IONBF,1);

  	/* Initializing byte_written */
  	byte_written='a';
	byte_read = (char*) malloc (sizeof(char));
   
  	/* Writing to the memory driver */
  	printf("Byte value written is %c\n",byte_written);
  	fwrite(&byte_written,1,1,MEM);
	fclose (MEM);

	/* Reading from memory driver */
	MEM = fopen("/dev/memory", "rb");
  	result = fread(byte_read,1,1,MEM);	
  	printf("Byte value read is %c\n",*byte_read);
	printf("Number of bytes read is %d\n",result);

  	fclose(MEM);

}
