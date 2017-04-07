#define _GNU_SOURCE             
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <sched.h>

#define NUM 100.0

typedef  struct slave_ {
    double start_point;
    double end_point;
    pthread_t *pthreads_id;
    int cpu;
    char trash [100];
} Slave;

void *thread_job(void *thr) {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET((*(Slave*)thr).cpu , &cpu_set);
    pthread_setaffinity_np(*(*(Slave*)thr).pthreads_id, sizeof(cpu_set_t), &cpu_set);
    double start_point = (*(Slave*)thr).start_point;
    double end_point = (*(Slave*)thr).end_point;
    double counter = start_point;
    double dx = 0.00000001;
    double sum = 0;

    while(counter < end_point) {
        sum += counter*counter*dx;
        counter +=dx;
    }
    (*(Slave*)thr).start_point = sum; 
    return NULL;
}

int main(int argc, const char * argv[]) {


    if(argc != 2) {
        printf("Enter number of threads!\n");
        return 0;
    }

    int n, max;
    max = sysconf(_SC_NPROCESSORS_ONLN);
    Slave sections[max];
   // printf("%p   %p\n", &sections[0], &sections[1]);
    n = (int) strtol(argv[1], NULL, 10);
    n = n > max ? max : n;
    double sum = 0;

    for(int i = 0; i < max; i++) {       
        if(i < n) {
            sections[i].start_point = (NUM/n)*i;
            sections[i].end_point = sections[i].start_point + (NUM/n);
            sections[i].cpu = i % n;
            pthread_create(&(sections[i].pthreads_id), NULL, thread_job, (void *)&(sections[i]));
        }
        else {
            sections[i].start_point = (NUM/n)*0;
            sections[i].end_point = sections[i].start_point + (NUM/n);
            sections[i].cpu = i % max;
            pthread_create(&(sections[i].pthreads_id), NULL, thread_job, (void *)&(sections[i]));
        }   
    }
    for(int i = 0; i < max; i++) {
        pthread_join(sections[i].pthreads_id, NULL);
        if(i < n)
            sum += sections[i].start_point;
    }

    printf("%f\n", (float)sum);
    //free(pthreads_ids);
    //free(sections);

    return 0;
}