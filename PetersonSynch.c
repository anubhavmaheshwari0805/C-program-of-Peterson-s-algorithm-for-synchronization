#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#define max 10
int flag[2];
int turn;
int buffer[max];
int rear=-1;
clock_t st;
void produce(int item)
{
	if(rear==max-1)
	{
		printf("Producer is not able to produce.\n\n");
		return;
	}
	buffer[++rear]=item;
	clock_t et=clock();
	double clk =(double)((et-st)/CLOCKS_PER_SEC);
	printf("%d is produced at time %.2lf sec.\n",item,clk);
	displaybuffer();
	if(clk>60)
	exit(0);
	return;
}
void consume()
{
	if(rear<0)
	{
		printf("Consumer has no product to consume.\n\n");
		return;
	}
	int item=buffer[0],i;
	for(i=0;i<rear;i++)
	buffer[i]=buffer[i+1];
	rear--;
	sleep(item);
	clock_t et=clock();
	double clk=(double)((et-st)/CLOCKS_PER_SEC);
	printf("%d is consumed at time %.2lf sec.\n",item,clk);
	displaybuffer();
	if(clk>60)
	exit(0);
	return;
}
void displaybuffer()
{
	int i;
	if(rear<0)
	{
		printf("Buffer is Empty.\n\n");
		return;
	}
	printf("Buffer : ");
	for(i=0;i<rear;i++)
	printf("%d-",buffer[i]);
	printf("%d\n\n",buffer[i]);
	return;
}
void *producer()
{
	do
	{
		flag[1]=1;//producer is ready to produce.
		turn=0; //turn=1 means producer's turn and turn=0 means consumer's turn.
		while(flag[0]==1&&turn==0);
		produce(rand()%10);
		flag[1]=0;
	}while(turn==1);
}
void *consumer()
{
	do
	{
		flag[0]=1;
		turn=1; //turn=1 means producer's turn and turn=0 means consumer's turn.
		while(flag[1]==1&&turn==1);
		consume();
		flag[0]=0;
	}while(turn==0);
}
void main()
{
	pthread_t p,c;
	flag[0]=flag[1]=0;
	st=clock();
	pthread_create(&p,NULL,producer,NULL);
	pthread_create(&c,NULL,consumer,NULL);
	pthread_join(p,NULL);
	pthread_join(c,NULL);
}
