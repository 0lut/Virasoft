#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H
#include <opencv2/core.hpp>
#include <vector>
// This class is implemented based on the code https://github.com/nearlyfreeapps/Rolling-Ball-Algorithm/blob/master/rolling_ball.py
class rollingBall
{
    int shrink_factor;
    int arc_trim_per;
    double small_ball_radius;
    double rsquare;
    double xtrim;
    int half_width;
    int width;
    std::vector <double> data;

public:
    rollingBall(double);
    cv::Mat rollingBallBg(cv::Mat, double radius,bool,bool);

};

#endif // ROLLINGBALL_H
