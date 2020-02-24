// inisialisasi graf
// inisialisasi graf 3000 (besar)
// djikstra
// output txt
// ukur waktu

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

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

int* djikstra(int** graf, int N, int src){

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

int main(){
    int N = 5;
    int ** graf = initializeGraf(N);
    struct timeval start, end;
    int **short_dis = (int **)malloc(N * sizeof(int*));
    for(int i = 0; i < N; i++) short_dis[i] = (int *)malloc(N * sizeof(int));

    gettimeofday(&start, NULL);

    for (int i=0; i<N; i++){
        short_dis[i] = djikstra(graf, N, i);
    }
    //printMatrix(graf, N);

    gettimeofday(&end, NULL);

    printOutput(short_dis,N,"tes.txt");

    int exectime = ((end.tv_sec - start.tv_sec) *1000000) + (end.tv_usec - start.tv_usec);
    printf("Execution time : %d microseconds\n", exectime);

    freeMatrix(graf, N);
    freeMatrix(short_dis, N);
    return 0;
}