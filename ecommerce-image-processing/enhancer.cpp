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
 * @brief Enhancer::getContour
 * @return 1 contour if it can be isolated, otherwise an empty vector
 */
std::vector<cv::Point> Enhancer::getContour(){
    cv::Mat tmp;
    cv::Mat grayscale;
    cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);

    cv::resize(grayscale, grayscale, cv::Size(), 0.10, 0.10);

    //Inverse to get white on black image
    cv::bitwise_not(grayscale, grayscale);

    //Blur to get better tresholding
    if(blur){
        cv::GaussianBlur(grayscale, grayscale, {7,7}, 0);
    }

    cv::threshold(grayscale, tmp, 0.0, 255.0, cv::THRESH_BINARY + cv::THRESH_OTSU);

    cv::Mat kernel = cv::Mat::ones( 9, 9, 0 );

    if(erode){
        cv::erode(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }

    if(dilate){
        cv::dilate(tmp,tmp, kernel,cv::Point(-1,-1), 10);
    }

    cv::resize(tmp, tmp, cv::Size(), 10.0, 10.0);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(tmp, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_KCOS);
    if(contours.size() == 1){
        return contours[0];
    }else{
        return std::vector<cv::Point>();
    }
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
    std::vector<cv::Point> contour = getContour();
    if(!contour.empty()){
       cv::Moments M = cv::moments(contour);
       int cX = int(M.m10 / M.m00);
       int cY = int(M.m01 / M.m00);

       int iX = img.size().width/2;
       int iY = img.size().height/2;

       int translateX = iX - cX;
       int translateY = iY - cY;

       cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, translateX, 0, 1, translateY);
       cv::warpAffine(img,img,trans_mat,img.size());

       fillBlank(translateX, translateY);
    }
}

void Enhancer::fillBlank(int translateX, int translateY){
    qDebug() << "translate x : " << translateX;
    qDebug() << "translate y : " << translateY;
    int xStart, xEnd;
    int yStart, yEnd;

    if(translateX > 0){
        //Translation to the right
        xStart = 0;
        xEnd = translateX;
    }else{
        //Translation to the left
        xStart = img.size().width - 1 + translateX;
        xEnd = img.size().width;
    }

    if(translateY > 0){
        //Translation to the bottom
        yStart = 0;
        yEnd = translateY;
    }else{
        //Translation to the top
        yStart = img.size().height - 1 + translateY;
        yEnd = img.size().height;
    }

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if((xStart < i && xEnd > i) || (yStart < j && yEnd > j)){
                img.at<cv::Vec3b>(cv::Point(i,j)) = cv::Vec3b(255,255,255);
            }
        }
    }
}

/**
 * @brief Enhancer::uniformizeBg
 * Uniformize background by removing everything outside contours.
 */
void Enhancer::uniformizeBg()
{
    qDebug() << "Background uniformization";
    std::vector<cv::Point> contour = getContour();
    if(!contour.empty()){
        succesfulBgUniformization++;
    }
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

cv::Mat Enhancer::process(QString filepath)
{
    img = cv::imread(filepath.toStdString());
    //cv::imshow("original", img);
    if(img.data == NULL){
        qDebug() << "No image found! Check path." << filepath;
    }else{
        qDebug() << "Image found. Processing.";
        qDebug() << filepath;
        totalProcessed ++;

        if(compression) compress();
        if(centering) center();
        if(illuminationCorrection) correctIllumination();
        if(uniformBackground) uniformizeBg();
        if(watermark) applyWatermark();
    }
    //cv::imshow("processed", img);

    return img;
}

int Enhancer::getTotal(){
    return totalProcessed;
}

int Enhancer::getSucessfulBgUniformization(){
       return succesfulBgUniformization;
}
