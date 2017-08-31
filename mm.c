#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define UPPER_LIMIT 25
#define LOWER_LIMIT 4

void help(char *);
void allocate(float **, float **, float **, size_t);
void initialize(float *, float *, float *, size_t);
void multiply(float *, float *, float *, size_t);
void deallocate(float **, float **, float **);

int main(int argc, char *argv[]){

    /* Input parsing */
    if (argc != 2) { //requires SIZE parameter
        help(argv[0]);
        return -1;
    }
    uint32_t size_power = atoi(argv[1]);
    if (size_power > UPPER_LIMIT || size_power < LOWER_LIMIT) {
        help(argv[0]);
        return -2;
    }

    /* Matrix allocation */
    size_t side = 1 << size_power; //Matrix size size = 2^size_power
    float *A, *B, *C;              //Matrices
    allocate (&A, &B, &C, side);

    /* Input computation */
    initialize (A, B, C, side);

    /* Matrix multiplication */
    multiply (A, B, C, side);

    /* Simple output check */
    printf("C[0][0] = %6.2f\n",C[0]);

    /* Matrix deallocation */
    deallocate(&A, &B, &C);

    return 0;
}

void help(char* program_name ) {
    printf("Simple matrix multiplication code.\nExpected call:\n");
    printf("'%s SIZE', where SIZE is a value between %d and %d.\n",program_name,LOWER_LIMIT,UPPER_LIMIT);
    printf("The multiplied matrices will be of size 2^SIZE x 2^SIZE\n");
}

void allocate(float** A, float** B, float** C, size_t side){
    *A = (float*) malloc(side*side*sizeof(float));
    *B = (float*) malloc(side*side*sizeof(float));
    *C = (float*) malloc(side*side*sizeof(float));
}

void initialize(float * A, float * B, float * C, size_t side){
    size_t i, j;
    for (i = 0; i < side; ++i) {
        for (j = 0; j < side; ++j) {
            A[i*side+j] = 1.0;
            B[i*side+j] = 2.0;
            C[i*side+j] = 0.0;
        }
    }

}

void multiply(float * A, float * B, float * C, size_t side){
    size_t i, j, k, jj, kk;
    for (jj = 0; jj < side; jj += STEP) {
        for (kk = 0; kk < side; kk += STEP) {
            for (i = 0; i < side; ++i) {
                for (j = jj; j < jj+STEP; ++j) {
                    float r = 0.0;
                    for (k = kk; k < kk+STEP; ++k) {
                        r += A[i*side+k] * B[k*side+j];
                    }
                    C[i*side+j] += r;
                }
            }
        }
    }
}

void deallocate(float** A, float** B, float** C ){
    free(*A);
    free(*B);
    free(*C);
}
