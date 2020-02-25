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
    int **global_short_dis;
    int *short_dis;
    int my_rank, num_of_process, node_per_process;

    MPI_Comm comm;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_process);
    node_per_process = num_of_process / N;

    N=100;
    if(my_rank==0){
        graf = initializeGraf(N);
        MPI_Bcast(graf, N*N, MPI_INT, 0, comm);
    }
   
    short_dis = (int *)malloc(N * sizeof(int));

    if(my_rank==0){
        gettimeofday(&start, NULL);
    }

    short_dis = dijkstra(graf, N, i);
    //printMatrix(graf, N);

    if (my_rank==0){
        global_short_dis = (int**)malloc(N * sizeof(int*));
        for(int i = 0; i < N; i++) global_short_dis[i] = (int *)malloc(N * sizeof(int));
        MPI_Gather(short_dis, N, MPI_INT, global_short_dis, N, MPI_INT, 0, comm);
        
        gettimeofday(&end, NULL);

        printOutput(globalshort_dis,N,"tes.txt");

        int exectime = ((end.tv_sec - start.tv_sec) *1000000) + (end.tv_usec - start.tv_usec);
        printf("Execution time : %d microseconds\n", exectime);

        freeMatrix(global_short_dis, N);
    }else{
        MPI_Gather(short_dis, N, MPI_INT, global_short_dis, N, MPI_INT, 0 comm);
    }

    freeMatrix(graf, N);
    free(short_dis);
    MPI_FINALIZE();
    return 0;
}