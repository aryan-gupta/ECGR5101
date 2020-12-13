/* Demos how to set priorities for threads. Needs root acess to run */

#include <pthread.h>
#include <stdio.h>
#define PRIO_GROUP 2

pthread_t hpt[PRIO_GROUP];
pthread_t mpt[PRIO_GROUP];
pthread_t lpt[PRIO_GROUP];

pthread_barrier_t mybarrier;

void *highprior_thread(void *arg){

pthread_t thread_id = pthread_self();
struct sched_param param;
int priority, policy, ret;

pthread_barrier_wait(&mybarrier);

ret = pthread_getschedparam (thread_id, &policy, &param);
priority = param.sched_priority;	
printf("%d \n", priority);


return NULL;
}

void *middleprior_thread(void *arg){


pthread_t thread_id = pthread_self();
struct sched_param param;
int priority, policy, ret;

pthread_barrier_wait(&mybarrier);

ret = pthread_getschedparam (thread_id, &policy, &param);
priority = param.sched_priority;	
printf("%d \n", priority);

return NULL;
}

void *lowprior_thread(void *arg){

pthread_t thread_id = pthread_self();
struct sched_param param;
int priority, policy, ret;

pthread_barrier_wait(&mybarrier);

ret = pthread_getschedparam (thread_id, &policy, &param);
priority = param.sched_priority;	
printf("%d \n", priority);

return NULL;
}

int main(int argc, char *argv[]){

struct sched_param my_param;
pthread_attr_t hp_attr;
pthread_attr_t mp_attr;
pthread_attr_t lp_attr;
int i, min_priority, policy;

/* MAIN-THREAD WITH LOW PRIORITY */
my_param.sched_priority = sched_get_priority_min(SCHED_FIFO);
min_priority = my_param.sched_priority;
pthread_setschedparam(pthread_self(), SCHED_RR, &my_param);
pthread_getschedparam (pthread_self(), &policy, &my_param);


/* SCHEDULING POLICY AND PRIORITY FOR OTHER THREADS */
pthread_attr_init(&lp_attr);
pthread_attr_init(&mp_attr);
pthread_attr_init(&hp_attr);

pthread_attr_setinheritsched(&lp_attr, PTHREAD_EXPLICIT_SCHED);
pthread_attr_setinheritsched(&mp_attr, PTHREAD_EXPLICIT_SCHED);
pthread_attr_setinheritsched(&hp_attr, PTHREAD_EXPLICIT_SCHED);

pthread_attr_setschedpolicy(&lp_attr, SCHED_FIFO);
pthread_attr_setschedpolicy(&mp_attr, SCHED_FIFO);
pthread_attr_setschedpolicy(&hp_attr, SCHED_FIFO);

my_param.sched_priority = min_priority + 1;
pthread_attr_setschedparam(&lp_attr, &my_param);
my_param.sched_priority = min_priority + 2;
pthread_attr_setschedparam(&mp_attr, &my_param);
my_param.sched_priority = min_priority + 3;
pthread_attr_setschedparam(&hp_attr, &my_param);

pthread_barrier_init(&mybarrier,NULL, PRIO_GROUP*3);

for(i=0; i<PRIO_GROUP; i++){
	pthread_create(&lpt[i], &lp_attr, lowprior_thread, NULL);
	pthread_create(&mpt[i], &mp_attr, middleprior_thread, NULL);
	pthread_create(&hpt[i], &hp_attr, highprior_thread, NULL);
}

for(i=0; i<PRIO_GROUP; i++){
	pthread_join(lpt[i], NULL);
	pthread_join(mpt[i], NULL);
	pthread_join(hpt[i], NULL);
}

pthread_barrier_destroy(&mybarrier);
pthread_attr_destroy(&lp_attr);
pthread_attr_destroy(&mp_attr);
pthread_attr_destroy(&hp_attr);

printf("main exiting\n");

return 0;
}


