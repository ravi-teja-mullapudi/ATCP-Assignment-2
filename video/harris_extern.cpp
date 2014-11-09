#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <cmath>
#include <string.h>


extern "C" void  harris_ref(int  C, int  R, float * img, void * harris_void)
{
  float * Ix;
  Ix = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Iy;
  Iy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Ixx;
  Ixx = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Ixy;
  Ixy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Iyy;
  Iyy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Sxx;
  Sxx = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Sxy;
  Sxy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  float * Syy;
  Syy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));

  float *harris = (float *)harris_void;

  for (int  i = 1; (i <= R); i = (i + 1))
  {
    for (int  j = 1; (j <= C); j = (j + 1))
    {
      // X derivative
      Ix[((i * (2 + C)) + j)] = (img[(((-1 + i) * (C + 2)) + (-1 + j))] * -0.0833333333333f) + 
                                (img[(((1 + i) * (C + 2)) + (-1 + j))] * 0.0833333333333f) + 
                                (img[(((-1 + i) * (C + 2)) + j)] * -0.166666666667f) + 
                                (img[(((1 + i) * (C + 2)) + j)] * 0.166666666667f) + 
                                (img[(((-1 + i) * (C + 2)) + (1 + j))] * -0.0833333333333f) + 
                                (img[(((1 + i) * (C + 2)) + (1 + j))] * 0.0833333333333f);
      // Y derivative
      Iy[((i * (2 + C)) + j)] = (img[(((-1 + i) * (C + 2)) + (-1 + j))] * -0.0833333333333f) + 
                                (img[(((-1 + i) * (C + 2)) + (1 + j))] * 0.0833333333333f) + 
                                (img[((i * (C + 2)) + (-1 + j))] * -0.166666666667f) + 
                                (img[((i * (C + 2)) + (1 + j))] * 0.166666666667f) + 
                                (img[(((1 + i) * (C + 2)) + (-1 + j))] * -0.0833333333333f) + 
                                (img[(((1 + i) * (C + 2)) + (1 + j))] * 0.0833333333333f);
    }
  }

  for (int  i = 1; (i <= R); i = (i + 1)) {
    for (int  j = 1; (j <= C); j = (j + 1)) {

      Ixx[((i * (2 + C)) + j)] = 
          Ix[((i * (2 + C)) + j)] * Ix[((i * (2 + C)) + j)];
      Iyy[((i * (2 + C)) + j)] = 
          Iy[((i * (2 + C)) + j)] * Iy[((i * (2 + C)) + j)];
      Ixy[((i * (2 + C)) + j)] = 
          Ix[((i * (2 + C)) + j)] * Iy[((i * (2 + C)) + j)];
    }
  }

  for (int  i = 2; (i < R); i = (i + 1)) {
    for (int  j = 2; (j < C); j = (j + 1)) {

      Syy[((i * (2 + C)) + j)] = Iyy[(((-1 + i) * (2 + C)) + (-1 + j))] + 
                                 Iyy[(((-1 + i) * (2 + C)) + j)] + 
                                 Iyy[(((-1 + i) * (2 + C)) + (1 + j))] + 
                                 Iyy[((i * (2 + C)) + (-1 + j))] + 
                                 Iyy[((i * (2 + C)) + j)] + 
                                 Iyy[((i * (2 + C)) + (1 + j))] + 
                                 Iyy[(((1 + i) * (2 + C)) + (-1 + j))] + 
                                 Iyy[(((1 + i) * (2 + C)) + j)] + 
                                 Iyy[(((1 + i) * (2 + C)) + (1 + j))];

      Sxy[((i * (2 + C)) + j)] = Ixy[(((-1 + i) * (2 + C)) + (-1 + j))] + 
                                 Ixy[(((-1 + i) * (2 + C)) + j)] +
                                 Ixy[(((-1 + i) * (2 + C)) + (1 + j))] + 
                                 Ixy[((i * (2 + C)) + (-1 + j))] + 
                                 Ixy[((i * (2 + C)) + j)] + 
                                 Ixy[((i * (2 + C)) + (1 + j))] + 
                                 Ixy[(((1 + i) * (2 + C)) + (-1 + j))] + 
                                 Ixy[(((1 + i) * (2 + C)) + j)] + 
                                 Ixy[(((1 + i) * (2 + C)) + (1 + j))];

      Sxx[((i * (2 + C)) + j)] = Ixx[(((-1 + i) * (2 + C)) + (-1 + j))] + 
                                 Ixx[(((-1 + i) * (2 + C)) + j)] + 
                                 Ixx[(((-1 + i) * (2 + C)) + (1 + j))] + 
                                 Ixx[((i * (2 + C)) + (-1 + j))] + 
                                 Ixx[((i * (2 + C)) + j)] + 
                                 Ixx[((i * (2 + C)) + (1 + j))] + 
                                 Ixx[(((1 + i) * (2 + C)) + (-1 + j))] + 
                                 Ixx[(((1 + i) * (2 + C)) + j)] + 
                                 Ixx[(((1 + i) * (2 + C)) + (1 + j))];
    }
  }

  for (int  i = 2; (i < R); i = (i + 1)) {
    for (int  j = 2; (j < C); j = (j + 1)) {

      float trace = 
          Sxx[((i * (2 + C)) + j)] + Syy[((i * (2 + C)) + j)];

      float det = 
          Sxx[((i * (2 + C)) + j)] * Syy[((i * (2 + C)) + j)] - 
          Sxy[((i * (2 + C)) + j)] * Sxy[((i * (2 + C)) + j)];

      harris[((i * (2 + C)) + j)] = det - (0.04f * trace * trace);
    }
  }
  
  free(Ix);
  free(Iy);
  free(Ixx);
  free(Ixy);
  free(Iyy);
  free(Sxx);
  free(Sxy);
  free(Syy);
}
