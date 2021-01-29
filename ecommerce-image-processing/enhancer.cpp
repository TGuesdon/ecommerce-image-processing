#include "enhancer.h"

Enhancer::Enhancer(bool compression, bool centering, bool uniformBackground, bool watermark, bool uniformLuminosity)
{

}

/**
 * @brief Enhancer::compress
 * Compress image ?
 */
void Enhancer::compress()
{

}

/**
 * @brief Enhancer::center
 * Center object using contours
 */
void Enhancer::center()
{

}

/**
 * @brief Enhancer::uniformizeBg
 * Uniformize background by removing everything outside contours.
 */
void Enhancer::uniformizeBg()
{

}

/**
 * @brief Enhancer::applyWatermark
 * Apply watermark
 */
void Enhancer::applyWatermark()
{

}

/**
 * @brief Enhancer::uniformizeL
 * Uniformize light.
 * Switch to lad -> ? -> rgb
 */
void Enhancer::uniformizeL()
{

}

void Enhancer::process(QString filepath)
{
    img = cv::imread(filepath.toStdString());
    if(img.data == NULL){
        qDebug() << "No image found! Check path." << filepath;
    }else{
        qDebug() << "Image found. Processing.";
    }
}
