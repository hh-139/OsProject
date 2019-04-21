#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

void *printgrid(void *arg);
void *giveResults(void *arg);
void *validiateGrid(void *arg);
void *validiateRows(void *arg);
void *validiateColumns(void *arg);

#define GRID_ROWS 9
#define GRID_COLS 9

pthread_mutex_t sharedlock_mutex;

struct limits
{
	int s_row, s_col, e_row, e_col;
};

struct r_c_g_check
{
	int value;
	int n_r, n_c, n_g;
	bool GTz_LSt;

	r_c_g_check()
	{
		this->value = 0;
		this->n_r = 0;
		this->n_c = 0;
		this->n_g = 0;
		this->GTz_LSt = true;
	}
};

r_c_g_check grid_puzzle_validiator[GRID_ROWS][GRID_COLS];

int thread_validiation_results[11] = {0};

int grid_puzzle[GRID_ROWS][GRID_COLS] =
	{{3, 2, 4, 5, 3, 9, 1, 8, 7},
	 {5, 1, 9, 7, 2, 8, 6, 3, 4},
	 {8, 3, 4, 6, 1, 4, 2, 9, 5},
	 {1, 4, 3, 8, 6, 5, 7, 2, 9},
	 {9, 5, 8, 2, 4, 7, 3, 6, 1},
	 {7, 6, 2, 3, 9, 1, 4, 5, 8},
	 {3, 7, 1, 9, 5, 6, 8, 4, 2},
	 {4, 9, 6, 1, 8, 2, 5, 2, 3},
	 {2, 8, 5, 4, 7, 3, 9, 1, 6}};

int main()
{
	if (pthread_mutex_init(&sharedlock_mutex, NULL) != 0)
	{
		printf("\n mutex init has failed\n");
		return 1;
	}

	//Prints the grid
	pthread_t threadPrint;
	pthread_create(&threadPrint, NULL, &printgrid, NULL);
	pthread_join(threadPrint, NULL);

	//set limits for row and column check
	struct limits c_r_wise;
	c_r_wise.s_row = 0;
	c_r_wise.s_col = 0;
	c_r_wise.e_row = 9;
	c_r_wise.e_col = 9;

	pthread_t threadColumns;
	pthread_create(&threadColumns, NULL, &validiateColumns, &c_r_wise);
	pthread_join(threadColumns, (void **)&thread_validiation_results[0]);

	pthread_t threadRow;
	pthread_create(&threadRow, NULL, &validiateRows, &c_r_wise);
	pthread_join(threadRow, (void **)&thread_validiation_results[1]);

	struct limits gridwise;
	gridwise.s_row = 0;
	gridwise.s_col = -3;
	gridwise.e_row = 2;
	gridwise.e_col = -1;

	pthread_t threadGrid[9];

	for (int i = 1, j = 2; i <= 11; i++, ++j)
	{
		if (i % 4 == 0)
		{
			gridwise.s_row += 3;
			gridwise.e_row += 3;
			gridwise.s_col = 0;
			gridwise.e_col = 2;
			++i;
		}
		else
		{
			gridwise.s_col += 3;
			gridwise.e_col += 3;
		}
		pthread_create(&threadGrid[j - 2], NULL, &validiateGrid, &gridwise);
		pthread_join(threadGrid[j - 2], (void **)&thread_validiation_results[j]);
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cout << grid_puzzle_validiator[i][j].value << "," << grid_puzzle_validiator[i][j].n_c << "," << grid_puzzle_validiator[i][j].n_r << "," << grid_puzzle_validiator[i][j].n_g << "  ";
		}
		cout << endl;
	}

	pthread_t threadPrintResult;
	pthread_create(&threadPrintResult, NULL, &giveResults, NULL);
	pthread_join(threadPrintResult, NULL);

	pthread_mutex_destroy(&sharedlock_mutex);

	return 0;
}

