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
std::vector<cv::Point> Enhancer::getContour(bool erode, bool dilate){
    cv::Mat tmp;
    cv::Mat grayscale;
    cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);

    cv::resize(grayscale, grayscale, cv::Size(), 0.10, 0.10);

    //Inverse to get white on black image
    //cv::bitwise_not(grayscale, grayscale);

    //Blur to get better tresholding
    if(blur){
        cv::GaussianBlur(grayscale, grayscale, {7,7}, 0);
    }

    //cv::threshold(grayscale, img, 0.0, 255.0, cv::THRESH_BINARY + cv::THRESH_OTSU);
    cv::adaptiveThreshold(grayscale, tmp, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

    //cv::Mat kernel = cv::Mat::ones( 9, 9, 0 ); //Square
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9,9));//Circle

    if(erode){
        cv::erode(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }

    if(dilate){
        cv::dilate(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }

    if(erode){
        cv::erode(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }

    if(dilate){
        cv::dilate(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }

    if(erode){
        cv::erode(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }

    if(dilate){
        cv::dilate(tmp,tmp, kernel,cv::Point(-1,-1), 2);
    }


    cv::resize(tmp, tmp, cv::Size(), 10.0, 10.0);
    cv::bitwise_not(tmp, tmp);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(tmp, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
    if(contours.size() == 1){
        return contours[0];
    }else if(contours.size() > 1){
        int imax = -1;
        int max = -1;
        for(unsigned int i = 0; i < contours.size(); i ++){
            if(cv::contourArea(contours[i]) > max){
                imax = i;
                max = cv::contourArea(contours[i]);
            }
        }
        return contours[imax];
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
    std::vector<cv::Point> contour = getContour(true, true);
    //cv::drawContours(img, std::vector<std::vector<cv::Point>>(1, contour), -1, cv::Scalar(0,255,0),3);

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

    //Replace blank pixels with white pixels
    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if((xStart < i && xEnd > i) || (yStart < j && yEnd > j)){
                img.at<cv::Vec3b>(cv::Point(i,j)) = cv::Vec3b(255,255,255);
            }
        }
    }

    //Gaussian blur to smooth the separation between new white pixels and gray/white pixels
    int xRegionStart, xRegionEnd, yRegionStart, yRegionEnd;
    int regionSize = 40;
    int kernelSize = 39;

    if(translateX > 0){
        xRegionStart = xEnd - regionSize / 2;
    }else{
        xRegionStart = xStart - regionSize / 2;
    }
    if(translateY > 0){
        yRegionStart = yEnd - regionSize / 2;
    }else{
        yRegionStart = yStart - regionSize / 2;
    }

    xRegionEnd = xRegionStart + regionSize;
    yRegionEnd = yRegionStart + regionSize;

    if(xRegionStart < 0) xRegionStart = 0;
    if(xRegionEnd > img.size().width) xRegionEnd = img.size().width;
    if(yRegionStart < 0) yRegionStart = 0;
    if(yRegionEnd > img.size().height) yRegionEnd = img.size().height;

    cv::Range rangeRows = cv::Range::all();
    cv::Range rangeCols = cv::Range(yRegionStart, yRegionEnd);
    cv::medianBlur(img(rangeRows, rangeCols), img(rangeRows, rangeCols), kernelSize);
    cv::GaussianBlur(img(rangeRows, rangeCols), img(rangeRows, rangeCols), {kernelSize,kernelSize}, 0);

    rangeRows = cv::Range(xRegionStart, xRegionEnd);
    rangeCols = cv::Range::all();
    cv::medianBlur(img(rangeRows, rangeCols), img(rangeRows, rangeCols), kernelSize);
    cv::GaussianBlur(img(rangeRows, rangeCols), img(rangeRows, rangeCols), {kernelSize,kernelSize}, 0);

}

/**
 * @brief Enhancer::uniformizeBg
 * Uniformize background by removing everything outside contours.
 */
void Enhancer::uniformizeBg()
{
    qDebug() << "Background uniformization";
    std::vector<cv::Point> contour = getContour(true, true);
    if(!contour.empty()){
        //Background color
        cv::Scalar white = cv::Scalar(255,255,255);

        //Mask corresponding to the inside of contour
        cv::Mat maskInsideContour = cv::Mat::zeros(img.size(), CV_8UC1);
        cv::drawContours(maskInsideContour, std::vector<std::vector<cv::Point>>(1, contour), -1, cv::Scalar(255), cv::FILLED);

        //White image target
        cv::Mat maskedImage = cv::Mat(img.size(), CV_8UC3);
        maskedImage.setTo(white);

        //Copy pixels from original image inside contour to white image (masked image)
        img.copyTo(maskedImage, maskInsideContour);
        img = maskedImage;
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

        if(uniformBackground) uniformizeBg();
        if(centering) center();
        if(illuminationCorrection) correctIllumination();
        if(compression) compress();
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
