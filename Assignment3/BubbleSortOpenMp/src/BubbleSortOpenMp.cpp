//============================================================================
// Name        : BubbleSortOpenMp.cpp
// Author      : VaibhavK
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<omp.h>
#include<cstdlib>
#include<cstdio>
#include <cstring>
using namespace std;
void parallelBubbleSort(int *arr,int n)
{
	//cout<<"Num threads: "<<omp_get_num_threads();
	omp_set_num_threads(4);
	double start,end;
	//cout<<"\nNum threads: "<<omp_get_num_threads();
	start=omp_get_wtime();
	int first=0;
	for(int i=0;i<n-1;i++)
	{
		first=i%2;
		#pragma omp  parallel for default(none) shared(arr,first,n)
		for(int j=first;j<n-1;j=j+2)
		{
			//printf("\nNum threads:%d ",omp_get_num_threads());
			if(arr[j]>=arr[j+1])
			{
				int temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
	end=omp_get_wtime();
	cout<<"Parallel Execution time: "<<(end-start);

}
void print(int *arr, int n)
{
	for(int i=0;i<n;i++)
	{
		cout<<" "<<arr[i];
	}
	cout<<"\n";
}
void merge(int *arr, int n, int *tmp)
{
int i=0;
int j=n/2;
int tmpIndex=0;
while(i<n/2 && j<n)
{
	if(arr[i]<arr[j])
	{
		tmp[tmpIndex++]=arr[i++];
	}else
	{
		tmp[tmpIndex++]=arr[j++];
	}
}

while(i<n/2)
{
	tmp[tmpIndex++]=arr[i++];
}
while(j<n)
{
	tmp[tmpIndex++]=arr[j++];
}
memcpy(arr, tmp, n*sizeof(int));
}

void mergeSort(int *arr, int n,int *tmp)
{
	if(n<2)
		return;
#pragma omp task firstprivate (arr,n,tmp)
	mergeSort(arr,n/2,tmp);

#pragma omp task firstprivate (arr,n,tmp)
	mergeSort(arr+(n/2),n-(n/2),tmp);
#pragma omp taskwait
	merge(arr,n,tmp);
	printf("Thread No: %d =",omp_get_thread_num());
	print(arr,n);
}
int main() {
	int n;
	cout<<"Enter No. of Elements";
	cin>>n;
	int *arr=new int[n];
	int *arr2=new int[n];
	int *tmp=new int[n];
	//generate input array
	for(int i=0;i<n;i++)
	{
		arr[i]=rand()%n;
		arr2[i]=arr[i];
	}
	print(arr,n);
	parallelBubbleSort(arr, n);

	cout<<"after sorting";
	print(arr,n);
	cout<<"\nMerge Sort \n";
#pragma omp parallel
	{
#pragma omp single
		mergeSort(arr2, n, tmp);
	}
print(arr2,n);
	return 0;
}
