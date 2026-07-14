#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
   int p2;float p;
   int n,t,i,j;
   printf("Enter the count of numbers:");
   scanf("%d",&n);
   int arr[n];
   printf("Elements:");
   for(i=0;i<n;i++)
      scanf("%d",&arr[i]);
   p2=fork();
   if(p2==-1){
      printf("Process not created");
   }
   else if(p2==0){
      int m;
      for(i=0;i<n;i++){
	 m+=arr[i];
      }
      p=m/n;
      printf("P2 process\n");
      printf("\nMean of the array:%.2f\n",p);
   }
   else{
      wait(NULL);
      printf("P1 process\n");
      for(i=0;i<n-1;i++){
	 for(j=i+1;j<n;j++){
	    if(arr[i]>arr[j]){
	       t=arr[i];
	       arr[i]=arr[j];
	       arr[j]=t;
	    }
	 }
      }
      printf("Sorted Array");
      for(i=0;i<n;i++){
	 printf("%d\t",arr[i]);
      }
      printf("\n");
      if(n%2==1){
	 printf("Median=%d",arr[n/2]);
      }
      else
	 printf("Median=%d",(arr[n/2]+arr[(n-1)/2])/2);
   }
   return 0;
}

