#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

void harris_ref(int C, int R, float * input, float *& harris);

using namespace cv;

#ifndef NRUNS
#define NRUNS 1
#endif

#ifndef INFO
#define INFO(x)
#else
#define INFO(x) x
#endif

#ifdef ANALYZE
    double t_start, t_end;
    #define TIMER__ t_start = getTickCount();
    #define __TIMER(section) t_end = getTickCount(); \
                    std::cout <<section <<" time = " \
                    <<1000 * ((t_end - t_start)/getTickFrequency()) <<" ms" \
                    <<std::endl;
#else
    #define TIMER__
    #define __TIMER(section)
#endif

void image_info(Mat &img)
{
    std::cout << "Channels: " << img.channels() << std::endl;
    std::cout << "Depth: "  << img.type() << std::endl;     
    std::cout << "Size: "  << img.size() << std::endl;     
}

int main(int argc, char** argv)
{    
    std::cout.precision(6);      
    // Counters for measuring execution time 
    double t_start, t_end, t_diff;
    Mat img, imgGray, imgGrayf;

    const char* window_image  = "Image";
    const char* window_opencv_result  = "OpenCV Result";
    const char* window_ref_result  = "Reference Result";

    if(argc == 2){
        img = imread(argv[1]);
    }
    else{
        printf("Usage: %s image\n", argv[0]);
        exit(1);
    }

    cv::cvtColor(img, imgGray, CV_BGR2GRAY);

    imgGray.convertTo(imgGrayf, CV_32F, 1.0/255);
    INFO(image_info(imgGrayf);)

    int M = img.size().width;
    int N = img.size().height;

    // Check if the region of interest is within image boundaries
    Rect roi(0, 0, M, N);
    Mat img_region = imgGrayf(roi).clone();
    Mat img_region_harris = imgGrayf(roi).clone();

    Mat img_show = img(roi).clone();
    Mat img_show_harris = img(roi).clone();
    Mat img_show_pipe = img(roi).clone();

    // OpenCV implementation using optimized harris corner detection
    // implementation
    Mat opencv_harris;
    Mat gradXKern = (Mat_<float>(3,3) <<  -1.0f, 0, 1.0f, 
                                          -2.0f, 0,  2.0f, 
                                          -1.0f, 0, 1.0f);
    gradXKern = 1.0f/12 * gradXKern;
    Mat gradYKern = (Mat_<float>(3,3) <<  -1.0f,  -2.0f, -1.0f, 
                                               0,       0,     0, 
                                           1.0f,   2.0f, 1.0f);
    gradYKern = 1.0f/12 * gradYKern;
    Mat gaussKern = (Mat_<float>(3,3) <<  0.0/16, 2.0/16, 1.0/16,
                                          2.0/16, 4.0/16, 2.0/16,
                                          1.0/16, 2.0/16, 1.0/16);
    Mat sumKern = (Mat_<float>(3,3) <<    1.0f, 1.0f, 1.0f,
                                          1.0f, 1.0f, 1.0f,
                                          1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 3; i++) {
	TIMER__	
        cornerHarris(img_region, opencv_harris, 3, 3, 0.04);
	__TIMER("OpenCV")
    }
    
    /* Reference Implementation */
    float *img_ref, *harris = NULL;
        
    img_ref = (float*) malloc((N+2)*(M+2) * sizeof(float));

    /* Adding a boundary */
    for (int i = 0; i < N+2; i++)
        for (int j = 0; j < M+2; j++) {
            int ii = i - 1 > 0 ?  (i -1 < N ? i-1:  N-1): 0;
            int jj = j - 1 > 0 ?  (j -1 < M ? j-1:  M-1): 0;
            img_ref[(i)*(M+2) + (j)] = img_region.at<float>(ii, jj);
    }

    for (int i = 0; i < NRUNS; i++) {
        if (harris != NULL)
            free(harris);
	TIMER__	
        harris_ref(M, N, img_ref, harris);      
	__TIMER("Reference")
    }
   
    Mat result_ref(N-2, M-2, img_region.type());
    for (int i = 0; i < N-2; i++) {
        for (int j = 0; j < M-2; j++){
            result_ref.at<float>(i, j) = harris[(i+2)*(M+2) + (j+2)];
        }
    }  

	#ifdef SHOW
    // Create windows
    namedWindow( window_image, WINDOW_NORMAL );
    namedWindow( window_opencv_result, WINDOW_NORMAL );
    namedWindow( window_ref_result, WINDOW_NORMAL );
    for(;;) {
        int c;
        c = waitKey(10);
        if( (char)c == 27 )
        { break; }
        imshow( window_image, img_show);
        // The scaling factor is to display the coarsity map
        imshow( window_opencv_result, opencv_harris * 10000);
        imshow( window_ref_result, result_ref * 10000);
    }
	#endif

    free(img_ref);
    free(harris);
    return 0;
}

/* Your Implementation */
void  harris_ref(int  C, int  R, float * img, float *& harris)
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

  harris = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));

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
