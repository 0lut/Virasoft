#include "skeletonize.h"
int table1 []  =           {0,0,0,0,0,0,1,3,0,0,3,1,1,0,1,3,0,0,0,0,0,0,0,0,0,0,2,0,3,0,3,3,
                            0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,3,0,2,2,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            2,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,3,0,2,0,
                            0,0,3,1,0,0,1,3,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                            3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            2,3,1,3,0,0,1,3,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            2,3,0,1,0,0,0,1,0,0,0,0,0,0,0,0,3,3,0,1,0,0,0,0,2,2,0,0,2,0,0,0};

int table2 [] =             {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,2,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Skeletonize::Skeletonize()
{

}
int Skeletonize::thin(int pass, int table [],cv::Mat &inImage)
{



        if(inImage.type() != CV_8U)
            throw std::invalid_argument( "Input image has to be binary image.\nBinarize function is defined in generalpurposefunctions.cpp");

        int pixelsRemoved=0;
        int pixelVal,bgColor;
        bgColor=255;
        int sum = 0;
        std::vector <unsigned char> kernelPixels(9,0);
        for(int i = 1; i < inImage.rows - 1; i++)
        {
            // I think this is super - efficient
            unsigned char* Mis [] = { inImage.ptr<unsigned char>(i-1),
                                              inImage.ptr<unsigned char>(i),
                                              inImage.ptr<unsigned char>(i+1)};
            const unsigned char* Mi;
            const unsigned char* MiBo;
            for(int j = 1; j < inImage.cols - 1; j++)
            {

                for(int k = 0 ; k < 9 ; k++)
                {
                    int rowInx, colInx;
                    rowInx = (int) (k / 3);
                    colInx = (int) (j - 1 + k % 3);
                    kernelPixels[k]= Mis[rowInx][colInx];

                }
            int index;
            int code;
            pixelVal = kernelPixels[4];
            //bgColor is 255
            if(pixelVal != bgColor)
            {
                index = 0;
                if (kernelPixels[0]!=bgColor) index |= 1;
                if (kernelPixels[1]!=bgColor) index |= 2;
                if (kernelPixels[2]!=bgColor) index |= 4;
                if (kernelPixels[5]!=bgColor) index |= 8;
                if (kernelPixels[8]!=bgColor) index |= 16;
                if (kernelPixels[7]!=bgColor) index |= 32;
                if (kernelPixels[6]!=bgColor) index |= 64;
                if (kernelPixels[3]!=bgColor) index |= 128;
                code = table[index];
                if ((pass&1)==1) { //odd pass
                    if (code==2||code==3) {
                        pixelVal = bgColor;
                        pixelsRemoved++;
                    }
                } else { //even pass
                    if (code==1||code==3) {
                        pixelVal = bgColor;
                        pixelsRemoved++;
                    }
                }

            }

            Mis[1][j] =(unsigned char) pixelVal;





                //Get 3x3 slice of image
                //            cv::Mat_ <unsigned char>  roi =  inImage(cv::Range(i-1, i+2), cv::Range(j-1, j+2));
                //            std::cout << roi.cols << ' '<<roi.rows <<'\n';
                //sum += (int) Mis[1][j];


            }
        }

        //std::cout <<sum;







    return pixelsRemoved;

}
void Skeletonize::process(cv::Mat &in)
{
    int pixelsRemoved = 0;
    int pass = 0;
    do {
        std::cout << "x1";
        pixelsRemoved = thin(pass++, table1, in);
        pixelsRemoved += thin(pass++, table1, in);

    } while (pixelsRemoved>0);

    do {
        std::cout << "x2";
        pixelsRemoved = thin(pass++, table2, in);
        pixelsRemoved += thin(pass++, table2, in);

    } while (pixelsRemoved>0);
}
