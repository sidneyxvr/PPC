#include "auxiliary.hpp"

std::condition_variable cv; //variável condicional utilizado para sincronização e exclusão mutua juntamente com mtx e turn
int count; //variável limitadora da corda
std::mutex mx, mtx; //mx utilizado para sincronizar os prints
char direction[MAX], turn; //array de direções para os babuínos e uma variável turn com 3 estados('F', free; 'W', west; 'E', east) indicando qual direção é permitida no momento ou se está livre
std::set<int> crossing, queue; //conjuntos para armazenar os babuínos que estão na corda e os que estão na fila

void notify()
{
    std::unique_lock<std::mutex> lock(mtx);//toda a região abaixo é assegurada pelo mutex mtx, liberando apenas ao final da função
    ++count;
    if(count == 4)
    {
        turn = 'F';//quando a variável count for 4 a corda estará livre novamente
        cv.notify_all();//notifica todos de uma vez quando a corda estiver livre para dar a oportunidade de outros do sentido opsto atravessar
    }
}

void wait(char _turn, int i)
{
    std::unique_lock<std::mutex> lock(mtx);
    if(turn == 'F') turn = _turn;
    while(count == 0 or _turn != turn)
    {
        cv.wait(lock);
        if(turn == 'F') turn = _turn;
    }    
    --count;
    //=================================================
    //região ultilizada para apresentar informações
    double aux = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
    matrix[i][1] = aux;//armazenar o tempo que o babuíno entrou na na região de 1 segundp do babuíno i
    printf("Climbing - Baboon:%d %lf %c\n", i, aux, _turn);
    sleep(1);//tempo de 1 segundo inter-babuíno
    mx.lock();//região de variável compartilhada
    queue.erase(i);//retira o babuíno da fila e insere na corda na estrutura logo abaixo
    crossing.insert(i);
    //a impressão dos babuínos é dado pelo número do babuíno seguido de sua direcação('W' - West; 'E' - East)
    printf("\n\nOn rope: ");//imprime os babuínos que estão na corda
    for(int p: crossing) printf("%d%c ", p, direction[p]);
    printf("\nQueue: ");//imprime os babuínos que estão na fila
    for(int p: queue) printf("%d%c ", p, direction[p]);
    printf("\n\n\n");
    mx.unlock();
    aux = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
    printf("Crossing - Baboon:%d %lf %c\n", i, aux, _turn);
    matrix[i][2] = aux;//armazenar o tempo de início da travessia do babuíno i
    //===================================================
}

void cross(int i, char _turn)
{
    wait(_turn, i);//função ultilizada para testar a condição de travessia do babuíno i
    sleep(4);//tempo de travessia do babuíno
    double aux = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
    printf("final - Baboon:%d %lf %c\n", i, aux, _turn);
    matrix[i][3] = aux;//armazenar o tempo final do babuíno i
    mx.lock();//região de variável compartilhada
    crossing.erase(i);//retira o babuíno do conjunto corda ultilizado para a corda
    mx.unlock();
    notify();//função que imcrementa a variável count e verifica se a corda está livre para notificar os babuínos
}
