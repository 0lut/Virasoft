#ifndef BACKGROUNDSUBTRACTER_H
#define BACKGROUNDSUBTRACTER_H
#include <opencv2/core.hpp>
#include <cstring>

class colourDeconvolution
{

    cv::Mat normalizeStaining(std::string stainingType);

public:
    colourDeconvolution();
    cv::Mat process(cv::Mat I, std::string stainingType);

};

#endif // BACKGROUNDSUBTRACTER_H
