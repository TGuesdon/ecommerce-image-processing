#include "enhancer.h"

Enhancer::Enhancer(bool compression, bool centering, bool uniformBackground, bool watermark, bool uniformLuminosity)
{

}

void Enhancer::compress()
{

}

void Enhancer::center()
{

}

void Enhancer::uniformizeBg()
{

}

void Enhancer::applyWatermark()
{

}

void Enhancer::uniformizeL()
{

}

void Enhancer::process(QString filepath)
{
    img = cv::imread(filepath.toStdString());
    if(img.data == NULL){
        qDebug() << "No image found! Check path.";
    }else{
        qDebug() << "Image found. Processing.";
    }
}
