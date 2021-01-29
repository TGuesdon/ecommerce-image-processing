#ifndef ENHANCER_H
#define ENHANCER_H

#include "opencv2/opencv.hpp"

#include <QString>
#include <QtDebug>



class Enhancer
{
public:
    Enhancer(bool compression, bool centering, bool uniformBackground, bool watermark, bool uniformLuminosity);

    void process(QString filepath);

private:

    void compress();
    void center();
    void uniformizeBg();
    void applyWatermark();
    void uniformizeL();

    bool compression = false;
    bool centering = false;
    bool uniformBackground = false;
    bool watermark = false;
    bool uniformLuminosity = false;

    cv::Mat img;
};

#endif // ENHANCER_H
