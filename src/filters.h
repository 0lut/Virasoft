#ifndef FILTERS_H
#define FILTERS_H
#include <opencv2/opencv.hpp>


class Filters
{

    cv::Mat filterToBeApplied;
    int filterType;
    double filterRadius;
public:
    static const int MEAN=0,MEDIAN=1;
    Filters(int,double);
    cv::Mat dilate(cv::Mat &);
    cv::Mat meanFilter(cv::Mat &);
    Filters();
    ~Filters();
    cv::Mat ApplyFilter(cv::Mat & inImage);



};
#endif // FILTERS_H
