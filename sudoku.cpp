#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;

int myarray[9][9];

#define NUM_THREADS 9

void* grid(void*)
{
	for (int i = 0; i < 9; ++i)
	{
		cout<<"                              -------------------------------------"<<endl<<"                              ";

		for (int j = 0; j < 9; ++j)
		{
			cout<<"| 1 ";
		}
		cout<<"|"<<endl;
	}
	cout<<"                              -------------------------------------"<<endl;

	pthread_exit(NULL);
}

int main()
{
	pthread_t threadID;


	pthread_create(&threadID,NULL,&grid,NULL);

	void* res;
	pthread_join(threadID,&res);

	return 0;
}		