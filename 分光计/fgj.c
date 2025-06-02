#include <stdio.h>
int main()
{
double a[5] ={38.675, 38.75, 38.52, 38.51, 38.625};

double b[5];

for (int i=0;i<5;i++)
{
    b[i] = (a[i]-(int)a[i]) * 60;
    printf("%f\n",b[i] );

}
return 0;

}
