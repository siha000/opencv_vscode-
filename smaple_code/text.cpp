#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * 边缘处理
*/

int main(){
    // 读取图像
    Mat src = imread("camera_model.png");
    if (src.empty()){
        cout << "could not load image." << endl;
        return -1;
    }
    namedWindow("src", WINDOW_AUTOSIZE);
    imshow("src", src); 

    // 1. 高斯模糊
    Mat srcBlur;
    GaussianBlur(src, srcBlur, Size(3, 3), 0, 0);

    // 2. 转灰度
    Mat srcGray;
    cvtColor(srcBlur, srcGray, COLOR_BGR2GRAY);

    // 3. 求方向梯度
    Mat gradX, gradY;
    Sobel(srcGray, gradX, CV_16S, 1, 0, 3);
    Sobel(srcGray, gradY, CV_16S, 0, 1, 3);
    // Scharr(srcGray, gradX, CV_16S, 1, 0);
    // Scharr(srcGray, gradY, CV_16S, 0, 1);
    convertScaleAbs(gradX, gradX);  // calculates absolute values, and converts the result to 8-bit.
    convertScaleAbs(gradY, gradY);
    namedWindow("gradY", WINDOW_AUTOSIZE);
    imshow("gradX", gradX);
    namedWindow("gradY", WINDOW_AUTOSIZE);
    imshow("gradY", gradY);

    printf("type: %d, %d", gradX.type(), gradY.type());

    // 4. 图像混合
    Mat dst;
    addWeighted(gradX, 0.5, gradY, 0.5, 0, dst);
    namedWindow("dst", WINDOW_AUTOSIZE);
    imshow("dst", dst);

    // 4.1 
    Mat gradXY = Mat(gradX.size(), gradX.type());
    for (int row = 0; row < gradX.rows; row++){
        for (int col = 0; col < gradX.cols; col++){
            int gX = gradX.at<uchar>(row, col);
            int gY = gradY.at<uchar>(row, col);
            gradXY.at<uchar>(row, col) = saturate_cast<uchar>(gX + gY);
        }
    }
    namedWindow("gradXY", WINDOW_AUTOSIZE);
    imshow("gradXY", gradXY);
  
    waitKey(0);
    return 0;
}