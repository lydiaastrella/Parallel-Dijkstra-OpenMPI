// inisialisasi graf
// inisialisasi graf 3000 (besar)
// djikstra
// output txt
// ukur waktu

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

int** initializeGraf(int N){
    int **graf = (int **)malloc(N * sizeof(int*));
    for(int i = 0; i < N; i++) graf[i] = (int *)malloc(N * sizeof(int));
    int r;
    srand((unsigned) 13517031);
    for(int i =0; i<N; i++){
        for (int j=0; j<N; j++){
            r = rand() % 10;
            graf[i][j] = r;
        }
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

int main(){
    int N=3000;
    int ** graf = initializeGraf(N);
    struct timeval start, end;

    gettimeofday(&start, NULL);

    //printMatrix(graf, N);

    printOutput(graf,N,"tes.txt");

    gettimeofday(&end, NULL);

    int exectime = ((end.tv_sec - start.tv_sec) *1000000) + (end.tv_usec - start.tv_usec);
    printf("Execution time : %d microseconds\n", exectime);

    freeMatrix(graf, N);
    return 0;
}