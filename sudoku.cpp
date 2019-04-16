#include<iostream>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;

#define GRID_ROWS 9
#define GRID_COLS 9

int GRID[GRID_ROWS][GRID_COLS];
int numbers[9]={1,2,3,4,5,6,7,8,9};

void draw_grid();
void print_grid();
void draw_grid();
int main(){

	//draw_grid();
	//print_grid();

	for (int m=0;m<GRID_ROWS;m++)
	{

		for (int i=0;i<GRID_COLS;i++)
		{
			int random1=rand()%9;
			int random2=rand()%9;
			int temp=numbers[random1];
			numbers[random1]=numbers[random2];
			numbers[random2]=temp;
		}

		for (int i=0;i<9;i++)
		{
				GRID[m][i]=numbers[i];
		}
	}

	cout<<endl;
	print_grid();

	return 0;
}

void write_grid()
{
	int fd=open("output.txt",O_WRONLY);
	
}
void print_grid()
{
	cout<<"Grid :"<<endl;
	for (int i=0;i<GRID_ROWS;i++)
	{
		for (int j=0;j<GRID_COLS;j++)
		{
			cout<<GRID[i][j]<<"    ";
		}
		cout<<endl;
	}	
}