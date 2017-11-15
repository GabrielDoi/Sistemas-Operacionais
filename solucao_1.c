//filosofo come apenas uma vez 
//todos os filosofos comem 
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

#define N 5 //total de filosofos
#define PENSAR 0
#define COMENDO 2
#define ESQUERDA (nfilosofo+4)%N //agarrar garfo
                                 //da esquerda
                                 
#define DIREITA (nfilosofo+1)%N  //agarrar garfo
                                 //da direita
                                 
//thread
void *filosofo(void *num);

//funcoes
void agarraGarfo(int);
void deixaGarfo(int);

//variaveis globais
//semaforo da regiao critica
sem_t mutex;

//estado do filosofo
int estado[N];
//id do filosofo
int nfilosofo[N]={0,1,2,3,4};

//thread
void *filosofo(void *num)
{
  int *i = num;
  sleep(1);
  agarraGarfo(*i);
  sleep(1);
  //deixaGarfo(*i);
}

void agarraGarfo(int nfilosofo)
{
	while(1){
		printf("\nVai pegar um Garfo %d\n",nfilosofo+1);
		
		//verificação se alguem esta usando a regiao critica
		//se sim ele entra no estado bloqueado ate que alguem pare de usar
	    sem_wait(&mutex);
	    
	    //verifica se o meu lado esquerdo e direito nao esta comendo
	    if(estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO){
	    	//quer dizer que nem uma lado meu esta comendo entao eu pego os garfos
	   		printf("\nPegou um Garfo %d\n",nfilosofo+1);
	   		
	   		//esse é variavel critica meu estado comendo
	   		estado[nfilosofo] = COMENDO;
	   		printf("\nFilosofo %d esta Comendo.-----------------------------------------\n", nfilosofo+1);
	   		
	   		//libero minha regiao critica pq ja usei
	   		sem_post(&mutex);
	   		
	   		//deixo os grfos porque eu como rapido
	   		deixaGarfo(nfilosofo);
	   		
	   		//se ja deixei meus garfos eu paro de trabalhar e me mato
	   		//pthread_exit(NULL);
	   		
	    }else{
	    	//caso a verificação der que algum lado meu esta comendo
	   	 	printf("\nNao Pegou um Garfo %d\n",nfilosofo+1);
	   	 	
	   	 	//apenas libero minha regiao critica pq ja usei
			sem_post(&mutex);
	    }
	   
	   sleep(1);
	}//continuo o while para verificar se ja liberou os garfos
}

void deixaGarfo(int nfilosofo)
{
	//vou deixar meus garfos ainda nao deixei 
	//ainda estou comendo aquele rapinha de comida do prato
	printf("\nVai deixar os garfos mais ainda esta comendo %d\n",nfilosofo+1);
	sleep(1);
	
	//verifico se alguem esta usando a regiao critica 
	//se sim eu ainda continuo comendo e entro no estado bloqueado ate alguem parar de usar
    sem_wait(&mutex);
    
    //aqui sim estou deixando o meu garfo para outro pegar 
    estado[nfilosofo]=PENSAR;
    
    printf("\nFilosofo %d deixou os garfos %d e %d.\n", nfilosofo+1, ESQUERDA+1, nfilosofo+1);
    printf("\nFilosofo %d esta a pensar.\n", nfilosofo+1);
   
   //libero minha regiao critica para outros poderem pegar os meus garfos
    sem_post(&mutex);
}


int main() {
   int i;
   pthread_t thread_id[N]; //identificadores das
                           //threads
                           
   sem_init(&mutex,0,1); //inicializa o semaforo da regial critica
      
   for(i=0;i<N;i++)
   {
      pthread_create(&thread_id[i], NULL, filosofo, &nfilosofo[i]);
      //criar as threads
      printf("Filosofo %d esta a pensar.\n",i+1);
   }
   
   //inicializa as threads
   for(i=0;i<N;i++)
   	pthread_join(thread_id[i],NULL); //para
                                    //fazer a junção das threads
   return(0);
}
