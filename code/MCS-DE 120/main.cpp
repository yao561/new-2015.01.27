#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>

//���������ؿ����������
#define point_num 122 //  �ڵ����Ŀ
#define Lambda 10
#define M 1000
#define Rc 0.95 //  ��Ҫ�ﵽ������ṹ���ȶ���
#define rc 0.8  //  ÿ���ߵ��ȶ��Ե��½�

//������DE�������
#define particle_num 15 //  ���ӵ���Ŀ
#define dim 183 //���ӵ�ά��
#define xmax 1
#define xmin 0.8
#define CR 0.7
#define F 0.5
#define iteration 100



using namespace std;

const double Alpha[] = {120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90, 100, 65, 100, 90, 160, 160,
                        120, 120, 90};
const double Beta[] =  {14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12,
                        14.7, 14.7, 8.75};

double x[particle_num][dim];
double v[particle_num][dim];
double u[particle_num][dim];
double gbest[dim];
double gbest_value = 10e20;
double fitness[iteration];  //  ������¼ÿһ�ֵ������Ӧֵ

double r[dim];
int original_r[point_num][point_num];  //  ��ʾԭʼ����ͨͼ
int r2[point_num][point_num];    //  r2[i][j]��1��ʾi,j������ͨ�ģ���0��ʾ����ͨ
bool visited[point_num];

int count1 = 0;

void DFS(int a)
{
    visited[a] = true;
    if(a == point_num-1)
        return;
    for(int i = 0; i < point_num; i++)
    {
        if(visited[i] == false && r2[a][i])
            DFS(i);
    }
}


bool is_connected()
{
    memset(visited, false, sizeof(visited));
    DFS(0);
    if(visited[point_num-1])
        return true;
    return false;
}


double MCS(const double *r, int simulation_replication) //  ����ÿ���ߵ��ȶ��Ժ����ؿ���ĵ�����������������ṹ���ȶ���
{
    int SUCCESS = 0;
    for(int i = 0; i < simulation_replication; i++)
    {
        memset(r2, 0, sizeof(r2));
        int temp = 0;
        for(int j = 0; j < point_num; j++)
            for(int k = 0; k < point_num; k++)
            {
                if(original_r[j][k])
                    if(1.0*rand()/RAND_MAX < r[temp++])
                        r2[j][k] = 1;
            }

        if(is_connected())
            SUCCESS++;
    }
    return (double)SUCCESS/simulation_replication;
}

double cost_function(double a, double b, double c)
{
    return a-b*log(1-c);
}

double cost_sum(const double *r)  //  ����ÿ���ߵ��ȶ��ԣ�����ܵ�cost
{
    double tot = 0;
    for(int i = 0; i < dim; i++)
        tot += cost_function(Alpha[i], Beta[i], r[i]);
    double Rr = MCS(r, M);
    if(Rr < Rc)
        tot *= pow((double)Rc/Rr, Lambda);
    return tot;
}


void init()
{
    for(int i = 0; i < particle_num; i++)
        for(int j = 0; j < dim; j++)
            x[i][j] = xmin + (xmax - xmin) * 1.0 * rand() / RAND_MAX;
    for(int i = 0; i < particle_num; i++)
    {
        if(cost_sum(x[i]) < gbest_value)
        {
            gbest_value = cost_sum(x[i]);
            for(int j = 0; j < dim; j++)
                gbest[j] = x[i][j];
        }
    }
    fitness[count1++] = gbest_value;
}

void process()
{
    for(int i = 0; i < particle_num; i++)
    {
        int r1, r2, r3;
        do
        {
        r1 = rand() % particle_num;
        } while(r1==i);
        do
        {
        r2 = rand() % particle_num;
        } while( r2==i || r2==r1);
        do
        {
        r3 = rand() % particle_num;
        } while( r3==i || r3==r1 || r3==r2 );

        for(int j = 0; j < dim; j++)
        {
            v[i][j] = x[r1][j] + F * (x[r2][j] - x[r3][j]);
            if(v[i][j] > xmax)
                //v[i][j] = 2 * xmax - v[i][j];
                v[i][j] = xmax;
            if(v[i][j] < xmin)
                //v[i][j] = 2 * xmin - v[i][j];
                v[i][j] = xmin;

            if(1.0*rand()/RAND_MAX<=CR || j == rand()%dim)
                u[i][j] = v[i][j];
            else
                u[i][j] = x[i][j];
        }
    }
}

void select()
{
    for(int i = 0; i < particle_num; i++)
    {
        if(cost_sum(u[i]) < cost_sum(x[i]))
            for(int j = 0; j < dim; j++)
            {
                x[i][j] = u[i][j];
            }
    }
    double temp_best = 10e20;
    for(int i = 0; i < particle_num; i++)
    {
        if(cost_sum(x[i]) < temp_best)
        {
            temp_best = cost_sum(x[i]);
            if(temp_best < gbest_value)
                gbest_value = temp_best;
                for(int j = 0; j < dim; j++)
                    gbest[j] = x[i][j];
        }
    }
    fitness[count1++] = gbest_value;
}

void output()
{
    for(int i = 0; i < iteration; i++)  //  ���ÿһ����gbest��ȡֵ
        printf("%d\t%g\n", i, fitness[i]);

    printf("Each dimension of gbest:\n");   //  �ҵ�����õ����ӵ�ÿһά��ȡֵ
    for(int i=0; i<dim; i++)
		printf("%g ", gbest[i]);
	printf("\ngbest_value = %f\n", gbest_value);
}



int main()
{
    srand(time(NULL));

    ifstream in;
    in.open("j1201_1.txt");
    for (int i = 0; i < point_num; i++)
    {
        for (int j = 0; j < point_num; j++)
        {
            in >> original_r[i][j];
        }
    }

    init();
    for(int i = 1; i < iteration; i++)
    {
        process();
        select();
    }
    output();
    return 0;
}