#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix{
	int col;
	int row;
	float **mat;
	int val;
}Matrix;

Matrix* init_mat(int row, int col);
Matrix* dup_mat(Matrix* mat);
Matrix* arr_mat(float* arr, int len, int row, int col);
Matrix* identity_mat(int row);
void free_mat(Matrix *ptr);
void print_mat(Matrix *mat);

Matrix* i_mat(Matrix *mat);/* inverse matrix */
Matrix* t_mat(Matrix *mat);/* transpose matrix */
Matrix* a_mat(Matrix* a, Matrix* b);/* matrix addition */
Matrix* s_mat(Matrix *a, float b);/* matrix scalar multiplication */
Matrix* m_mat(Matrix *a, Matrix *b);/* matrix multiplication */
/* void e1_mat(Matrix *a, int row, float multiplicant) */
/* void e3_mat(Matrix *a, int row1, int row2, float multiplicant) */
/* inverse matrix done by LU decompostion or Gaussian Elimination */

#endif