#include "backgroundsubtracter.h"
#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <cmath>
#include <vector>
typedef std::vector <double> _vd;
using namespace cv;
using namespace std;
colourDeconvolution::colourDeconvolution()
{

}

cv::Mat colourDeconvolution::process(cv::Mat I,string stainingType)
{
    //=======STEP 1=======
        // rgbI <-- RGB channels of (I) with 3 channels & 64-bit precision
        Mat rgbI;
        if (I.type() == CV_64FC3)
            I.convertTo(I, CV_32FC3);

        // Careful! Image matrix read in OpenCV is in BGR color.
        cvtColor(I, rgbI, CV_BGR2RGB);

        switch (rgbI.type())
        {
        case CV_8UC3:
            rgbI.convertTo(rgbI, CV_64FC3, 1.0 / 255);
            break;
        case CV_64FC3:
            break;
        case CV_32FC3:
            rgbI.convertTo(rgbI, CV_64FC3);
            break;
        default:
            // just for check in test
            cout << "Type of image doesn't belong to {8U,32F,64F}" << endl;
            break;
        }



        //=======STEP 2=======
        // odRGB <-- optical density of (I)
        Mat odRGB;
        odRGB = rgbI + 1 / 255.0;
        log(odRGB, odRGB);
        odRGB = -(255 * odRGB) / log(255);

        //normalise vector length

        Mat M = normalizeStaining("H DAB");
        // D <-- M^-1
        Mat D = M.inv();
        //======STEP 4=======
        // odHEB <-- optical density of HEB image
        //		 <-- D^T * odRGB
        Mat odHEB;
        D = D.t();
        transform(odRGB, odHEB, D);

        //======STEP 5=======
        // hebI <-- HEB channels of image (I)
        Mat hebI;
        hebI = -(odHEB - 255)*log(255) / 255.0;
        exp(hebI, hebI);
        //=======STEP 6=======
        // Before thresholding, convert (hebI) to 32-bit matrix
        // because cv::threshold can handle only 8-bit & 32-bit
        hebI.convertTo(hebI, CV_32FC3);
        threshold(hebI, hebI, 255, 255, THRESH_TRUNC);
        hebI = hebI + 0.5;
        Mat output;
        // here is no need to multiply 255
        hebI.convertTo(output, CV_8UC3);
        //======RETURN=======
    return output;
}

