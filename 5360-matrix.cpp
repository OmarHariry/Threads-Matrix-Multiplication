
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
#include <iostream>
#include <bits/stdc++.h>

//Each thread computes single element in the resultant matrix
void *first_approach(void* arg)
{
	int *arr = (int *)arg;
	int sum = 0, i = 0;
	//x is the number of elements in each column
	int elements_per_col = arr[0];
	//normal matrix multiplication between the row and the col to get the resulting element
	for (i = 1; i <= elements_per_col; i++)
		sum += arr[i]*arr[i+ elements_per_col];
	//allocating pointer to the resulting element
	int *p = (int*)malloc(sizeof(int));
		*p = sum;

	//exit the thread
	pthread_exit(p);
}
//Each thread computes single row in the resultant matrix
void* second_approach(void* arg)
{
    int *arr = (int *)arg;
	int sum = 0, i = 0;
	int elements_per_col = arr[0]; //common dimension
	int y= arr[1];
	int *p = (int*)malloc(sizeof(int)*y);
	//for each element in the row of the reultant matrix
	for(int j=0; j<y;j++){
	//caculate it's value as in the previous function
	  for (i = 2; i <= elements_per_col +1; i++)
		sum += arr[i+(2*j* elements_per_col)]*arr[i+ elements_per_col +(2*j* elements_per_col)];
	//store the calculated value in an array to be retrieved later as the required row
	        p[j]  = sum;
	        sum = 0;
}
	//exit the thread
	pthread_exit((void*)p);
}

//Driver code
int main()
{


	//reading from the input file
	FILE* f;

    if ((f = fopen("input.txt", "r")) == NULL) {
        printf("Error!");
    }
        int rowA=0, rowB=0, colA=0,colB =0;
        fscanf(f, "%d %d", &rowA, &colA);
        int matA[rowA][colA];
        for (int i=0 ; i<rowA; i++) {
            for (int j=0; j<colA; j++) {

                fscanf(f, "%d", &matA[i][j]);

            }
        }
        fscanf(f, "%d %d", &rowB, &colB);
        int  matB[rowB][colB];
        for (int i=0 ; i<rowB; i++) {
            for (int j=0; j<colB; j++) {
                fscanf(f, "%d", &matB[i][j]);
            }
        }
        fclose(f);
     
        FILE* fout;
	fout = fopen("output.txt", "w");

	auto start = high_resolution_clock::now();
	int max = rowA*colB;


	pthread_t *threads;
	threads = (pthread_t*)malloc(max*sizeof(pthread_t));

	int count = 0;
	int* arr = NULL;
	//for each element in the output 
	//store the row and column which results in this element 
	for (int i = 0; i < rowA; i++)
		for (int j = 0; j < colB; j++)
			{
			arr = (int *)malloc((20)*sizeof(int));
			//store the number of elements per row
			arr[0] = colA;
			//storing the row elements
			for (int k = 0; k < colA; k++)
				arr[k+1] = matA[i][k];
			//then the column elements
			for (int k = 0; k < rowB; k++)
				arr[k+colA+1] = matB[k][j];
			//know calculate the element multiplication using threads
				pthread_create(&threads[count++], NULL,
							first_approach, (void*)(arr));

					}

	for (int i = 0; i < max; i++)
	{
	void *k;

	//Joining all threads and collecting return value
	pthread_join(threads[i], &k);
	int *p = (int *)k;
	printf("%d ",*p);
	fprintf(fout, "%d ",*p);
	if ((i + 1) % colB == 0) {
		printf("\n");
		fprintf(fout, "\n");
}
	}
 	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start);
	cout<<"END1	";
  	cout << duration.count() << endl;
	fprintf(fout, "END1\t%ld nanoseconds\n",duration.count());

//End of first approach 



	auto start2 = high_resolution_clock::now();
	//creating array of threads of size rowA
	//which is the number of rows in the output matrix
	pthread_t *threads2;
	threads2 = (pthread_t*)malloc(rowA*sizeof(pthread_t));
	int* arr2 = NULL;
	count = 0;
	//for each output row
	//we need the input row and all the columns
	for (int i = 0; i < rowA; i++) {
		arr2 = (int *)malloc((100)*sizeof(int));
		//store the number of elements per row 
		arr2[0] = colA;
		//colB is the number of elements per row in the output matrix
		arr2[1] = colB;
		for (int j = 0; j < colB; j++)
			{
			//storing the row elements of matrixA
			for (int k = 0; k < colA; k++)
				arr2[k+2+(j*colA*2)] = matA[i][k];
			//store all the columns of matrixB
			for (int k = 0; k < rowB; k++)
				arr2[k+colA+2+(j*colA*2)] = matB[k][j];

					}

				
				pthread_create(&threads2[count++], NULL,
							second_approach, (void*)(arr2));
}

	for (int i = 0; i < rowA; i++)
	{
	void *k;
	//joining threads and collecting return values
	pthread_join(threads2[i], &k);
		int *p = (int *)k;
	for(int j=0;j<colB;j++){
		printf("%d ",p[j]);
		fprintf(fout,"%d ",p[j]);
		}
		printf("\n");
		 fprintf(fout, "\n");
	}
	auto stop2 = high_resolution_clock::now();
	//time calculation
	auto duration2 = duration_cast<nanoseconds>(stop2 - start2);
	cout<<"END2	";
	cout << duration2.count() << endl;
	fprintf(fout, "END2\t%ld nanoseconds",duration2.count());
return 0;
}


