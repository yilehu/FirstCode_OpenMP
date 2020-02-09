#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	int tid,Num_Threads,Num_Threads_Max;
	#pragma omp parallel
	{
		Num_Threads_Max = omp_get_num_threads();
		tid = omp_get_thread_num();
		//printf("Hello world! from thread %d.\n",tid);
	}
	printf("Maximum number of threads = %d\n",Num_Threads_Max);

	Num_Threads = 12;
	omp_set_num_threads(Num_Threads);
	printf("Number of threads chosen = %d\n",Num_Threads);

	int i,N = 1000000000;
	double Pi,Sum = 0.0,*Temp;
	Temp = (double*)malloc(Num_Threads*sizeof(double));
	for(i=0;i<Num_Threads;i++)
	{
		Temp[i] = 0.0;
	}

	double START_WTIME,END_WTIME;
	double START_CLOCK,END_CLOCK;
	double Time,WTime;

	START_CLOCK = clock();
	START_WTIME = omp_get_wtime();

	#pragma omp parallel default(none) private(tid,i) shared(Sum,Temp,N,Num_Threads)
	{
		tid = omp_get_thread_num();

		#pragma omp for schedule(static,1) reduction(+:Sum)
		for(i=0;i<N;i++)
		{
			Sum += 1.0 / (4.0 * i + 1.0) - 1.0 / (4.0 * i + 3.0);
			Temp[tid] += 1.0 / (4.0 * i + 1.0) - 1.0 / (4.0 * i + 3.0);
			//printf("Hello world! from thread %d, i = %d\n",tid,i);
		}

		#pragma omp master
		{
			for(i=1;i<Num_Threads;i++)
			{
				Temp[0] += Temp[i];
			}
		}
	}

	printf("Pi = %30.24lf\n",4.0*Sum);
	printf("Pi = %30.24lf\n",4.0*Temp[0]);

	END_CLOCK = clock();
	END_WTIME = omp_get_wtime();
	Time = END_CLOCK - START_CLOCK;
	WTime = END_WTIME - START_WTIME;

	printf("Time = %10.4f s, WTime = %10.4f s, CPU Frequency = %6.3f GHz\n",(float)(END_CLOCK - START_CLOCK)/CLOCKS_PER_SEC,WTime,1.0/omp_get_wtick()/1.0E6);
	system("pause");
}
