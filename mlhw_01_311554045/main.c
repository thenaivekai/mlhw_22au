#include"list.h"
#include"matrix.h"
#include<stdio.h>
#include<stdlib.h>

List** file_list(){
    FILE* fp=fopen("testfile.txt","r");
    List** arr_list=malloc(2*sizeof(List*));
    arr_list[0]=init_list();/* x_list */
    arr_list[1]=init_list();/* y_list */
    float a,b;
    /* deal with num list */
    while(fscanf(fp,"%f,%f", &a, &b)!=EOF){
        list_push(arr_list[0],a);
        list_push(arr_list[1],b);
    }
    fclose(fp);
    return arr_list;
}

float power(float value, int bases){
    float j=1;
    for(int i=0; i<bases; i++){
        j*=value;
    }
    return j;
}

List* model(List* ptr, int bases){
    List* new_list=malloc(sizeof(List));
    new_list->list=malloc((bases+1)*ptr->length*sizeof(float));
    for(int i=0; i<ptr->length; i++){
        for(int j=0; j<bases; j++){
            new_list->list[bases*i+j]=power(ptr->list[i],bases-j-1);
        }
    }
    
    new_list->capacity=bases*ptr->length;
    new_list->length=bases*ptr->length;

    return new_list;
}

float error(List* t, Matrix* model, Matrix* coeff){
    Matrix* y=m_mat(model, coeff);
    Matrix* ny=s_mat(y, -1);
    Matrix* t_m=arr_mat(t->list, t->length, t->length, 1);
    Matrix* ev=a_mat(t_m, ny);
    Matrix* evt=t_mat(ev);
    Matrix* e=m_mat(evt, ev);
    float error = e->mat[0][0];
    free_mat(y);
    free_mat(ny);
    free_mat(t_m);
    free_mat(ev);
    free_mat(evt);
    free_mat(e);
    return error;
}

Matrix* LSE(List* x, List* t, int bases, int lambda){
    List* x_mod=model(x, bases);
    Matrix* d=arr_mat(x_mod->list, x_mod->length, x_mod->length/bases, bases);
    Matrix* t_m=arr_mat(t->list, t->length, t->length, 1);
    Matrix* d_tr=t_mat(d);
    Matrix* e=m_mat(d_tr,d);
    /* LSE special */
    Matrix* i=identity_mat(e->row);
    Matrix* ni=s_mat(i, lambda);
    Matrix* ad=a_mat(ni, e);
    /* LSE special */
    Matrix* f=i_mat(ad);
    Matrix* g=m_mat(f, d_tr);
    Matrix* h=m_mat(g, t_m);
    free_mat(d);
    free_mat(d_tr);
    free_mat(e);
    free_mat(f);
    free_mat(g);
    free_mat(t_m);
    free_mat(i);
    free_mat(ni);
    free_mat(ad);

    return h;
}

Matrix* Newton(List* x, List* t, int bases){
    List* x_mod=model(x, bases);
    Matrix* d=arr_mat(x_mod->list, x_mod->length, x_mod->length/bases, bases);
    Matrix* t_m=arr_mat(t->list, t->length, t->length, 1);
    Matrix* d_tr=t_mat(d);
    Matrix* e=m_mat(d_tr,d);
    Matrix* f=i_mat(e);
    Matrix* g=m_mat(f, d_tr);
    Matrix* h=m_mat(g, t_m);
    free_mat(d);
    free_mat(d_tr);
    free_mat(e);
    free_mat(f);
    free_mat(g);
    free_mat(t_m);

    return h;
}

void draw(FILE* plot, List* x, List* y, Matrix* coeff){
    /* regression function */
    for(int i=0; i<coeff->row; i++){
        if(i==0){
            fprintf(plot, "f%d<-function(x){%f}\n", i, coeff->mat[coeff->row-i-1][0], i);
        }else{
            fprintf(plot, "f%d<-function(x){%f*x^%d+f%d(x)}\n", i, coeff->mat[coeff->row-i-1][0], i, i-1);
        }
    }
    fprintf(plot, "plot(f%d,-6,6)\n", coeff->row-1);

    /* data point */
    for(int i=0; i<x->length; i++){
        fprintf(plot, "points(x=%f, y=%f)\n", x->list[i], y->list[i]);
    }

    return ;
}

void reg_report(Matrix* coeff, float error){
    printf("Fitting Line:");
    for(int i=coeff->row-1; 0<=i; i--){
        if(i){
            printf("%fX^%d + ", coeff->mat[coeff->row-i-1][0], i);
        }else{
            printf("%f", coeff->mat[coeff->row-i-1][0]);
        }
    }
    printf("\n");
    printf("Total error: %f\n\n", error);
    return ;
}

int main(void){
    int bases = 3;
    int lambda = 10000;

    /* read file */
    List** arr=file_list();
    printf("bases?\n");
    scanf("%d", &bases);
    printf("lambda?\n");
    scanf("%d", &lambda);


    List* x_mod=model(arr[0], bases);
    Matrix* d=arr_mat(x_mod->list, x_mod->length, x_mod->length/bases, bases);

    /* LSE method */
    Matrix* lse=LSE(arr[0], arr[1], bases, lambda);/*todo: shoulda move the coeff matrix gen function out of lse/nwt method? */

    /* Newton method */
    Matrix* nwt=Newton(arr[0], arr[1], bases);

    /* drawing module(R) */
    FILE* plot=popen("R --no-save -q","w");
    draw(plot, arr[0], arr[1], lse);
    draw(plot, arr[0], arr[1], nwt);
    pclose(plot);
    printf("LSE:\n");
    reg_report(lse, error(arr[1], d, lse));
    printf("Newton's Method:\n");
    reg_report(nwt, error(arr[1], d, nwt));


    /* free dynamic memory */
    free_list(arr[0]);
    free_list(arr[1]);
    free(arr);
    free(x_mod);
    free(d);
    free(lse);
    free(nwt);

    return 0;
}