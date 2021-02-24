#include "cg.h"


///////////////////////////////////////////////////////////////////////
//Return de time in a especific moment
double crono() {
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + tv.tv_usec / 1e6;
}


////////////////////////////////////////////////////////////////////////
//print a matrix
void print_matrix(matrix m) {
	int i;
	for(i=0; i<m.elements; i++)
		fprintf(stdout, "%d %d %20.19g\n", m.x[i], m.y[i], m.data[i]);
	}


////////////////////////////////////////////////////////////////////////
//print a vector 
void print_vector(vector m) {
	int i;
	for(i=0; i<m.lines; i++)
		//fprintf(stdout, "%d %10.9g\n", m.x[i], m.data[i]);
		fprintf(stdout, "%d %f\n", m.x[i], m.data[i]);
}


//////////////////////////////////////////////////////////////////
// Dot Vector
//////////////////////////////////////////////////////////////////
double vector_product(vector *v1, vector *v2) {
	double value = 0;
	int i;
	for (i = 0; i < v1->lines; i++)
		value += v1->data[i] * v2->data[i];
	return value;
}


//////////////////////////////////////////////////////////////////
// matrix_vector_product
//////////////////////////////////////////////////////////////////
void matrix_vector_product(matrix *m, vector *v1, vector *v2) {
	double value;
	int j = 0;
	int i;
	for(i = 0; i < m->lines; i++) {
		value = 0;
		while (m->x[j]  == i) {
			value += m->data[j] * v1->data[m->y[j]];
			j++;
			if (j == m->elements)
				break;
		}
		v2->data[i] = value;
	}
}


//////////////////////////////////////////////////////////////////
// Initialize vector
//////////////////////////////////////////////////////////////////
void initialize_vector(vector *b, matrix *m, vector *x, vector *r) {
	double value;
	int j = 0;
	int i;
	for (i = 0; i < m->lines; i++) {
		value = 0; 
		while (m->x[j] == i) {
			value += m->data[j] * x->data[m->y[j]];
			j++;
			if (j == m->elements)
				break;
		}
		r->data[i] = b->data[i] - value;
	}
}


//////////////////////////////////////////////////////////////////
// Get Alpha
//////////////////////////////////////////////////////////////////
double get_alpha(double rho, vector *v1, vector *v2) {
	return rho/vector_product(v1, v2);
}


//////////////////////////////////////////////////////////////////
// vector_product_plus
//////////////////////////////////////////////////////////////////
void scalar_product(vector *v1, vector *v2, vector *v3, double alpha) {
	int i;
	for (i= 0 ; i < v1->lines; i++)
		v3->data[i] = v1->data[i] + alpha*v2->data[i];
}


//////////////////////////////////////////////////////////////////
// Norm of a vector
//////////////////////////////////////////////////////////////////
double norm(vector *v) {
	//double res = vector_product(v, v);
	//res = sqrt(res);
	//return res;
	return sqrt(vector_product(v, v));
}


//////////////////////////////////////////////////////////////////
// Divide the before rho with the corrent rho
////////////////////////////////////////////////////////////////////
double get_rho(double rho0, double rho1) {
	return rho0/rho1;
}


//////////////////////////////////////////////////////////////////
// Initialize a vector
//////////////////////////////////////////////////////////////////
vector *vector_initialize(int lines) {	
	int i;
	vector *v = malloc(sizeof(vector));
	v->data = malloc(lines * sizeof(double));
	v->x = malloc(lines * sizeof(double));
	v->lines = lines;
	for(i = 0; i < lines; i++) {
		v->data[i] = 0;
		v->x[i] = i;
	}
	return v;
}


//////////////////////////////////////////////////////////////////
// CG
//////////////////////////////////////////////////////////////////

double CG(matrix *A, vector *b, vector *x, double tolerance, int iterations) {

	int i;
	double alpha;
	double beta;
	double rho_before;
	double rho_corrent;
	
	vector *p = vector_initialize(b->lines);
	vector *q = vector_initialize(b->lines);
	vector *r = vector_initialize(b->lines);

	//begin crono
	double execution = crono();

	double norm_b = norm(b);
	initialize_vector(b, A, x, r);
	rho_before = 1;
	rho_corrent = vector_product(r, r);
		
	for(i = 0; i < iterations; i++) {
		beta = rho_corrent/rho_before;
		scalar_product(r, p, p, beta);
		matrix_vector_product(A, p, q);
		
		alpha = get_alpha(rho_corrent, p, q);
		scalar_product(x, p, x, alpha);
		scalar_product(r, q, r, -alpha);
		print_vector(*x);

		if((rho_corrent/norm_b) <= tolerance) {
			fprintf(stderr, "Iterations:%d\n", i);
			fprintf(stderr, "Norm_x:%f\n", norm(x));
			break;
		}

		rho_before = rho_corrent;
		rho_corrent = vector_product(r, r);
	}
	//End crono
	return crono() - execution;
	//return norm(x); //
}

