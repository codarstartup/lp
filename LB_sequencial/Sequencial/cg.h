#include <stdio.h>      //Input/Output
#include <stdlib.h>     //Conversions
#include <math.h>       //Math functions and conversions
#include <malloc.h>     //Memory manangement
#include <sys/time.h>   //Counting the time


typedef struct {
	double *data;
	int *x;
	int *y;
	int elements;
	int lines;
} matrix;



typedef struct {
	double *data;
	int *x;
	int lines;
} vector;


double vector_product(vector *v1, vector *v2);
void matrix_vector_product(matrix *m, vector *v1, vector *v2);
void initialize_vector(vector *b, matrix *m, vector *x, vector *r);
double get_alpha(double rho, vector *v1, vector *v2);
void scalar_product(vector *v1, vector *v2, vector *v3, double alpha);
double norm(vector *v);
double get_rho(double rho0, double rho1);
vector *vector_initialize(int lines);
double CG(matrix *A, vector *b, vector *x, double tolerance, int iterations);

double crono();
void print_vector(vector v);
void print_matrix(matrix m);

