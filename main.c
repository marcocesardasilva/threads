#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_TENANT_THREADS 5
#define NUM_BROKER_THREADS 3


typedef struct
{
    int codigo;
    char endereco[250];
    float preco;

} Imovel;
Imovel imovelDisponivel[10];
Imovel imovelEntregue[10];

pthread_mutex_t mutexImovelDisponivel;
pthread_mutex_t mutexImovelEntregue;

void CadastraImoveisDisponiveis()
{
    imovelDisponivel[0].codigo = 1;
    strcpy(imovelDisponivel[0].endereco, "Avenida Beira Mar, 100");
    imovelDisponivel[0].preco = 21000.00;

    imovelDisponivel[1].codigo = 2;
    strcpy(imovelDisponivel[1].endereco, "Avenida Paulo Fontes, 50");
    imovelDisponivel[1].preco = 2000.00;

    imovelDisponivel[2].codigo = 3;
    strcpy(imovelDisponivel[2].endereco, "Avenida das Rendeiras, 1234");
    imovelDisponivel[2].preco = 2350.20;

    imovelDisponivel[3].codigo = 4;
    strcpy(imovelDisponivel[3].endereco, "Rua das Corticeiras, 349");
    imovelDisponivel[3].preco = 1890.00;

    imovelDisponivel[4].codigo = 5;
    strcpy(imovelDisponivel[4].endereco, "Rua Joao Pio Duarte Silva, 3942");
    imovelDisponivel[4].preco = 1500.00;

    imovelDisponivel[5].codigo = 6;
    strcpy(imovelDisponivel[5].endereco, "Rua Lauro Linhares, 2879");
    imovelDisponivel[5].preco = 1700.00;

    imovelDisponivel[6].codigo = 7;
    strcpy(imovelDisponivel[6].endereco, "Rua Deodoro");
    imovelDisponivel[6].preco = 15000.00;

    imovelDisponivel[7].codigo = 8;
    strcpy(imovelDisponivel[7].endereco, "Rodovia Baldicero Filomeno, 5973");
    imovelDisponivel[7].preco = 950.00;

    imovelDisponivel[8].codigo = 9;
    strcpy(imovelDisponivel[8].endereco, "Avenida Madre Benvenuta, 4982");
    imovelDisponivel[8].preco = 7500.00;

    imovelDisponivel[9].codigo = 10;
    strcpy(imovelDisponivel[9].endereco, "Rua Delfino Conti");
    imovelDisponivel[9].preco = 2800.00;
}

void instantiate_delivered_properties(){
    for(int k=0; k<10; k++)
    {
        imovelEntregue[k].codigo = -1;
        strcpy(imovelEntregue[k].endereco, "");
        imovelEntregue[k].preco = -1.00;
    }
}

int get_random_number(int max_random_number) {
  int random_number = rand() % max_random_number;
  return random_number;
}

int rent_property(int tenant) {
    for(int y = 0; y < 10; y++) {
      int index = get_random_number(10);
      pthread_mutex_lock(&mutexImovelDisponivel);
      if(imovelDisponivel[index].codigo != -1) {
        Imovel propriedade;
        propriedade.codigo = imovelDisponivel[index].codigo;
        strcpy(propriedade.endereco, imovelDisponivel[index].endereco);
        propriedade.preco = imovelDisponivel[index].preco;
        imovelDisponivel[index].codigo = -1;
        strcpy(imovelDisponivel[index].endereco, "");
        imovelDisponivel[index].preco = -1.00;
        pthread_mutex_unlock(&mutexImovelDisponivel);
        printf("Inquilino %d alugando imóvel %d, localizado em %s, no valor de %.2f\n", tenant, propriedade.codigo, propriedade.endereco, propriedade.preco);
        int delivered_list_index;
        for(delivered_list_index = 0; delivered_list_index < 10; delivered_list_index++)
        {
          pthread_mutex_lock(&mutexImovelEntregue);
          if(imovelEntregue[delivered_list_index].codigo == -1)
          {
              imovelEntregue[delivered_list_index].codigo = propriedade.codigo;
              strcpy(imovelEntregue[delivered_list_index].endereco, propriedade.endereco);
              imovelEntregue[delivered_list_index].preco = propriedade.preco;
              break;
          }
          pthread_mutex_unlock(&mutexImovelEntregue);
        }
        return delivered_list_index;
      } else {
        pthread_mutex_unlock(&mutexImovelDisponivel);
        return -1;
      }
    }
}

void return_property(int index, int tenant) {
  Imovel propriedade;
  pthread_mutex_lock(&mutexImovelEntregue);
  propriedade.codigo = imovelEntregue[index].codigo;
  strcpy(propriedade.endereco, imovelEntregue[index].endereco);
  propriedade.preco = imovelEntregue[index].preco;
  imovelEntregue[index].codigo = -1;
  strcpy(imovelEntregue[index].endereco, "");
  imovelEntregue[index].preco = -1.00;
  pthread_mutex_unlock(&mutexImovelEntregue);
  for(int y = 0; y < 10; y++)
  {
      pthread_mutex_lock(&mutexImovelDisponivel);
      if(imovelDisponivel[y].codigo == -1)
      {
          imovelDisponivel[y].codigo = propriedade.codigo;
          strcpy(imovelDisponivel[y].endereco, propriedade.endereco);
          imovelDisponivel[y].preco = propriedade.preco;
          printf("Inquilino %d devolvendo imóvel %d, localizado em %s, no valor de %.2f\n", tenant, propriedade.codigo, propriedade.endereco, propriedade.preco);
          break;
      }
      pthread_mutex_unlock(&mutexImovelDisponivel);
  }
}

void *broker_thread_function(void *arg) {
}

void *tenant_thread_function(void *arg) {
  while (1){
    int *tenant = (int *)arg;
    int index = rent_property(*tenant);
    if (index != -1) {
      sleep(get_random_number(5));
      return_property(index, *tenant);
    }
  }
}

int main() {
  pthread_t tenant_threads[NUM_TENANT_THREADS];
  pthread_t corretor_threads[NUM_BROKER_THREADS];

  pthread_mutex_init(&mutexImovelDisponivel, NULL);
  pthread_mutex_init(&mutexImovelEntregue, NULL);
  pthread_mutex_lock(&mutexImovelEntregue);
  instantiate_delivered_properties();
  pthread_mutex_unlock(&mutexImovelEntregue);
  pthread_mutex_lock(&mutexImovelDisponivel);
  CadastraImoveisDisponiveis();
  pthread_mutex_unlock(&mutexImovelDisponivel);
  for(int t=0; t<NUM_TENANT_THREADS; t++){
      printf("In main: creating tenant thread %d\n", t);
      int res_thread;
      res_thread = pthread_create(&tenant_threads[t], NULL, tenant_thread_function, &t);
      if (res_thread){
          printf("ERROR; return code from pthread_create() is %d\n", res_thread);
          exit(-1);
      }
  }
  for(int t=0; t<NUM_BROKER_THREADS; t++)
  {
      printf("In main: creating broker thread %d\n", t);
      int cor_thread;
      cor_thread = pthread_create(&corretor_threads[t], NULL, broker_thread_function, NULL);
      if(cor_thread)
      {
          printf("ERROR; return code from pthread_create() is %d\n", cor_thread);
          exit(-1);
      }
  }
  for(int t=0; t<NUM_TENANT_THREADS; t++){
      pthread_join(tenant_threads[t], NULL);
  }
  for(int t=0; t<NUM_BROKER_THREADS; t++){
      pthread_join(corretor_threads[t], NULL);
  }
  return 0;
}
