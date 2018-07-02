#include "baboonRope.hpp"

int main ()
{ 
    std::thread threads[MAX];//criação dos babuínos
    turn = 'F';//variável turn iniciada com 'F' para indicar que a corda está livre
    count = 4; //quantidade máxima permitida na corda
    int qtdeW = 0; //quantidade de babuínos indo para a direção oeste
    srand(time(NULL));
    for (int i = 0; i < MAX; ++i)
    {
        int k = rand() % 2; //sorteio para indicar qual direção cada babuíno irá
        direction[i] = (k == 0? 'E': 'W');
        if(k == 1) qtdeW++; // quantidade de babuínos para a direção W
    }
    srand(time(NULL));
    for (int i = 0; i < MAX; ++i)
    {        
        int j = (rand() % 5) + 1;
        sleep(j);//tempo inicial gasto por cada babuino para chegar no canion
        mx.lock();//bloco protegido por mutex para não gerar incossistência ns prints
        double aux = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
        printf("Created - Baboon:%d %lf %c\n", i, aux, direction[i]);
        matrix[i][0] = aux;//armazenar o tempo inicial do babuíno i
        threads[i] = std::thread(cross, i, direction[i]);//criação do babuínp i
        queue.insert(i);//inserindo o babuíno na fila para apresentação dos que na fila estão
        mx.unlock();//libera o mutex mx
    }   
    
    for (int i = 0; i < MAX; ++i)
    { 
        threads[i].join();         
    }
    
    printf("\n   \tCreated   Climbing   Crossing   Final\n");//impressão da matriz com os tempos de cada babuíno
    for(int i = 0; i < MAX; i++)
    {
        std::cout << std::setw(4) << i << ' ';
        for(int j = 0; j < 4; j++)
            std::cout << std::setw(9) << std::fixed << std::setprecision(4) << matrix[i][j] << ' ';
        std::cout << (direction[i] == 'E'? " East": " West") << std::endl;
    }
    //impressão dos dados solicitados no corpo do trabalho
    printf("\n\nWest: %d - East: %d\nWaiting time: %.2lf\n", qtdeW, MAX - qtdeW, averegeWaitTime());
    int _min = (int)matrix[MAX - 1][3] / 60, _sec = (int)matrix[MAX - 1][3] % 60;
    //matrix[MAX - 1][3] = tempo total
    printf("Utilization Rate: %.2lf%%\nElapsed time: %d:%d\n",(utilizationTime() / matrix[MAX - 1][3]) * 100, _min, _sec);
    return 0;
}   
