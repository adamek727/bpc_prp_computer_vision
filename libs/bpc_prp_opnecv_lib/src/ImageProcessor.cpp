#include "bpc_prp_opencv_lib/ImageProcessor.h"

#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

namespace bpc_prp_opencv_lib {

    std::vector<Detection> ImageProcessor::analyzeImage(cv::Mat img) const {

        cv::Mat preprocessed = preprocessImage(img);
        std::vector<Detection> output = detectObstacles(preprocessed);
        return output;
    }


    cv::Mat ImageProcessor::preprocessImage(cv::Mat img) const {

        if(verbose_) {
            std::cout << "Preprocessing image" << std::endl;
        }

        cv::Mat blured;
        cv::blur(img, blured, cv::Size{5,5});
        saveImage("blured", blured);

        cv::Mat downsampled;
        cv::resize(blured, downsampled, cv::Size{0,0}, 0.5, 0.5);
        saveImage("downsampled", downsampled);

        return downsampled;
    }


    void ImageProcessor::splitChannels(const cv::Mat& img, cv::Mat& r, cv::Mat& g, cv::Mat& b) const {

        cv::Mat bgr[3];
        cv::split(img,bgr);

        r = bgr[2];
        g = bgr[1];
        b = bgr[0];
    }


    std::vector<Detection> ImageProcessor::detectObstacles(const cv::Mat& inputImg) const {

        if(verbose_) {
            std::cout << "Looking for workers and zombies" << std::endl;
        }

        std::vector<Detection> output;

        cv::Mat r,g,b;
        splitChannels(inputImg, r,g,b);
        saveImage("r", r);
        saveImage("g", g);
        saveImage("b", b);

        cv::Mat gray;
        cv::cvtColor(inputImg, gray, cv::COLOR_BGR2GRAY);
        saveImage("gray", gray);

        cv::Mat rt,gt,bt, grayt;
        cv::threshold(r, rt, 150, 255, CV_8U);
        cv::threshold(g, gt, 100, 255, CV_8U);
        cv::threshold(b, bt, 100, 255, CV_8U);
        cv::threshold(gray, grayt, 100, 255, CV_8U);

        saveImage("rt", rt);
        saveImage("gt", gt);
        saveImage("bt", bt);
        saveImage("grayt", grayt);

        cv::Mat workerMask = rt & ( (255-grayt));
        cv::Mat zombieMask = gt & ( (255-grayt));

        saveImage("workerMask", workerMask);
        saveImage("zombieMask", zombieMask);

        auto workers = maskToDetection(workerMask, Detection::DetectionType::kWorker);
        auto zombies = maskToDetection(zombieMask, Detection::DetectionType::kZombie);

        for(const auto& worker : workers) { output.push_back(worker); }
        for(const auto& zombie : zombies) { output.push_back(zombie); }

        return output;
    }


    std::vector<Detection> ImageProcessor::maskToDetection(const cv::Mat obstaclesMask, Detection::DetectionType type) const {

        std::vector<Detection> output;

        cv::Mat obstaclesMaskOpened;
        cv::Mat kernel = getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(7, 7));
        cv::morphologyEx(obstaclesMask, obstaclesMaskOpened, cv::MORPH_OPEN, kernel);
        saveImage("mask", obstaclesMaskOpened);

        cv::Mat stats, centroids, labelImage;
        int nLabels = connectedComponentsWithStats(obstaclesMaskOpened, labelImage, stats, centroids, 8, CV_16U);

        for(int i = 1 ; i < nLabels ; i++) {
            int left = stats.at<int>(i, cv::CC_STAT_LEFT);
            int top = stats.at<int>(i, cv::CC_STAT_TOP);
            int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
            int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);
            int area = stats.at<int>(i, cv::CC_STAT_AREA);

            if(verbose_) {
                switch(type) {
                    case Detection::DetectionType::kWorker: std::cout << "Worker "; break;
                    case Detection::DetectionType::kZombie: std::cout << "Zombie "; break;
                }
                std::cout << i << "] " << left << " " << top << " " << width << " " << height << " " << area << std::endl;
            }
            output.emplace_back( Detection(type,
                                           estimateDistance(left, top, width, height, area),
                                           estimateDirection(left, top, width, height, area)));
        }

        return output;
    }


    float ImageProcessor::estimateDistance(int left, int top, int width, int height, int area) const {
        // TODO: create code to calculate obstacle distance
        return 1.0f;
    }


    float ImageProcessor::estimateDirection(int left, int top, int width, int height, int area) const {
        // TODO: create code to calculate obstacle direction
        return 0.0f;
    }


    void ImageProcessor::saveImage(std::string imageName, const cv::Mat& img) const {
        if(saveImages_) {
            cv::imwrite( imagePath_ + imageName + ".jpg", img);
        }
    }
}

