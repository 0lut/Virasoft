#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "src/filters.cpp"
#include "src/loselesssaver.cpp"
#include "src/backgroundsubtracter.cpp"
#include "src/rollingball.cpp"

//using namespace  cv;
int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }


    //Row Column

    cv::Mat img=cv::imread(argv[1],1);
    Filters filter(3,4);
    colourDeconvolution CD;
    img = filter.dilate(img);
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    imwrite("/home/natrium/sahinCD.tiff", img, compression_params);
    cv::namedWindow("asd",cv::WINDOW_AUTOSIZE);
    cv::imshow("asd",img);
    cv::waitKey(0);
    std::cout <<"Finished succesfully";



    return 0;
}