void *printgrid(void *arg)
{
	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			cout << grid_puzzle[i][j] << " ";
		}
		cout << endl;
	}

	pthread_exit(NULL);
}
void *giveResults(void *arg)
{
	cout << "Thread:"
		 << "1 has calculated invalid columns and index as" << endl;

	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			if (grid_puzzle_validiator[j][i].n_c > 1)
			{
				cout << "Column:" << i + 1 << " having index " << j << i << " and value:" << grid_puzzle_validiator[j][i].value << " is invalid" << endl;
			}
		}
	}
	cout << "Thread:"
		 << "2 has calculated invalid rows and index as" << endl;

	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			if (grid_puzzle_validiator[i][j].n_r > 1)
			{
				cout << "row:" << i + 1 << " having index " << i << j << " and value:" << grid_puzzle_validiator[i][j].value << " is invalid" << endl;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0 + (i * 3); k < 3 + (i * 3); k++)
			{
				for (int l = 0 + (j * 3); l < 3 + (j * 3); l++)
				{
					if (grid_puzzle_validiator[k][l].n_g > 1)
					{
						cout << "Thread:" << ((i * j) + 1 + j + i) + 2 << " for Grid:" << ((i * j) + 1 + j + i) << " having index " << k << l << " and value:" << grid_puzzle_validiator[k][l].value << " is invalid" << endl;
					}
				}
			}
		}
	}

	pthread_exit(NULL);
}

void *validiateColumns(void *arg)
{
	pthread_mutex_lock(&sharedlock_mutex);

	bool check = true;

	struct limits *l;
	l = (struct limits *)arg;

	for (int i = l->s_col; i < l->e_col; ++i)
	{
		r_c_g_check temp[GRID_ROWS];

		for (int j = l->s_row; j < l->e_row; ++j)
		{
			if (grid_puzzle[j][i] > 0 && grid_puzzle[j][i] < 10)
			{
				temp[(grid_puzzle[j][i]) - 1].n_c += 1;
			}
			// else
			// {
			// 	temp[(grid_puzzle[j][i]) - 1].GTz_LSt = false;
			// }
		}
		for (int k = l->s_row; k < l->e_row; ++k)
		{
			grid_puzzle_validiator[k][i].n_c = temp[(grid_puzzle[k][i]) - 1].n_c;
			if (grid_puzzle_validiator[k][i].n_c > 1)
			{
				check = false;
			}
			grid_puzzle_validiator[k][i].value = grid_puzzle[k][i];
		}
	}

	pthread_mutex_unlock(&sharedlock_mutex);

	pthread_exit((void *)check);
}

void *validiateRows(void *arg)
{
	pthread_mutex_lock(&sharedlock_mutex);

	bool check = true;

	struct limits *l;
	l = (struct limits *)arg;

	for (int i = l->s_row; i < l->e_row; ++i)
	{
		r_c_g_check temp[GRID_ROWS];

		for (int j = l->s_col; j < l->e_col; ++j)
		{
			if (grid_puzzle[i][j] > 0 && grid_puzzle[i][j] < 10)
			{
				temp[(grid_puzzle[i][j]) - 1].n_r += 1;
			}
			// else
			// {
			// 	temp[(grid_puzzle[i][j]) - 1].GTz_LSt = false;
			// }
		}
		for (int k = l->s_col; k < l->e_col; ++k)
		{
			grid_puzzle_validiator[i][k].n_r = temp[(grid_puzzle[i][k]) - 1].n_r;
			if (grid_puzzle_validiator[k][i].n_r > 1)
			{
				check = false;
			}
			grid_puzzle_validiator[i][k].value = grid_puzzle[i][k];
		}
	}

	pthread_mutex_unlock(&sharedlock_mutex);

	pthread_exit((void *)check);
}

void *validiateGrid(void *arg)
{
	pthread_mutex_lock(&sharedlock_mutex);

	bool check = true;

	struct limits *l;
	l = (struct limits *)arg;

	r_c_g_check temp[GRID_ROWS];
	for (int i = l->s_row; i <= l->e_row; ++i)
	{

		for (int j = l->s_col; j <= l->e_col; ++j)
		{
			if (grid_puzzle[i][j] > 0 && grid_puzzle[i][j] < 10)
			{
				temp[(grid_puzzle[i][j]) - 1].n_g += 1;
			}
			// else
			// {
			// 	temp[(grid_puzzle[i][j]) - 1].GTz_LSt = false;
			// }
		}
	}
	for (int i = l->s_row; i <= l->e_row; i++)
	{
		for (int k = l->s_col; k <= l->e_col; ++k)
		{
			grid_puzzle_validiator[i][k].n_g = temp[(grid_puzzle[i][k]) - 1].n_g;
			if (grid_puzzle_validiator[k][i].n_g > 1)
			{
				check = false;
			}
			grid_puzzle_validiator[i][k].value = grid_puzzle[i][k];
		}
	}

	pthread_mutex_unlock(&sharedlock_mutex);

	pthread_exit((void *)check);
}
