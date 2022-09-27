#include<stdio.h>
#include<stdlib.h>
#include"matrix.h"

Matrix* init_mat(int row, int col){
	/* mat allocation */
	Matrix *ptr=malloc(sizeof(Matrix));
	ptr->mat=malloc(row * sizeof(float*));
	ptr->mat[0]=malloc(row * col * sizeof(float));
	int length=row*col;
	/* mat value initialization*/
	for(int i=0; i< length; i++){
		ptr->mat[0][i]=0;
	}
	/* rows ptr initialization */
	for(int rows=1; rows < row; rows++){
		ptr->mat[rows]=&(ptr->mat[0][col*rows]);
	}
	/* other param initialization */
	ptr->col = col;
	ptr->row = row;
	return ptr;
}

Matrix* dup_mat(Matrix* mat){
	Matrix *ptr=malloc(sizeof(Matrix));
	ptr->mat=malloc(mat->row * sizeof(float*));
	ptr->mat[0]=malloc(mat->row * mat->col * sizeof(float));
	int length=mat->row*mat->col;
	/* mat value initialization*/
	for(int i=0; i< length; i++){
		ptr->mat[0][i]=mat->mat[0][i];
	}
	/* rows ptr initialization */
	for(int rows=1; rows < mat->row; rows++){
		ptr->mat[rows]=&(ptr->mat[0][mat->col*rows]);
	}
	/* other param initialization */
	ptr->col = mat->col;
	ptr->row = mat->row;

	return ptr;
}

Matrix* arr_mat(float* arr, int len, int row, int col){
	/* mat allocation */
	Matrix *ptr=malloc(sizeof(Matrix));
	ptr->mat=malloc(row * sizeof(float*));
	ptr->mat[0]=malloc(row * col * sizeof(float));
	int length=row*col;
	/* mat value initialization*/
	for(int i=0; i< length; i++){
		ptr->mat[0][i]=arr[i];
	}
	/* rows ptr initialization */
	for(int rows=1; rows < row; rows++){
		ptr->mat[rows]=&(ptr->mat[0][col*rows]);
	}
	/* other param initialization */
	ptr->col = col;
	ptr->row = row;
	return ptr;
}

Matrix* identity_mat(int row){
	/* mat allocation */
	Matrix *ptr=malloc(sizeof(Matrix));
	ptr->mat=malloc(row * sizeof(float*));
	ptr->mat[0]=malloc(row * row * sizeof(float));
	int length=row*row;
	/* mat value initialization*/
	for(int i=0; i< length; i++){
		ptr->mat[0][i]=0;
	}
	/* rows ptr initialization */
	for(int rows=1; rows < row; rows++){
		ptr->mat[rows]=&(ptr->mat[0][row*rows]);
	}
	/* diagnal */
	for(int i=0; i< row; i++){
		ptr->mat[i][i]=1;
	}
	/* other param initialization */
	ptr->col = row;
	ptr->row = row;
	return ptr;
}

void free_mat(Matrix *ptr){
	/**/
	free(ptr->mat[0]);
	free(ptr->mat);
	return ;
}

void print_mat(Matrix *mat){
	int length = mat->col * mat->row;
	char icon = '\n';
	printf("[\n");
	for(int i=0; i<length; i++){
		icon = ((i+1)%mat->col)?',':'\n';
		printf("%.2f%c", mat->mat[0][i], icon);
	}
	printf("]\n");
	return ;
}

/* private: calculate the det of an 2*2 matrix */
float detM(Matrix* mat){
	if(mat->col==mat->row && mat->col==2){
		return mat->mat[0][0]*mat->mat[1][1] - mat->mat[0][1]*mat->mat[1][0];
	}else{
		return 0;
	}
}

