#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define maxn 100
#define maxm 100
int n, m;                   //n为进程的个数，m为资源类型的个数
int Source[maxm];        //总资源矩阵
int Available[maxm];        //可利用资源向量
int Claim[maxn][maxm];        //最大需求矩阵
int Allocation[maxn][maxm]; //分配矩阵
int Need[maxn][maxm];       //需求矩阵

int Request[maxm], id;       //请求向量
int rest[maxn];               //n个进程完成情况
int CurrentAvailable[maxm];             //工作空间
int Queue[maxn], Qi;        //存安全队列

bool CheckResourceIsEnough(int k) {//第k个进程需求资源小于声明需求
    for (int i = 0; i < m; i++) {
        if (Request[i] > Need[k][i]) return 0;
    }
    return 1;
}
bool CheckAvailableIsEnough(int k) {//第k个进程需求资源小于可利用资源
    for (int i = 0; i < m; i++) {
        if (Request[i] > Available[i]) return 0;
    }
    return 1;
}
void AllocateResource(int k) {//分配资源
    for (int i = 0; i < m; i++) {
        Available[i] -= Request[i];
    }
    for (int i = 0; i < m; i++) {
        Allocation[k][i] += Request[i];
    }
    for (int i = 0; i < m; i++) {
        Need[k][i] -= Request[i];
    }
}

void RollBack(int k) {
    for (int i = 0; i < m; i++) {
        Available[i] += Request[i];
    }
    for (int i = 0; i < m; i++) {
        Allocation[k][i] -= Request[i];

    }
    for (int i = 0; i < m; i++) {
        Need[k][i] += Request[i];
    }
}
//展示现状
void Display() {
    printf("--------------Current Situation Of Progress And Source--------------\n");
    printf("\t%10s%16s%10s\n", "Max", "allocation", "Need");
    printf("Progress Name\t");
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("    %d   ", i);
        printf("      ");
        for (int j = 0; j < m; j++)
            printf("%2d", Claim[i][j]);
        printf("      ");
        for (int j = 0; j < m; j++)
            printf("%2d", Allocation[i][j]);
        printf("      ");
        for (int j = 0; j < m; j++)
            printf("%2d", Need[i][j]);
        printf("\n");
    }
    printf("Available ");
    for (int i = 0; i < m; i++)
        printf("%3d", Available[i]);
    printf("\n -------------------------------------------------------------------\n");
}

bool Safe() {//安全性算法
    memset(rest, 1, sizeof(rest));
    memcpy(CurrentAvailable, Available, sizeof(int) * (m + 1));
    bool possible = true;
    Qi = 0;
    while (possible)
    {
        bool found = false;
        for (int i = 0; i < n; i++)
        {
            if (rest[i] == 0)
                continue;
            int count = 0;
            for (int j = 0; j < m; j++) {
                if (Need[i][j] <= CurrentAvailable[j])
                    count++;
            }
            if (count == m)
            {
                for (int j = 0; j < m; j++) {
                    CurrentAvailable[j] += Allocation[i][j];
                }
                rest[i] = 0;
                Queue[Qi++] = i;
                found = true;
            }
        }

        if (!found)
        {
            possible = false;
        }
    }

    //判断是否有进程无法完成并造成死锁
    for (int i = 0; i < n; i++) {
        if (rest[i]) return false;
    }
    return true;
}

int CheckSimulation() {

    if (!Safe()) {
        printf("Unsafe Initial Data\n");
        return 1;
    }

    while (1) {
        printf("\nPlease Input A Request Progress Id And Each Number Of Source In Need By the Progress\n");
        scanf("%d", &id);
        for (int i = 0; i < m; i++) {
            scanf("%d", &Request[i]);
        }
        if (CheckResourceIsEnough(id)) {//判断小于声明的
            if (!CheckAvailableIsEnough(id))//大于系统资源
                printf("Please Wait For Enough Source,Waiting.... \n");
            else {
                AllocateResource(id);
                if (Safe()) {//分配资源
                    printf("There Is A Safe Queue:");
                    //展示安全序列
                    for (int i = 0; i < Qi; i++) {
                        if (i) printf("->");
                        printf("%d", Queue[i]);
                    }
                    printf("\nAllocation Succeed！\n");
                    Display();
                }
                else {
                    RollBack(id);
                    printf("Unsafe,Allocation Fails\n");
                }
            }
        }
        else {
            printf("Error,Source In Need Exceeds The Boundary Of Source\n");
        }
    }
}

int main()
{
    //输入进程数以及资源数
    printf("Please Input The Number Of Process And Source\n");
    scanf("%d %d", &n, &m);

    //初始化，没有进程启动，输入需求矩阵Claim
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < m; j++)
            scanf("%d", &Claim[i][j]);

        for (int j = 0; j < m; j++)
            Allocation[i][j] = 0;

        for (int j = 0; j < m; j++)
            Need[i][j] = Claim[i][j] - Allocation[i][j];
    }

    //输入资源向量Available，因为初始未启动，Available等同于Source
    printf("Please Input Available Source In System\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &Available[i]);

    Display();
    CheckSimulation();
    return 0;
}