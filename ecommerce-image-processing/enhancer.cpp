#include "enhancer.h"

Enhancer::Enhancer(bool compression, bool centering, bool uniformBackground, bool watermark, bool illuminationCorrection)
{
    this->compression = compression;
    this->centering = centering;
    this->uniformBackground = uniformBackground;
    this->watermark = watermark;
    this->illuminationCorrection = illuminationCorrection;
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
 * @brief Enhancer::illuminationCorrection
 * Siwtch to lab space
 * Use adaptative histogram equalization
 */
void Enhancer::correctIllumination()
{
    cv::Mat lab;
    cv::cvtColor(img, lab, cv::COLOR_BGR2Lab);

    //Split lab into L,a and b channel
    std::vector<cv::Mat> lab_planes(3);
    cv::split(lab, lab_planes);

    //Apply clahe algorithm on L channel
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2.0);
    cv::Mat dst;
    clahe->apply(lab_planes[0], dst);

    //Merge the the color planes back into an Lab image
    dst.copyTo(lab_planes[0]);
    cv::merge(lab_planes, lab);

    //Convert back to rgb
    cv::cvtColor(lab, img, cv::COLOR_Lab2BGR);
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
        if(illuminationCorrection) correctIllumination();
    }
}
