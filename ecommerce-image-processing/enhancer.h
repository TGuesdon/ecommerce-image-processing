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

private:

    void compress();
    void center();
    void uniformizeBg();
    void applyWatermark();
    void correctIllumination();

    std::vector<cv::Point> getContour();

    bool compression = false;
    bool centering = false;
    bool uniformBackground = false;
    bool watermark = false;
    bool illuminationCorrection = false;

    bool blur = true;
    bool erode = true;
    bool dilate = true;

    cv::Mat img;
};

#endif // ENHANCER_H
