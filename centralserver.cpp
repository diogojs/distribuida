#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){
    int size, rank;
    unsigned int msg;
    unsigned int duration = 100;
    MPI_Status st;

    if(argc == 2){
        duration = atoi(argv[1]);
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello world from process %d out of %d processes.\n", rank, size);

    if (rank == 0) { // Mestre
        unsigned int token = 1;
        for (int i = 1; i < duration; i++) {
            printf("Central Server waiting...");
            MPI_Recv(&msg, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
        }    
    }
    else { // Escravos
      MPI_Recv(&msg, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
      pontos_no_circulo = compute_pi(0, msg);
      msg = pontos_no_circulo;
      MPI_Send(&msg, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
    }

  MPI_Finalize();
  return 0;
}


/*  srand(seed);
  
  for(i=0; i<pontos; i++){
    // sorteia um ponto: coordenadas x e y dentro do quadrado
    // consideramos que R = 1, então x e y pertencem ao intervalo [0; 1]
    x = (double)rand()/(double)(RAND_MAX);
    y = (double)rand()/(double)(RAND_MAX);      
    
    // verifica se o ponto sorteado encontra-se dentro do circulo
    // um ponto (x, y) esta dentro do circulo se: x^2 + y^2 < R^2
    // nesse caso, consideramos R = 1
    if( (x*x + y*y) < 1 ){
      pontos_no_circulo++;
    }      
  }
  
  return pontos_no_circulo;
}
*/