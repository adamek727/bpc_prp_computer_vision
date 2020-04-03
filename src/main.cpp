#include <iostream>
#include <chrono>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/mat.hpp>

#include <bpc_prp_opencv_lib/ImageProcessor.h>
#include <bpc_prp_opencv_lib/utils.h>


int main(int argc, char* argv[]) {

    bpc_prp_opencv_lib::printHelloWorld();

    cv::VideoCapture cap;

    if(!cap.open(0)) {
        std::cerr << "Error, unable to open camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    cap >> frame;

    if( frame.empty() ) {
        std::cerr << "Error, frame is empty" << std::endl;
        return -2;
    }

    /// example of bpc_prp_opencv_lib usage ///
    bpc_prp_opencv_lib::ImageProcessor imgProcessor(true, false, "/home/pi/images/");

    bool runLoop = true;
    while(runLoop) {

        cap >> frame;
        auto start = std::chrono::high_resolution_clock::now();
        auto detections = imgProcessor.analyzeImage(frame);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << "Duration [us]: " << duration.count() << std::endl;

        for(const auto& detection : detections) {
            std::cout << detection.getTypeAsString() << " " << detection.getDistance() << " " << detection.getDirection() << std::endl;
        }
    }

    return 0;
}

