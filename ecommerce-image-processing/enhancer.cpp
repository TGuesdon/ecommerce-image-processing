#include "enhancer.h"

Enhancer::Enhancer(bool compression, bool centering, bool uniformBackground, bool watermark, bool uniformLuminosity)
{
    this->compression = compression;
    this->centering = centering;
    this->uniformBackground = uniformBackground;
    this->watermark = watermark;
    this->uniformLuminosity = uniformLuminosity;
}

/**
 * @brief Enhancer::compress
 * Compress image ?
 */
void Enhancer::compress()
{
    qDebug() << "Compression";
}

/**
 * @brief Enhancer::center
 * Center object using contours
 */
void Enhancer::center()
{
    qDebug() << "Centering";
}

/**
 * @brief Enhancer::uniformizeBg
 * Uniformize background by removing everything outside contours.
 */
void Enhancer::uniformizeBg()
{
    qDebug() << "Background uniformization";
}

/**
 * @brief Enhancer::applyWatermark
 * Apply watermark
 */
void Enhancer::applyWatermark()
{
    qDebug() << "Apply watermark";
}

/**
 * @brief Enhancer::uniformizeL
 * Uniformize light.
 * Switch to lad -> ? -> rgb
 */
void Enhancer::uniformizeL()
{
    qDebug() << "Light uniformization";
}

void Enhancer::process(QString filepath)
{
    img = cv::imread(filepath.toStdString());
    if(img.data == NULL){
        qDebug() << "No image found! Check path." << filepath;
    }else{
        qDebug() << "Image found. Processing.";
        if(compression) compress();
        if(centering) center();
        if(uniformBackground) uniformizeBg();
        if(watermark) applyWatermark();
        if(uniformLuminosity) uniformizeL();
    }
}
