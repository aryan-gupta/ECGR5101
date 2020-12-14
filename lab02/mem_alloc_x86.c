#include <stdlib.h>
#include <stdio.h>

// extern char *heap_end = 0;

int main() {
	char c;
	char *ptr = NULL;
	char *p = NULL;
	size_t alloc_size = 1;
	do {
  		c = getchar();
  		printf("%d: %c\n", c, c);

  		ptr = realloc(ptr, alloc_size);
  		if(ptr == NULL) {
   			puts("Out of memory!\nProgram halting.");
   		for(;;);
  		} else {
   			printf("new alloc of %d bytes at address 0x%p\n", alloc_size, (unsigned int)ptr);
   			alloc_size <<= 1;
  		}
 	} while (1);
}
