#include<iostream>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;

struct limitcheck
{
	int startingrow,startingcol,endingrow,endingcol;
	bool check;
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
		int checkarr[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
		for (int j = 0; j < 9; ++j)
		{
			if (grid[j][i]>0 && grid[j][i]<10)
			{
				checkarr[(grid[j][i])-1]=1;
			}
		}
		for (int k = 0; k < 9; ++k)
		{
			if(checkarr[k]!=1){
				l->check=false;
				pthread_exit(l);
			}
		}
	}
	l->check=true;
				pthread_exit(l);
}

void* checkeveryrow(void* arg)
{
	bool check=true;
	struct limitcheck *m;
	m=(struct limitcheck * ) arg;
	for (int i = 0; i < 9; ++i)
	{
		int checkarr[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
		for (int j = 0; j < 9; ++j)
		{
			if (grid[i][j]>0 && grid[i][j]<10)
			{
				checkarr[(grid[i][j])-1]=1;
			}
		}
		for (int k = 0; k < 9; ++k)
		{
			if(checkarr[k]!=1){
				m->check=false;
				pthread_exit(m);
			}
		}
	}
	m->check=true;
				pthread_exit(m);
}

void* checkeverygrid(void* arg)
{

}

int main()
{
	
	pthread_t threadID;
	pthread_create(&threadID,NULL,&printgrid,NULL);
	pthread_join(threadID,NULL);

	struct limitcheck colwise;
	colwise.startingrow=0;
	colwise.startingcol=0;
	colwise.endingrow=9;
	colwise.endingcol=9;
	colwise.check=true;

	pthread_t threadID2;
	// pthread_attr_t attr;
	// pthread_attr_init(&attr);
	// pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&threadID2,NULL,&checkeverycol,&colwise);

	pthread_join(threadID2,(void**)& colwise);
	cout<<endl<<colwise.check<<endl;
	if (colwise.check)
	{
		cout<<"Column thread returned true"<<endl;
	}
	else
	{
		cout<<"Column thread returned false"<<endl;
	}

	struct limitcheck rowwise;
	rowwise.startingrow=0;
	rowwise.startingcol=0;
	rowwise.endingrow=9;
	rowwise.endingcol=9;
	rowwise.check=true;

	pthread_t threadID3;
	pthread_create(&threadID3,NULL,&checkeveryrow,&rowwise);
	pthread_join(threadID3,(void**)& rowwise);
	cout<<endl<<rowwise.check<<endl;
	if (rowwise.check)
	{
		cout<<"Row thread returned true"<<endl;
	}
	else
	{
		cout<<"Row thread returned false"<<endl;
	}
	

	struct limitcheck gridwise;
	gridwise.startingrow=0;
	gridwise.startingcol=0;
	gridwise.endingrow=2;
	gridwise.endingcol=2;
	gridwise.check=true;

	pthread_t threadIDgrid[9];

	for (int i=1;i<=9;i++)
	{
		pthread_create(&threadIDgrid[i],NULL,&checkeverygrid,&gridwise);
		pthread_join(threadIDgrid[i],void(**)& gridwise);
		if (i%3==0)
		{
			gridwise.startingrow+=3;
			gridwise.endingrow+=3;
			gridwise.startingcol=0;
			gridwise.endingcol=2;
		}
		else
		{
			gridwise.startingcol+=3;
			gridwise.endingcol+=3;
		}
	}


	return 0;
}