#include<iostream>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;

struct limitcheck
{
	int startingrow,startingcol,endingrow,endingcol;
};

int grid[9][9]={{6,2,4,5,3,9,1,8,7},{5,1,9,7,2,8,6,3,4},{8,3,7,6,1,4,2,9,5},
{1,4,3,8,6,5,7,2,9},{9,5,8,2,4,7,3,6,1},{7,6,2,3,9,1,4,5,8},{3,7,1,9,5,6,8,4,2},
{4,9,6,1,8,2,5,7,3},{2,8,5,4,7,3,9,1,6}};

void* printgrid(void* arg)
{
	for (int i=0;i<9;i++)
	{
		for (int j=0;j<9;j++)
		{
			cout<<grid[i][j]<<" ";
		}
	cout<<endl;
	}
	pthread_exit(NULL);
}

void *checkeverycol(void* arg)
{
	/*int l=((limitcheck*)arg)->startingrow;
	int m=((limitcheck*)arg)->startingcol;
	int n=((limitcheck*)arg)->endingrow;
	int o=((limitcheck*)arg)->endingcol;


	cout<<l<<endl;
	cout<<m<<endl;
	cout<<n<<endl;
	cout<<o<<endl;
*/
	bool check=true;
	struct limitcheck *l;
	l=(struct limitcheck * ) arg;
	for (int i = 0; i < 9; ++i)
	{
		int * checkarr=new int[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
		for (int j = 0; j < 9; ++j)
		{
			checkarr[(grid[i][j])-1]=1;
		}
		for (int k = 0; k < 9; ++k)
		{
			if(checkarr[k]!=1){
				pthread_exit(NULL);
			}
		}
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t threadID;
	pthread_create(&threadID,NULL,&printgrid,NULL);
	pthread_join(threadID,NULL);



	limitcheck colwise;
	colwise.startingrow=0;
	colwise.startingcol=0;
	colwise.endingrow=9;
	colwise.endingcol=9;

	pthread_t threadID2;
	// pthread_attr_t attr;
	// pthread_attr_init(&attr);
	// pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&threadID2,NULL,&checkeverycol,&colwise);

	pthread_exit(NULL);
	return 0;
}