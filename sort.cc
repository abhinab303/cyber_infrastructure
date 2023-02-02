#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<omp.h>


void insertion_sort (int arr[], int N){
	int i, j, k;
	for (i=1; i<N; i++){
		k = arr[i];
		j = i - 1;
		for(;arr[j]>k && j>0;j--){
			arr[j+1] = arr[j];
		}
		arr[j+1] = k;
	}
}



int X[10000000];

int main(int argc,char** argv){
	int N,K,A,B,C,M;
	FILE* fin = fopen(argv[1],"r");
	fscanf(fin,"%d%d%d%d%d%d",&N,&K,&A,&B,&C,&M);
	for(int i = 0;i < K;++i)
		fscanf(fin,"%d",&X[i]);
	fclose(fin);

	FILE* fout = fopen(argv[2],"w");
	for(int i = K;i < N;++i)
		X[i] = ((long long)A * X[i - 1] + (long long)B * X[i - 2] + C) % M;
	#pragma omp parallel shared(X, N)
	{
	 // std::sort(X,X + N);

		insertion_sort(X, N);


	}
	for(int i = 0;i < N;++i)
		fprintf(fout,"%d\n",X[i]);
	fclose(fout);
	printf("N: %d", N);
	return 0;
}

/*
vanila insertion sort:
N: 100000
real	0m4.634s
user	0m4.621s
sys		0m0.013s

*/
