#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
 #include <unistd.h>
#include <pthread.h>
typedef struct DATA_T
{
	long double start;
	long double end;
	long int iter;
	long double sum;
	long int num_of_cpus;
} DATA;

   
#define START 0
#define END 500
#define NUM_OF_INTERVALS 500000000L


void init_data(DATA* argv, long int num_of_threads)
{
	long double delta = END - START;
	for(long int i = 0; i < num_of_threads; ++i)
	{
		argv[i].start = START + i * delta / num_of_threads;
		argv[i].end = START + (i + 1) * delta / num_of_threads;
		argv[i].sum = 0;
		argv[i].num_of_cpus = num_of_threads;
	}
}


long double func(long double arg)
{
	return (sinl(arg) * cosl(arg));
}

void* simpson_count(void* argv)
{
	DATA *data = (DATA*)argv;
	long num_of_cpus = data -> num_of_cpus;
	long double start = data -> start;
	long double end = data -> end;
	long double sum = 0;
	long iters = NUM_OF_INTERVALS / num_of_cpus;
	long double step = (end - start) / iters;
	for(int i = 0;i < iters; i++)
		sum += func(start+i*step) * step;
	data -> sum = sum;

	return NULL;
}

int min(int a, int b)
{
	return a > b ? b : a; 
}


int  main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		perror("NOT ENOUGH ARGUMENTS");
		return -1;
	}
	int num_of_threads = 0;
	num_of_threads = strtol(argv[1], NULL, 10);

	//num_of_threads = min(num_of_threads, sysconf(_SC_NPROCESSORS_ONLN));
	DATA *data = NULL;
	data = (DATA*)malloc(num_of_threads * sizeof(DATA));
	if(data == NULL)
	{
		perror("MEMORY ERROR");
		return -1;
	}

	pthread_t* pthread;
	pthread = malloc(num_of_threads * sizeof(pthread_t));//
	if(pthread == NULL)
	{
		perror("MEMORY ERROR");
		return -1;
	}

	init_data(data, num_of_threads);

	for(int i = 0; i < num_of_threads; i++)
	{
		int err = pthread_create(&pthread[i], NULL, simpson_count, &data[i]); 
		if(err != 0)
		{
			perror("CAN NOT MAKE THREAD");
			exit(-1);
		}
	}

	for(int i = 0; i < num_of_threads; i++)
	{
		pthread_join(pthread[i], NULL);
	}

	long double res = 0;
	for(int i = 0; i < num_of_threads; i++)
	{
		res += data[i].sum;
	}

	printf("%Lf\n", res);
	free(data);
	free(pthread);
	
	return 0;
}
