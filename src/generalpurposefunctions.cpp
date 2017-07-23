#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
//TODO Inverted images will be checked.
cv::Mat binarize(cv::Mat inImage)
{
    //Grayscale matrix
    cv::Mat grayscaleMat(inImage.size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor(inImage, grayscaleMat, CV_BGR2GRAY);

    //Binary image
    cv::Mat binaryMat(grayscaleMat.size(), grayscaleMat.type());

    //Apply thresholding
    cv::threshold(grayscaleMat, binaryMat, 95, 255, cv::THRESH_BINARY);

    return binaryMat;

}
