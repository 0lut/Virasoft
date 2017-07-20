#include "rollingball.h"
#include <cmath>
#include <iostream>
rollingBall::rollingBall(double radius)
{
    if(radius <= 10.0)
    {
        shrink_factor = 1;
        arc_trim_per = 24;
    }

    else if(radius <= 30.0)
    {
        shrink_factor = 2;
        arc_trim_per = 24;

    }

    else if(radius <= 100.0 )
    {
        shrink_factor = 4;
        arc_trim_per = 32;
    }
    else
    {
        shrink_factor = 8;
        arc_trim_per = 40;
    }


    small_ball_radius=radius/shrink_factor;
    if(small_ball_radius < 1.0)
        small_ball_radius=1;

    rsquare=small_ball_radius*small_ball_radius;
    xtrim = int(arc_trim_per * small_ball_radius) / 100;
    half_width = int(round(small_ball_radius - xtrim));
    width = (2 * half_width) + 1;
    data.resize(width * width , 0.0);
    int p = 0;
    for( int y = 0 ; y < width ; y++)
        for( int x = 0 ; x < width ; x++)
         {
            int xval = x - half_width;
            int yval = y - half_width;
            double temp = rsquare - (xval * xval) - (yval * yval);

            if(temp > 0)
            {
                data[p] = sqrt(temp);
                p++;
            }

         }


    std::cout <<"/"<< data [2];
}

cv::Mat rollingBall::rollingBallBg(cv::Mat arr , double radius , bool light_background = true , bool smoothing = true)
{
    bool invert = false;
    if(light_background)
        invert = true;
    rollingBall ball(radius);
    cv::Mat float_arr(arr);
   // float_arr = rollingBallFloatBg(float_arr,radius,invert,smoothing,ball);
    cv::Mat backgroundPixels(float_arr.reshape(1,1));

    double offset;
    if(invert)
        offset = 255.5;
    else
        offset = 0.5;

    cv::Mat pixels(1,backgroundPixels.cols,CV_8S);
    pixels = arr.reshape(1,1);

   /* for(int p = 0 ; p < pixels.cols ; p++)
    {
        double value = (pixels.at(cv::Point(0,p)) & 0xff) - (backgroundPixels.at(cv::Point(0,p))) + offset;
        if(value < 0)
            value = 0;
        if(value > 255.0)
            value = 255;


    }*/










}