cv::Mat colourDeconvolution::normalizeStaining(std::string stainingType)
{
    _vd MODx(3,0.0);
    _vd MODy(3,0.0);
    _vd MODz(3,0.0);
    _vd cosx(3,0.0);
    _vd cosy(3,0.0);
    _vd cosz(3,0.0);
    _vd len(3,0.0);
    double leng;

    if (stainingType == "H&E")
    {
        // GL Haem matrix
        MODx[0]= 0.644211; //0.650;
        MODy[0]= 0.716556; //0.704;
        MODz[0]= 0.266844; //0.286;
        // GL Eos matrix
        MODx[1]= 0.092789; //0.072;
        MODy[1]= 0.954111; //0.990;
        MODz[1]= 0.283111; //0.105;
        // Zero matrix
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }

    if (stainingType == "H DAB")
    {
        // 3,3-diamino-benzidine tetrahydrochloride
        // Haem matrix
        MODx[0]= 0.650;//0.773;
        MODy[0]= 0.704;//0.548;
        MODz[0]= 0.286;//0.320;
        // DAB matrix
        MODx[1]= 0.268;//0.477;
        MODy[1]= 0.570;//0.670;
        MODz[1]= 0.776;//0.569;
        // Zero matrix
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }

    if (stainingType == "FastRed FastBlue DAB")
    {
        //fast red
        MODx[0]= 0.21393921;
        MODy[0]= 0.85112669;
        MODz[0]= 0.47794022;
        // fast blue
        MODx[1]= 0.74890292;
        MODy[1]= 0.60624161;
        MODz[1]= 0.26731082;
        // dab
        MODx[2]= 0.268;
        MODy[2]= 0.570;
        MODz[2]= 0.776;
    }


    if (stainingType == "Methyl Green DAB")
    {
        // MG matrix (GL)
        MODx[0]= 0.98003;
        MODy[0]= 0.144316;
        MODz[0]= 0.133146;
        // DAB matrix
        MODx[1]= 0.268;
        MODy[1]= 0.570;
        MODz[1]= 0.776;
        // Zero matrix
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }

    if (stainingType =="H&E DAB")
    {
        // Haem matrix
        MODx[0]= 0.650;
        MODy[0]= 0.704;
        MODz[0]= 0.286;
        // Eos matrix
        MODx[1]= 0.072;
        MODy[1]= 0.990;
        MODz[1]= 0.105;
        // DAB matrix
        MODx[2]= 0.268;
        MODy[2]= 0.570;
        MODz[2]= 0.776;
    }

    if (stainingType == "H AEC")
    {
        // 3-amino-9-ethylcarbazole
        // Haem matrix
        MODx[0]= 0.650;
        MODy[0]= 0.704;
        MODz[0]= 0.286;
        // AEC matrix
        MODx[1]= 0.2743;
        MODy[1]= 0.6796;
        MODz[1]= 0.6803;
        // Zero matrix
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }


    if (stainingType == "Azan-Mallory")
    {
        //Azocarmine and Aniline Blue (AZAN)
        // GL Blue matrix
        MODx[0]= .853033;
        MODy[0]= .508733;
        MODz[0]= .112656;
        // GL Red matrix
        MODx[1]= 0.070933;
        MODy[1]= 0.977311;
        MODz[1]= 0.198067;
        // Orange matrix (not set yet, currently zero)
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }


    if (stainingType == "Alcian blue & H")
    {
        // GL Alcian Blue matrix
        MODx[0]= 0.874622;
        MODy[0]= 0.457711;
        MODz[0]= 0.158256;
        // GL Haematox after PAS matrix
        MODx[1]= 0.552556;
        MODy[1]= 0.7544;
        MODz[1]= 0.353744;
        // Zero matrix
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }


    if (stainingType == "H PAS")
    {
        // GL Haem matrix
        MODx[0]= 0.644211; //0.650;
        MODy[0]= 0.716556; //0.704;
        MODz[0]= 0.266844; //0.286;
        // GL PAS matrix
        MODx[1]= 0.175411;
        MODy[1]= 0.972178;
        MODz[1]= 0.154589;
        // Zero matrix
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 0.0;
    }


    if (stainingType == "RGB")
    {
        //R
        MODx[0]= 0.0;
        MODy[0]= 1.0;
        MODz[0]= 1.0;
        //G
        MODx[1]= 1.0;
        MODy[1]= 0.0;
        MODz[1]= 1.0;
        //B
        MODx[2]= 1.0;
        MODy[2]= 1.0;
        MODz[2]= 0.0;
    }


    if (stainingType == "CMY")
    {
        //C
        MODx[0]= 1.0;
        MODy[0]= 0.0;
        MODz[0]= 0.0;
        //M
        MODx[1]= 0.0;
        MODy[1]= 1.0;
        MODz[1]= 0.0;
        //Y
        MODx[2]= 0.0;
        MODy[2]= 0.0;
        MODz[2]= 1.0;
    }

    for (int i=0; i<3; i++)
    {

        cosx[i]=cosy[i]=cosz[i]=0.0;
        len[i]=std::sqrt(MODx[i]*MODx[i] + MODy[i]*MODy[i] + MODz[i]*MODz[i]);
        if (len[i] != 0.0)
        {
            cosx[i]= MODx[i]/len[i];
            cosy[i]= MODy[i]/len[i];
            cosz[i]= MODz[i]/len[i];
        }
    }

    if (cosx[1]==0.0)
    {
        //2nd colour is unspecified
        if (cosy[1]==0.0)
        {
            if (cosz[1]==0.0)
            {
                cosx[1]=cosz[0];
                cosy[1]=cosx[0];
                cosz[1]=cosy[0];
            }
        }
    }



    if (cosx[2]==0.0){ // 3rd colour is unspecified
        if (cosy[2]==0.0)
        {
            if (cosz[2]==0.0)
            {
                if ((cosx[0]*cosx[0] + cosx[1]*cosx[1])> 1){

                    cosx[2]=0.0;
                }
                else {
                    cosx[2]=std::sqrt(1.0-(cosx[0]*cosx[0])-(cosx[1]*cosx[1]));
                }

                if ((cosy[0]*cosy[0] + cosy[1]*cosy[1])> 1){

                    cosy[2]=0.0;
                }
                else {
                    cosy[2]=std::sqrt(1.0-(cosy[0]*cosy[0])-(cosy[1]*cosy[1]));
                }

                if ((cosz[0]*cosz[0] + cosz[1]*cosz[1])> 1){

                    cosz[2]=0.0;
                }
                else {
                    cosz[2]=std::sqrt(1.0-(cosz[0]*cosz[0])-(cosz[1]*cosz[1]));
                }
            }
        }
    }

    leng=std::sqrt(cosx[2]*cosx[2] + cosy[2]*cosy[2] + cosz[2]*cosz[2]);

    cosx[2]= cosx[2]/leng;
    cosy[2]= cosy[2]/leng;
    cosz[2]= cosz[2]/leng;



    for (int i=0; i<3; i++)
    {
        if (cosx[i] == 0.0)
            cosx[i] = 0.001;
        if (cosy[i] == 0.0)
            cosy[i] = 0.001;
        if (cosz[i] == 0.0)
            cosz[i] = 0.001;


    }







    Mat M = (Mat_<float>(3, 3) <<
        cosx[0], cosy[0], cosz[0],
        cosx[1], cosy[1], cosz[1],
        cosx[2], cosy[2], cosz[2]);

    return M;



}
