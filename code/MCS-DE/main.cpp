//MCS-DE
//#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>

//下面是蒙特卡洛相关设置
#define point_num 6 //  节点的数目
#define Lambda 10
#define M 1000
#define Rc 0.95 //  想要达到的整体结构的稳定性
#define rc 0.8  //  每条边的稳定性的下界

//下面是DE相关设置
#define particle_num 15 //  粒子的数目
#define dim 9 //粒子的维度
#define xmax 1
#define xmin 0.8
#define CR 0.7
#define F 0.5
#define iteration 100



using namespace std;

const double Alpha[] = {120, 120, 90, 100, 65, 100, 90, 160, 160};
const double Beta[] = {14.7, 14.7, 8.75, 9.9, 5.64, 9.9, 8.75, 12, 12};


double x[particle_num][dim];
double v[particle_num][dim];
double u[particle_num][dim];
double gbest[dim];
double gbest_value = 10e20;
double fitness[iteration];  //  用来记录每一轮的最佳适应值

double r[dim];
int r2[point_num][point_num];    //  r2[i][j]表示的是i与j两点之间的距离
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


double MCS(const double *r, int simulation_replication) //  输入每条边的稳定性和蒙特卡洛的迭代次数，输出整个结构的稳定性
{
    int SUCCESS = 0;
    for(int i = 0; i < simulation_replication; i++)
    {
        for(int j = 0; j < point_num; j++)
            for(int k = 0; k < point_num; k++)
                r2[j][k] = 0;
        if(1.0*rand()/RAND_MAX < r[0])
            r2[0][1] = 1;
        if(1.0*rand()/RAND_MAX < r[1])
            r2[0][3] = 1;
        if(1.0*rand()/RAND_MAX < r[2])
            r2[1][3] = 1;
        if(1.0*rand()/RAND_MAX < r[2])
            r2[3][1] = 1;
        if(1.0*rand()/RAND_MAX < r[3])
            r2[1][2] = 1;
        if(1.0*rand()/RAND_MAX < r[3])
            r2[2][1] = 1;
        if(1.0*rand()/RAND_MAX < r[4])
            r2[2][3] = 1;
        if(1.0*rand()/RAND_MAX < r[4])
            r2[3][2] = 1;
        if(1.0*rand()/RAND_MAX < r[5])
            r2[3][4] = 1;
        if(1.0*rand()/RAND_MAX < r[5])
            r2[4][3] = 1;
        if(1.0*rand()/RAND_MAX < r[6])
            r2[2][4] = 1;
        if(1.0*rand()/RAND_MAX < r[6])
            r2[4][2] = 1;
        if(1.0*rand()/RAND_MAX < r[7])
            r2[2][5] = 1;
        if(1.0*rand()/RAND_MAX < r[8])
            r2[4][5] = 1;

        if(is_connected())
            SUCCESS++;
    }
    return (double)SUCCESS/simulation_replication;
}

double cost_function(double a, double b, double c)
{
    return a-b*log(1-c);
}

double cost_sum(const double *r)  //  输入每条边的稳定性，输出总的cost
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
    for(int i = 0; i < iteration; i++)  //  输出每一代的gbest的取值
        printf("%d\t%g\n", i, fitness[i]);

    printf("Each dimension of gbest:\n");   //  找到的最好的粒子的每一维的取值
    for(int i=0; i<dim; i++)
		printf("%g ", gbest[i]);
	printf("\ngbest_value = %f\n", gbest_value);
}



int _tmain(int argc, _TCHAR* argv[])
{
    srand(time(NULL));
    init();
    for(int i = 1; i < iteration; i++)
    {
        process();
        select();
    }
    output();
    return 0;
}
