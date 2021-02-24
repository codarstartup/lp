// Including structs and others headers
#include "cg.h"		//CG Functions
#include "mmio.h"       //Matrix Market Format


//////////////////////////////////////////////////////////////////////
//return a vector
vector *get_vector(FILE *f) {

	int i;
	vector *m = malloc(sizeof(matrix));
	MM_typecode matcode;
	int ret_code;
	int N;

	if (mm_read_banner(f, &matcode) != 0) {
		printf("Could not process Matrix Market banner.\n");
		exit(-1);
	}
	
	//  This is how one can screen matrix types if their application 
	//  only supports a subset of the Matrix Market data types.      
	if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)) {
		printf("Sorry, this application does not support ");
		printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
		exit(-2);
	}
	
	// find out size of sparse matrix .... 
	if ((ret_code = mm_read_mtx_array_size(f, &m->lines, &N)) !=0)
		exit(-3);
	
	// reseve memory for matrices
	m->x = (int *) malloc(m->lines * sizeof(int));
	m->data = (double *) malloc(m->lines * sizeof(double));
	
	// NOTE: when reading in doubles, ANSI C requires the use of the "l"  
	//   specifier as in "%lg", "%lf", "%le", otherwise errors will occur 
	//  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            


	for (i=0; i<m->lines; i++) {
		fscanf(f, "%d %lg\n", &m->x[i], &m->data[i]);
		m->x[i]--;  // adjust from 1-based to 0-based
	}
	
	//closing archive
	if (f !=stdin)
		fclose(f);

	//return a struct matrix
	return m;
}


////////////////////////////////////////////////////////////////////////
//return a matrix
matrix *get_matrix(FILE *f) {

	int i;
	MM_typecode matcode;
	int ret_code;
	matrix *m = malloc(sizeof(matrix));

	if (mm_read_banner(f, &matcode) != 0) {
		printf("Could not process Matrix Market banner.\n");
		exit(-1);
	}
	
	//  This is how one can screen matrix types if their application 
	//  only supports a subset of the Matrix Market data types.      
	if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)) {
		printf("Sorry, this application does not support ");
		printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
		exit(-2);
	}
	
	// find out size of sparse matrix .... 
	if ((ret_code = mm_read_mtx_crd_size(f, &m->lines, &m->lines, &m->elements)) !=0)
		exit(-3);
	
	// reseve memory for matrices 
	m->x = (int *) malloc(m->elements * sizeof(int));
	m->y = (int *) malloc(m->elements * sizeof(int));
	m->data = (double *) malloc(m->elements * sizeof(double));
	
	// NOTE: when reading in doubles, ANSI C requires the use of the "l"  
	//   specifier as in "%lg", "%lf", "%le", otherwise errors will occur 

	for (i=0; i<m->elements; i++) {
		fscanf(f, "%d %d %lg\n", &m->x[i], &m->y[i], &m->data[i]);
		m->x[i]--;  // adjust from 1-based to 0-based
		m->y[i]--;
	}
	
	//closing archive
	if (f !=stdin)
		fclose(f);

	//return a struct matrix
	return m;
}


////////////////////////////////////////////////////////////////////////
// Test GC
////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

	double execution;
        double norm_x;

	int iterations;
	double tolerance;

	FILE *file_A, *file_b, *file_x;

	matrix *A;
	vector *b;
	vector *x;
	
	//Checking arguments
	if (argc !=6) {
		fprintf(stderr, "Usage: %s [matrix_A] [vector_b] [vector_x] [tolerance] [iterations]\n\n", argv[0]);
		exit(1);
	}

	//Open files
	if ((file_A = fopen(argv[1], "r")) == NULL)
		exit(2);
	if ((file_b = fopen(argv[2], "r")) == NULL)
		exit(3);
	if ((file_x = fopen(argv[3], "r")) == NULL)
		exit(4);

	//Tolerance
	tolerance = atof(argv[4]);
	
	//Iterations
	iterations = atof(argv[5]);

	//Initialize A, b and x
	A = get_matrix(file_A);
	b = get_vector(file_b);
	//x = get_vector(file_x);
	x = vector_initialize(b->lines);
	
	//print_matrix(*A);
	//print_vector(*b);
	//print_vector(*x);

	//Executing de methode CG: return communication time
	execution = crono();
	norm_x = CG(A, b, x, tolerance, iterations);
	execution = crono() - execution;
	
	fprintf(stderr, "Norm_x: %f\nExecution: %f\n", norm_x, execution);
	return 0;
}

