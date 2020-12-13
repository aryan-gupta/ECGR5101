#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  if (argc < 2)
    {
      printf("USAGE: %s loop-iterations\n", argv[0]);
      return 1;
    }

  int delay = atoi(argv[1]);

  struct timeval start, end;
  time_t start_time, cur_time;

  gettimeofday(&start, NULL);
  
  time(&start_time);
  do
  {
	time(&cur_time);
  }
  while((cur_time - start_time) < delay);


  gettimeofday(&end, NULL);

  printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));

 
  return 0;
}
