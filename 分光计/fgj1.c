//   3 local a = {321.50, 321.3, 38.51, 38.50, 321.5} -- 左游标读数
// 2 local b = {141.15, 141.2, 218.53, 218.52, 141.25} -- 右游标读数

#include <stdio.h>
#include <math.h>
double pi = 3.1415926535897932384626433832795028841971693993751058209;
double cot(double x)
{

  return 1.0 / tan(x);
}
double a[5] = {321.50, 321.3, 38.51, 38.50, 321.5};

double b[5] = {141.15, 141.2, 218.53, 218.52, 141.25};

int main()
{

  double C[5];
  double M = 0.0;
  for (int i = 0; i < 5; i++)
  {
    double c = a[i] * pi / 180;
    if (c > pi / 2)
    {
      c = 2 * pi - c;
    }

    double d = b[i] * pi / 180 - pi;

    d = d > 0 ? d : -d;

    C[i] = (c + d) / 2;
    M += C[i];

    printf("%f\n", C[i]);
  }

  M = M / 5.0;
  printf("%f\n", M);
  double g, f = 0.00;
  for (int i = 0; i < 5; i++)
  {

    g = C[i] - M;

    f += g * g;
  }

  double n = sqrt(f) / 2;

  printf("%f\n", n);

  double T = 2.0 / 30.0 * pi / 180.0;
  double e = sqrt(n * n + T * T);
  printf("%.10f\n", T);
  printf("%.10f\n %f\n %f\n  ", e, e / pi * 180.0, e / pi * 180.0 * 60);

  double a1 = (60.125 + 60.075) / 2.0 * pi / 180.0;

  double N = sin((a1 + M) / 2.0) / sin(a1 / 2.0);

  printf("%f\n", N);

  double c1 = cot((a1 + M) / 2.0);

  double b2 = 0.5 * sqrt((c1 - cot(a1 / 2)) * (c1 - cot(a1 / 2)) * (n * n + T * T) + c1 * c1 * 0.001 * 0.001);

  printf("%f\n", b2);

  return 0;
}
