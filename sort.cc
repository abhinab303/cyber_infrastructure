#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<omp.h>
#include <iostream>
using namespace std;


#define MAX_TH 24

void swap(int* x1, int* x2){
    int t = *x1;
    *x1 = *x2;
    *x2 = t;
}

void insertion_sort (int x[], int N){
	int i, j, k;
	for (i=1; i<N; i++){
		k = x[i];
		j = i - 1;
		for(;x[j]>k && j>0;j--){
			x[j+1] = x[j];
		}
		x[j+1] = k;
	}
}

void bubble_sort(int x[], int N){
	int i, j;
	for (i=0;i<N-1;i++){
		for (j=0;j<N-i-1;j++){
			if (x[j]>x[j+1]){
				swap(&x[j], &x[j+1]);
			}
		}
	}
}

// int L[100000000], R[100000000];
// int L[100000000][100000000];
// int R[100000000][100000000];
// long long rec = 0;

void merge(int x[], int l, int m, int r){
	// int r = rec;
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	// cout << n1 << " " << n2;
	// cout << "l: " << l << "\n";

	// int L[n1], R[n2];
	int *L = new int[n1];
	int *R = new int[n2];

  

	// copy to temp
	for (i=0;i<n1;i++){
		L[i] = x[l+i];
	}
	for (j=0;j<n2;j++){
		R[j] = x[m+1+j];
	}
	
	// merge
	for (i=0,j=0,k=l;i<n1 && j<n2;k++){
		if (L[i] <= R[j]){
			x[k] = L[i];
			i++;
		}else{
			x[k] = R[j];
			j++;
		}
	}

	// copy remaining elements
	while (i<n1){
		x[k] = L[i];
		i++;
		k++;
	}
	while (j<n2){
		x[k] = R[j];
		j++;
		k++;
	}

	delete [] L;
	delete [] R;
}

void merge_sort(int x[], int l, int r){
	if (l<r){
		int m = l + (r-l)/2;
		// cout << l << " " << r;
		merge_sort(x,l,m);
		merge_sort(x,m+1,r);

		merge(x,l,m,r);
	}
}

void merge_sort_parallel(int x[], int l, int r){
	if (l<r){
		int m = l + (r-l)/2;
		// cout << l << " " << r;
		// #pragma omp taskgroup
		// {
			#pragma omp task shared(x)
			merge_sort(x,l,m);
		// }
		
		// #pragma omp taskgroup
		// {
			#pragma omp task shared(x)
			merge_sort(x,m+1,r);
		// }

		// #pragma omp taskgroup
		// {
			#pragma omp taskwait
			merge(x,l,m,r);
		// }
	}
}



int X[100000000];

int main(int argc,char** argv){
	cout << "init: \n";
	int N,K,A,B,C,M;
	
	FILE* fin = fopen(argv[1],"r");
	fscanf(fin,"%d%d%d%d%d%d",&N,&K,&A,&B,&C,&M);
	// int X[N];
	for(int i = 0;i < K;++i)
		fscanf(fin,"%d",&X[i]);
	fclose(fin);

	FILE* fout = fopen(argv[2],"w");
	for(int i = K;i < N;++i)
		X[i] = ((long long)A * X[i - 1] + (long long)B * X[i - 2] + C) % M;


		switch(4){

			case 1:
				cout << "standard sort start: \n";
				std::sort(X,X + N);
				break;

			case 2:
				cout << "insertion sort start: \n";
				insertion_sort(X, N);
				break;
			
			case 3:
				cout << "merge sort start: \n";
				merge_sort(X, 0, N-1);
				break;

			case 4:
				cout << "merge sort parallel start: \n";
				#pragma omp parallel
				{
					// cout << "MAX: " << omp_get_max_threads();
					// cout << "th: " << omp_get_thread_num() << "\n";
					#pragma omp single
					merge_sort_parallel(X, 0, N-1);
				}
				break;
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

naive implementation of parallel on insertion sort:
N: 100000
real	0m4.635s
user	0m4.629s
sys		0m0.005s

Standard cpp sort:
N: 100000
real	0m0.044s
user	0m0.044s
sys		0m0.000s

vanila merge sort:
N: 100000
real	0m0.037s
user	0m0.032s
sys		0m0.005s

standard sort start: 
N: 1000000
real	0m0.210s
user	0m0.198s
sys		0m0.012s

merge sort start: 
N: 1000000
real	0m0.208s
user	0m0.199s
sys		0m0.009s

standard sort start: 
N: 100000000
real	0m22.202s
user	0m21.748s
sys	0m0.352s

merge sort start: 
N: 100000000
real	0m20.868s
user	0m20.268s
sys	0m0.502s


ON GRANGER:
standard sort start: 
N: 100000000
real	0m35.717s
user	0m33.460s
sys	0m0.409s
correct: 4

merge sort start: 
N: 100000000
real	0m34.445s
user	0m32.223s
sys	0m1.050s



*/
