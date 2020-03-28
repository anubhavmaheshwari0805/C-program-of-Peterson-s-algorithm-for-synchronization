/*Problem:The producer consumer problem (or bounded buffer problem) describes two processes,
the producer and the consumer, which share a common, fixed-size buffer used as a queue.
Producer produce an item and put it into buffer.
If buffer is already full then producer will have to wait for an empty block in buffer.
Consumer consume an item from buffer.
If buffer is already empty then consumer will have to wait for an item in buffer.
Implement Peterson�s Algorithm for the two processes using shared memory such that there is mutual exclusion between them.
The solution should have free from synchronization problems.*/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#define max 10
int flag[2];
int turn;                                         //turn=1 means producer's turn and turn=0 means consumer's turn.
int buffer[max];                                  //queue named as buffer which is shared by producer and consumer both.
int rear=-1;
clock_t st;
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
void *producer()
{
	do
	{
		flag[1]=1;                                 //producer is ready to produce.
		turn=0;                                    //consumer's turn
		while(flag[0]==1&&turn==0);                //checking of consumer to consume.
		produce(rand()%10);                        //producer produces an element.
		flag[1]=0;                                 //now, producer is not ready to produce.
	}while(turn==1);                               //when this is producer's turn to produce.
}
void *consumer()
{
	do
	{
		flag[0]=1;                                 //consumer is ready to consume.
		turn=1;                                    //producer's turn
		while(flag[1]==1&&turn==1);                //checking of producer to produce.
		consume();                                 //consumer consumes an element.
		flag[0]=0;                                 //now, consumer is not ready to consume.
	}while(turn==0);                               //when this is consumer's turn to consume.
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