#ifndef ENHANCER_H
#define ENHANCER_H

#include "opencv2/opencv.hpp"

#include <QString>
#include <QtDebug>



class Enhancer
{
public:
    Enhancer(bool compression, bool centering, bool uniformBackground, bool watermark, bool illuminationCorrection);

    cv::Mat process(QString filepath);

    int getSucessfulBgUniformization();
    int getTotal();

private:

    void compress();
    void center();
    void uniformizeBg();
    void applyWatermark();
    void correctIllumination();

    std::vector<cv::Point> getContour(bool erode, bool dilate);
    void fillBlank(int translateX, int translateY);

    bool compression = false;
    bool centering = false;
    bool uniformBackground = false;
    bool watermark = false;
    bool illuminationCorrection = false;

    bool blur = true;

    int totalProcessed = 0;
    int succesfulBgUniformization = 0;

    cv::Mat img;
};

#endif // ENHANCER_H
