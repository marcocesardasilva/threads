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
    char bairro[250];

} Imovel;
Imovel imovelDisponivel[20];
Imovel imovelEntregue[20];

int contador = 10;

pthread_mutex_t mutex;

void CadastraImoveisDisponiveis()
{
    imovelDisponivel[0].codigo = 1;
    strcpy(imovelDisponivel[0].endereco, "Avenida Beira Mar, 100");
    imovelDisponivel[0].preco = 21000.00;
    strcpy(imovelDisponivel[0].bairro, "Beira Mar Norte");

    imovelDisponivel[1].codigo = 2;
    strcpy(imovelDisponivel[1].endereco, "Avenida Paulo Fontes, 50");
    imovelDisponivel[1].preco = 2000.00;
    strcpy(imovelDisponivel[1].bairro, "Centro");

    imovelDisponivel[2].codigo = 3;
    strcpy(imovelDisponivel[2].endereco, "Avenida das Rendeiras, 1234");
    imovelDisponivel[2].preco = 2350.20;
    strcpy(imovelDisponivel[2].bairro, "Lagoa da Conceicao");

    imovelDisponivel[3].codigo = 4;
    strcpy(imovelDisponivel[3].endereco, "Rua das Corticeiras, 349");
    imovelDisponivel[3].preco = 1890.00;
    strcpy(imovelDisponivel[3].bairro, "Campeche");

    imovelDisponivel[4].codigo = 5;
    strcpy(imovelDisponivel[4].endereco, "Rua Joao Pio Duarte Silva, 3942");
    imovelDisponivel[4].preco = 1500.00;
    strcpy(imovelDisponivel[4].bairro, "Corrego Grande");

    imovelDisponivel[5].codigo = 6;
    strcpy(imovelDisponivel[5].endereco, "Rua Lauro Linhares, 2879");
    imovelDisponivel[5].preco = 1700.00;
    strcpy(imovelDisponivel[5].bairro, "Trindade");

    imovelDisponivel[6].codigo = 7;
    strcpy(imovelDisponivel[6].endereco, "Rua Deodoro");
    imovelDisponivel[6].preco = 15000.00;
    strcpy(imovelDisponivel[6].bairro, "Centro");

    imovelDisponivel[7].codigo = 8;
    strcpy(imovelDisponivel[7].endereco, "Rodovia Baldicero Filomeno, 5973");
    imovelDisponivel[7].preco = 950.00;
    strcpy(imovelDisponivel[7].bairro, "Ribeirao da Ilha");

    imovelDisponivel[8].codigo = 9;
    strcpy(imovelDisponivel[8].endereco, "Avenida Madre Benvenuta, 4982");
    imovelDisponivel[8].preco = 7500.00;
    strcpy(imovelDisponivel[8].bairro, "Santa Monica");

    imovelDisponivel[9].codigo = 10;
    strcpy(imovelDisponivel[9].endereco, "Rua Delfino Conti");
    imovelDisponivel[9].preco = 2800.00;
    strcpy(imovelDisponivel[9].bairro, "Trindade");

    int r;
    for(r = 10; r < 20; r++)
    {
        imovelDisponivel[r].codigo = -1;
        strcpy(imovelDisponivel[r].endereco, " ");
        imovelDisponivel[r].preco = -1.00;
        strcpy(imovelDisponivel[r].bairro, " ");
    }
}

void instantiate_delivered_properties(){
    for(int k=0; k<20; k++)
    {
        imovelEntregue[k].codigo = -1;
        strcpy(imovelEntregue[k].endereco, "");
        imovelEntregue[k].preco = -1.00;
        strcpy(imovelEntregue[k].bairro, "");
    }
}

int get_random_number(int max)
{
  int max_random_number = max;
  srand((unsigned)time(NULL));
  int random_number = rand() % max_random_number;
  return random_number;
}

void rent_property()
{
  while(1)
  {
      int identificador = get_random_number(20);
      pthread_mutex_lock(&mutex);
      if(imovelDisponivel[identificador].codigo != -1 && imovelDisponivel[identificador].codigo != 0)
      {
          Imovel propriedade;
          propriedade.codigo = imovelDisponivel[identificador].codigo;
          strcpy(propriedade.endereco, imovelDisponivel[identificador].endereco);
          propriedade.preco = imovelDisponivel[identificador].preco;
          strcpy(propriedade.bairro, imovelDisponivel[identificador].bairro);

          imovelDisponivel[identificador].codigo = -1;
          strcpy(imovelDisponivel[identificador].endereco, "");
          imovelDisponivel[identificador].preco = -1.00;
          strcpy(imovelDisponivel[identificador].bairro, "");
          printf("alugando imovel %d, localizado no bairro %s em %s, no valor de %.2f\n", propriedade.codigo, propriedade.bairro, propriedade.endereco, propriedade.preco);

          pthread_mutex_unlock(&mutex);
          sleep(get_random_number(10));
          pthread_mutex_lock(&mutex);

          int i;
          for(i = 0; i < 20; i++)
          {
              if(imovelEntregue[i].codigo == -1)
              {
                  imovelEntregue[i].codigo = propriedade.codigo;
                  strcpy(imovelEntregue[i].endereco, propriedade.endereco);
                  imovelEntregue[i].preco = propriedade.preco;
                  strcpy(imovelEntregue[i].bairro, propriedade.bairro);
                  break;
              }
          }
          pthread_mutex_unlock(&mutex);
          break;
      }
      pthread_mutex_unlock(&mutex);
  }
}

