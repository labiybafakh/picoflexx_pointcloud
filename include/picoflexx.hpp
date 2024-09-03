#ifndef PICOFLEXX
#define PICOFLEXX

#include <royale.hpp>
#include <GL/freeglut.h>
#include <atomic>
#include <string>
#include <chrono>
#include <vector>
#include <string.h>


class picoflexx: public royale::IDepthDataListener{
    public:
        void onNewData (const royale::DepthData *data) override;
        void displayData();
        picoflexx();
        ~picoflexx();
    private:
        bool setUpCamera();
        std::unique_ptr<royale::ICameraDevice> m_cameraDevice;
        int iterX, iterY, iterZ;
        int pointcloud_size_;
        const royale::DepthData *depth_data_;
        size_t desired_usecase_index;

};

#endif