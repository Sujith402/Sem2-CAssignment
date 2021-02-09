#include <stdio.h>

int bs(int *,int size,int (*)(int, int));
void swap(int *x,int *y);
int ascend(int x,int y);
int descend(int x,int y);

int main(){
    int a[]={1,2,3,4,76,54,32,0,-1,88,999,100,23};
    int size = sizeof(a)/sizeof(int);
    bs(a,size,ascend);
    for (int i=0;i<size;i++){
        printf("%d      ",a[i]);
    }
    printf("\n");
    bs(a,size,descend);
    for (int i=0;i<size;i++){
        printf("%d      ",a[i]);
    }
}

int ascend(int x,int y){
    return x>y;
}

int descend(int x,int y){
    return x<y;
}

void swap(int *x,int *y){
    int temp = *x;
    *x=*y;
    *y=temp;
}
int bs(int *p,int size,int (*comp)(int,int)){
    int i,j;
    for (i=0;i<size;i++){
        for (j=0;j<size-1;j++){
            if (comp(p[j],p[j+1])){
                swap(&p[j],&p[j+1]);
            }
        }
    }
}

//printf("%d",++a*b++ + b*a++);