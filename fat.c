#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
void mergesort(int a[],int i,int j);
void merge(int a[],int i1,int j1,int i2,int j2);

int main(int argc, char* argv[])
{
    int *a, num, i,m;
    MPI_Init(&argc, &argv);//initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &m);//determine number of nodes
    printf("\n Enter array size \n"); //get array size from user
    scanf("%d",&num);
    double start = omp_get_wtime();//get start time of execution
    
    

    
   a = (int *)malloc(sizeof(int) * num);
	
    for( i = 0 ; i < num ; i++ ) 
   {
      a[i] = rand() % 100; //randomly allot values to array
      
   }
    mergesort(a, 0, num-1); //call mergesort function
    
    
    double end = omp_get_wtime();//get end time of execution
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);//all processes are synchronized
    MPI_Finalize();//close MPI environment
    printf("Time taken is %lf s \n",end-start);//print time taken for execution

    
    return 0;
}

void mergesort(int a[],int i,int j)
{
    int mid;
    int myid;

    MPI_Comm_rank(MPI_COMM_WORLD, &myid);//determine rank of processes
if(myid!=0) //carry out for all processes except master process
if(i<j)
    {
        mid=(i+j)/2;

        #pragma omp parallel sections //section out the code for left and right separately f
        {

            #pragma omp section
            {
                mergesort(a,i,mid);        //left recursion 
            }

            #pragma omp section
            {
                mergesort(a,mid+1,j);    //right recursion
            }
        }

        merge(a,i,mid,mid+1,j);    //merging of two sorted sub-arrays
    }
}

void merge(int a[],int i1,int j1,int i2,int j2)
{
    int temp[10];    //array used for merging
    int i,j,k;
    i=i1;    //beginning of the first list
    j=i2;    //beginning of the second list
    k=0;

    while(i<=j1 && j<=j2)    //while elements in both lists
    {
        if(a[i]<a[j])
            temp[k++]=a[i++];
        else
            temp[k++]=a[j++];
    }

    while(i<=j1)    //copy remaining elements of the first list
        temp[k++]=a[i++];

    while(j<=j2)    //copy remaining elements of the second list
        temp[k++]=a[j++];

    //Transfer elements from temp[] back to a[]
    for(i=i1,j=0;i<=j2;i++,j++)
        a[i]=temp[j];
}