/* private: delete the e_row-th row and the e_col-th col */
/* shoulda be public I think */
Matrix* cofM(Matrix *mat, int e_row, int e_col){
	Matrix *a=init_mat(mat->row-1, mat->col-1);
	int is_c=0;
	int is_r=0;
	/* cofactor */
	for(int i=0; i<mat->row; i++){
		if(i==e_row){
			is_r=1;
			continue;
		}
		for(int j=0; j<mat->col; j++){
			if(j==e_col){
				is_c=1;
				continue;
			}
			a->mat[i-is_r][j-is_c] = mat->mat[i][j];
		}
		is_c=0;
	}

	return a;
}

/* (undone)private: calculate the cofactor of a certain element of a matrix */
/* matrices passed in this function will be free */
/* det */
float cof_ele(Matrix *mat){
	if(mat->col != mat->row){
		/* return if it's not a square matrix */
		return 0;
	}else if(mat->col>2){
		/* recursive call */
		float i=0;
		for(int j=0; j<mat->row; j++){
			if(mat->mat[0][j]){
				i+=((1-j%2*2)*mat->mat[j][0]*cof_ele(cofM(mat,j,0)));
			}
		}
		free_mat(mat);
		return i;
	}else{
		float i=detM(mat);
		free_mat(mat);
		return i;
	}
}

// float det_mat(Matrix *mat){
// }
/* (impure function)elementary row operation: row*multiplicant */
void e1_mat(Matrix *a, int row, float multiplicant){
	for(int i=0; i<a->col; i++){
		a->mat[row][i]*=multiplicant;
	}
	
	return;
}

/* (impure function)elementary row operation: row1+row2*multiplicant */
void e3_mat(Matrix *a, int row1, int row2, float multiplicant){
	for(int i=0; i<a->col; i++){
		a->mat[row1][i]+=(a->mat[row2][i]*multiplicant);
	}

	return;
}

/* public: return the inverse of mat */
Matrix* i_mat(Matrix *mat){
	/* Gaussian Elimination */
	Matrix* org=dup_mat(mat);
	Matrix* inv=identity_mat(mat->row);

	for(int i=0; i<org->col; i++){
		for(int j=0; j<org->row; j++){
			if(i==j){
				/* do e1 */
				e1_mat(inv, j, 1/org->mat[j][i]);
				e1_mat(org, j, 1/org->mat[j][i]);
			}else{
				/* do e3*/
				e3_mat(inv, j, i, -org->mat[j][i]/org->mat[i][i]);
				e3_mat(org, j, i, -org->mat[j][i]/org->mat[i][i]);
			}
		}
	}

	free_mat(org);

	return inv;
}

/* public: return the transpose of A */
Matrix* t_mat(Matrix *mat){
	Matrix* a=init_mat(mat->col, mat->row);
	for(int i=0; i<mat->col; i++){
		for(int j=0; j<mat->row; j++){
			a->mat[i][j] = mat->mat[j][i];
		}
	}
	return a;
}

/* public: matrix addition */
Matrix* a_mat(Matrix* a, Matrix* b){
	if(a->col==b->col && a->row==b->row){
		Matrix* c=init_mat(a->row, a->col);
		for(int i=0; i<a->col; i++){
			for(int j=0; j<a->row; j++){
				c->mat[j][i]=a->mat[j][i]+b->mat[j][i];
			}
		}

		return c;
	}
	return NULL;
}

/* public: matrix scalar multiplication */
Matrix* s_mat(Matrix *a, float b){
	int length=a->row*a->col;
	Matrix* c=dup_mat(a);
	for(int i=0; i<length; i++){
		c->mat[0][i]*=b;
	}

	return c;
}

/* public: matrix multiplication */
Matrix* m_mat(Matrix *a, Matrix *b){
	if(a->col != b->row){
		return NULL;
	}
	Matrix* c=init_mat(a->row, b->col);

	for(int i=0; i<b->col; i++){
		for(int j=0; j<a->row; j++){
			for(int k=0; k<a->col; k++){
				c->mat[j][i]+=a->mat[j][k]*b->mat[k][i];
			}
		}
	}

	return c;
}
