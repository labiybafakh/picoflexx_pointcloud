#pragma once
#include <vector>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include "picoflexx.hpp"

class PointCloudUtils {
public:
    PointCloudUtils() = default;

    pcl::PointCloud<pcl::PointXYZ> convertToPCL(const std::vector<point3d>& cloud) const;
    pcl::PointCloud<pcl::PointXYZ> downsamplePCL(const pcl::PointCloud<pcl::PointXYZ>& input_cloud) const;
    void visualizePCL(const pcl::PointCloud<pcl::PointXYZ>& pcl_cloud) const;
};