void return_property() {
  int l = 0;
  while(1)
  {
      pthread_mutex_lock(&mutex);
      if(imovelEntregue[l].codigo != -1 && imovelEntregue[l].codigo != 0)
      {
          Imovel propriedade;

          propriedade.codigo = imovelEntregue[l].codigo;
          strcpy(propriedade.endereco, imovelEntregue[l].endereco);
          propriedade.preco = imovelEntregue[l].preco;
          strcpy(propriedade.bairro, imovelEntregue[l].bairro);

          imovelEntregue[l].codigo = -1;
          strcpy(imovelEntregue[l].endereco, "");
          imovelEntregue[l].preco = -1.00;
          strcpy(imovelEntregue[l].bairro, "");

          int y;
          for(y = 0; y < 20; y++)
          {
              if(imovelDisponivel[y].codigo == -1)
              {
                  imovelDisponivel[y].codigo = propriedade.codigo;
                  strcpy(imovelDisponivel[y].endereco, propriedade.endereco);
                  imovelDisponivel[y].preco = propriedade.preco;
                  printf("devolvendo imovel %d, localizado no bairro %s em %s, no valor de %.2f\n", propriedade.codigo, propriedade.bairro, propriedade.endereco, propriedade.preco);
                  break;
              }
          }
          pthread_mutex_unlock(&mutex);
          break;
      }
      pthread_mutex_unlock(&mutex);
      l++;
      if(l > 19)
      {
          l = 0;
      }
  }
}

void add_property()
{
    int k;
    pthread_mutex_lock(&mutex);
    for(k = 0; k < 20; k++)
    {
        if(imovelDisponivel[k].codigo == -1)
        {
            imovelDisponivel[k].codigo = contador;
            contador++;
            strcpy(imovelDisponivel[k].endereco, "Residencial Novo Campeche, 2022");
            imovelDisponivel[k].preco = 1500.00;
            strcpy(imovelDisponivel[k].bairro, "Novo Campeche");
            printf("adicionando imovel %d, localizado no bairro %s em %s, no valor de %.2f\n", imovelDisponivel[k].codigo, imovelDisponivel[k].bairro, imovelDisponivel[k].endereco, imovelDisponivel[k].preco);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void remove_property()
{
    int idRemover;
    while(1)
    {
        idRemover = get_random_number(20);
        pthread_mutex_lock(&mutex);
        if(imovelDisponivel[idRemover].codigo != -1 && imovelDisponivel[idRemover].codigo != 0)
        {
            printf("removendo o imovel %d, localizado no bairro %s em %s, no valor de %.2f da base de dados\n", imovelDisponivel[idRemover].codigo, imovelDisponivel[idRemover].bairro, imovelDisponivel[idRemover].endereco, imovelDisponivel[idRemover].preco);
            imovelDisponivel[idRemover].codigo = -1;
            strcpy(imovelDisponivel[idRemover].endereco, " ");
            imovelDisponivel[idRemover].preco = -1.00;
            strcpy(imovelDisponivel[idRemover].bairro, " ");
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void *broker_thread_function(void *arg)
{
    sleep(get_random_number(10));
    int casoUso = get_random_number(3);
    switch(casoUso)
    {
    case 0:
        return_property();
        break;
    case 1:
        add_property();
        break;
    case 2:
        remove_property();
        break;
    }
}

void *tenant_thread_function(void *arg)
{
    rent_property();
}

void main()
{
    pthread_t tenant_threads[NUM_TENANT_THREADS];
    pthread_t corretor_threads[NUM_BROKER_THREADS];

    instantiate_delivered_properties();
    CadastraImoveisDisponiveis();
    pthread_mutex_init(&mutex, NULL);
    for(int t=0; t<NUM_TENANT_THREADS; t++){
        printf("In main: creating tenant thread %ld\n", t);
        int res_thread;
        res_thread = pthread_create(&tenant_threads[t], NULL, tenant_thread_function, NULL);
        if (res_thread){
            printf("ERROR; return code from pthread_create() is %d\n", res_thread);
            exit(-1);
        }
    }
    for(int t=0; t<NUM_BROKER_THREADS; t++)
    {
        printf("In main: creating broker thread %ld\n", t);
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
    pthread_mutex_destroy(&mutex);
}
