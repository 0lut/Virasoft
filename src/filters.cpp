#include "filters.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#define MAX_KERNEL_LENGTH 20
Filters::Filters(int inFilterType,double coef){

    filterType=inFilterType;
    filterRadius=coef;
}
Filters::Filters()
{
    filterType=Filters::MEAN;
}
Filters::~Filters()
{
    filterToBeApplied.release();

}
cv::Mat Filters::ApplyFilter(cv::Mat &inImage)
{
    cv::Mat outputImage=inImage.clone();
    if(filterType==Filters::MEAN)
    {
        for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 ) // Max Kernel ??
        {
             cv::medianBlur ( inImage, outputImage, i );
        }
    }
    if(filterType==Filters::MEDIAN)
    {
        for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        {
            cv::blur( inImage, outputImage, cv::Size( i, i ), cv::Point(-1,-1) );
        }
    }

   inImage=outputImage;
   return outputImage;

}
/**
 @param input image
 * ImageJ is using 3x3 box filter in default.
 * In inverted images, dilate() should be used instead erode. It will be implemented.
 */
cv::Mat Filters::dilate(cv::Mat & inImage)
{
    cv::Mat outImage(inImage);
    cv::Mat kernel = cv::Mat::ones(3,3,CV_8U);
    cv::erode(inImage,outImage,kernel);
    inImage=outImage;
    outImage.release();
    return inImage;
}
/**
 * @param Input Image
 * @param Size of kernel
 */
cv::Mat dilate(cv::Mat & inImage, int kernelSize)
{

    cv::Mat outImage(inImage);
    cv::Mat kernel = cv::Mat::ones(kernelSize,kernelSize,CV_8U);
    cv::erode(inImage,outImage,kernel);
    inImage=outImage;
    outImage.release();
    return inImage;

}


/*
 *
 * IJ is using 3x3 circular kernel for mean filtering
 * Currently it does not generate kernel but it can be easily applied by looking makeLineRadii method in ImageJ
 *
*/
cv::Mat Filters::meanFilter(cv::Mat & inImage)
{
    cv::Mat outImage(inImage);
    float kernelD [] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
    cv::Mat kernel(3,3,CV_32F,kernelD);
    kernel=kernel/2;
    cv::filter2D(inImage,outImage,inImage.depth(),kernel);
    inImage=outImage;
    outImage.release();
    return inImage;
}



