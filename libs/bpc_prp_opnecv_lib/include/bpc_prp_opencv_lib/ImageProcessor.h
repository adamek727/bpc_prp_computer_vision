#pragma once

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/mat.hpp>

#include "Detection.h"

namespace bpc_prp_opencv_lib {

    class ImageProcessor {

    public:

        ImageProcessor(bool verbose, bool saveImages, std::string imagePath)
        : verbose_{verbose}
        , saveImages_{saveImages}
        , imagePath_{std::move(imagePath)} {

        }

        ~ImageProcessor() {

        }

        std::vector<Detection> analyzeImage(cv::Mat img) const;

    private:

        cv::Mat preprocessImage(cv::Mat) const;
        void splitChannels(const cv::Mat&, cv::Mat& r, cv::Mat& g, cv::Mat& b) const;

        std::vector<Detection> detectObstacles(const cv::Mat&) const;
        std::vector<Detection> maskToDetection(const cv::Mat, Detection::DetectionType type) const;

        float estimateDistance(int left, int top, int width, int height, int area) const;
        float estimateDirection(int left, int top, int width, int height, int area) const;

        void saveImage(std::string imageName, const cv::Mat&) const;

        bool verbose_;
        bool saveImages_;
        std::string imagePath_;
    };

}

