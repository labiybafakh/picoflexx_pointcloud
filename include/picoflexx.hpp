#ifndef PICOFLEXX
#define PICOFLEXX

#include <royale.hpp>
#include <GL/freeglut.h>


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
        royale::DepthData depthData_;
};

#endif