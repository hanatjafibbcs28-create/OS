#include<stdio.h>
#include<unistd.h>
int main()
{
   int p2,p3,p4,p5;
   p2=fork();
   if (p2==-1)
      printf("Process not created");
   else if(p2==0)
   {
      p4=fork();
      if(p4==-1)
	 printf("Process not created");
      else if(p4==0)
	 printf("p4Id:%d\t,pid:%d\n",getpid(),getppid());
      else{
	 wait(NULL);
	 printf("p2Id:%d\t,pid:%d\n",getpid(),getppid());
      }
   }
   else{
      wait(NULL);
      p3=fork();
      if(p3==-1)
      {
	 printf("Process not created");
      }
      else if(p3==0)
      {
	 p5=fork();
	 if(p5==-1)
	    printf("Process not created");
	 else if(p5==0)
	 {
	    printf("p5Id:%d\t,pid:%d\n",getpid(),getppid());
	 }
	 else{
	    wait(NULL);
	    printf("p3Id:%d\t,pid:%d\n",getpid(),getppid());
	 }
      }
      else{
	 wait(NULL);
	 printf("p1Id:%d\t,pid:%d\n",getpid(),getppid());
      }
   }
   return 0;
}
