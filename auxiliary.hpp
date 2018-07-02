#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <set>
#include <utility>
#define MAX 50

double matrix[MAX][4]; //matriz utilizada para salvar informações sobre o tempo
// 0 - tempo inicial 
// 1 - tempo inter-babuíno
// 2 - tempo de início da travessia
// 3 - tempo de término da travessia
auto start = std::chrono::high_resolution_clock::now(); //variável global para armazenar o tempo

void sleep(int t)//função ultilizada para consumir um tempo t gasto por um babuíno
{
    auto start_time = std::chrono::high_resolution_clock::now();  
    while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() < t);
}

double averegeWaitTime()//calculo do tempo médio de espera com base na matriz de valores de cada babuíno
{
    double r = .0;
    for(int i = 0; i < MAX; i++)
    {
        r += (matrix[i][3] - matrix[i][0]);
    }
    return r / MAX;
}

double utilizationTime()//calculo do tempo de ultilização da corda com base na matriz de valores de cada babuíno
{
    std::set<std::pair<int, int>> times;
    for(int i = 0; i < 50; i++)
    {
        times.insert({matrix[i][2], matrix[i][3]});
    }
    double r = .0;
    std::pair<int, int> aux(0 ,0);
    for(auto p: times)
    {
        if(p.first <= aux.second and p.first >= aux.first)
        {
            aux = {std::min(aux.first, p.first), std::max(aux.second, p.second)};
        }
        else
        {
            r += (p.first - aux.second);
            aux = p;
        }
    }
    return matrix[MAX - 1][3] - r;
}
