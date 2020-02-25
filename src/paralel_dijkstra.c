#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <mpi.h>

#define N_MAX 9999

int** initializeGraf(int N){
    int **graf = (int **)malloc(N * sizeof(int*));
    for(int i = 0; i < N; i++) graf[i] = (int *)malloc(N * sizeof(int));
    int r;
    int pembatas=0;
    srand((unsigned) 13517031);
    for(int i =0; i<N; i++){
        for (int j=0; j<N-pembatas; j++){
            r = rand() % 10;
            graf[i][j] = r;
            graf[j][i] = r;
        }
        pembatas +=1;
    }
    return graf;
}

int sizeArray (int* array){
    size_t length = sizeof(array) / sizeof(array[0]);
    return length;
}

void freeMatrix(int ** matrix, int N){
    for (int i=0; i<N; i++){
        free(matrix[i]);
    }
}

void printOutput(int** matrix, int N, char* filename ){
    FILE *out_file = fopen(filename, "w");
    if (out_file == NULL){
        printf("Error, Could not find file\n");
        exit(-1);
    }
    for (int i=0; i<N; i++){
        fprintf(out_file, "jarak dari node %d: ", i);
        for(int j=0; j<N; j++){
            fprintf(out_file, "%d ", matrix[i][j]);
        }
        fprintf(out_file,"\n");
    }
    fclose(out_file);
}

//for debugging
void printMatrix(int ** matrix, int N){
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int minDist(int* array, int* included, int N){
    int minIdx = 0;
    int min = N_MAX;
    for (int i = 0; i < N; i++){
        if (array[i] <= min ){
            if (included[i] == 0){
                min = array[i];
                minIdx = i;
            }
        }
    }
    return minIdx;
}

int* dijkstra(int** graf, int N, int src){

    int* shortestDist = (int*) malloc( N * sizeof(int));
    int* included =  (int*) malloc(N * sizeof(int));
    int minIdx;

    if (shortestDist != NULL && included != NULL){
        for (int i = 0; i < N; i++ ){
           shortestDist[i] = N_MAX;
           included[i] = 0;
        }
        
        shortestDist[src] = 0;

        for (int i = 0; i < N -1; i++){
            minIdx = minDist(shortestDist, included, N);
            
            included[minIdx] = 1;

            for (int j = 0; j < N; j++){
                if (included[j] == 0 && shortestDist[minIdx] != N_MAX && graf[minIdx][j] != 0){
                    if (graf[minIdx][j] + shortestDist[minIdx] < shortestDist[j]){
                        shortestDist[j] = graf[minIdx][j] + shortestDist[minIdx];
                    }
                }
            }
        }
        free(included);
        return shortestDist;
    }
}

int main(int argc, char** argv){
    int N;
    struct timeval start, end; 
    int *global_short_dis = NULL;
    int **graf;
    int **short_dis;
    int *array_short_dis;
    int my_rank, num_of_process, node_per_process;

    MPI_Comm comm;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_process);
    N=4;
    node_per_process = N/num_of_process; //asumsi N adalah kelipatan num_of_process
    
    /*if(my_rank==0){
        graf = initializeGraf(N);
        MPI_Bcast(graf, N*N, MPI_INT, 0, comm);
    }*/
   
    graf = initializeGraf(N);

    short_dis = (int **)malloc(node_per_process * sizeof(int*));
    for(int i = 0; i < node_per_process; i++) short_dis[i] = (int *)malloc(N * sizeof(int));

    array_short_dis = (int *) malloc(node_per_process*N * sizeof(int));

    printf("%d\n",my_rank);
    if(my_rank==0){
        gettimeofday(&start, NULL);
    }

    printf("sebelum for djikstra\n");
    for(int i_rank=0; i_rank < num_of_process; i_rank++){
        if(i_rank == my_rank){
            for (int i=0; i<node_per_process;i++){
                short_dis[i] = dijkstra(graf, N, my_rank*node_per_process + i);
                printf("%d : short_dis[i]=%d\n",my_rank,short_dis[i][0]);
                for(int j=0; j< N ;j++){
                    printf("%d : short_dis[%d][%d] : %d \n",my_rank,i,j, short_dis[i][j]);
                }
            }
            for (int i=0; i<node_per_process;i++){
                for (int j=0; j<N; j++){
                    array_short_dis[i*N+j] = short_dis[i][j];
                }
            }
        }
    }
    printf("%d: selesai djisktra\n",my_rank);

    printf("%d : last_idx = %d\n",my_rank, N*node_per_process-1);
    printf("node + N %d %d \n", node_per_process, N);
    for(int j=0; j< node_per_process* N ;j++){
        printf(" %d ", array_short_dis[j]);
    }
    printf("\n%d : array short dis (last_idx) : %d\n",my_rank, array_short_dis[N*node_per_process-1]);

    //printMatrix(graf, N);

    if (my_rank==0){
        
        printf("masuk my_rank 0\n");
        //short_dis = (int **)malloc(node_per_process * sizeof(int*));
        //for(int i = 0; i < node_per_process; i++) short_dis[i] = (int *)malloc(N * sizeof(int));

        //global_short_dis = (int **)malloc(N * sizeof(int*));
        //for(int i = 0; i < N; i++) global_short_dis[i] = (int *)malloc(N * sizeof(int));
        global_short_dis = (int *)malloc(N * N * sizeof(int));        
        printf("beres inisialisasi global short dis\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(array_short_dis, node_per_process * N, MPI_INT, global_short_dis, node_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);
        
    printf("beres gather \n");

    if(my_rank==0){
        gettimeofday(&end, NULL);

        //printOutput(global_short_dis,N,"tes.txt");

        int exectime = ((end.tv_sec - start.tv_sec) *1000000) + (end.tv_usec - start.tv_usec);
        printf("Execution time : %d microseconds\n", exectime);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if(my_rank==0){
        free(global_short_dis);
    }

    freeMatrix(graf, N);
    for (int i=0; i<node_per_process; i++){
        free(short_dis[i]);
    }
    free(short_dis);
    free(array_short_dis);
    MPI_Finalize();
    return 0;
}