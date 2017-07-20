#include "filters.h"
#include <iostream>
#define MAX_KERNEL_LENGTH 31 // Why? What is kernel size in ImageJ as default?
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
/*
 * IJ is using 4x4 circular kernel
 * Results are same with IJ (almost)
 * Dilate and erode are inverse operations, in inverted images, erode should be used instead
 */
cv::Mat Filters::dilate(cv::Mat & inImage)
{
    //return inImage;
    cv::Mat outImage(inImage);
    unsigned char kernelD [] = {0, 1, 1, 0,
                                1, 1, 1, 1,
                                1, 1, 1, 1,
                                0, 1, 1, 0};

    /*uchar kernelD [] = {0, 1, 0,
                        1, 0, 1,
                        0, 1, 0};*/
    cv::Mat kernel(4,4,CV_8U,kernelD);
    cv::erode(inImage,outImage,kernel);
    return outImage;
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

    return outImage;
}
