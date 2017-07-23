#ifndef SKELETONIZE_H
#define SKELETONIZE_H
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

class Skeletonize
{


public:
    Skeletonize();
    int thin(int,int [],cv::Mat &);
    void process(cv::Mat &);
};

#endif // SKELETONIZE_H
