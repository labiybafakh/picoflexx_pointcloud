#include "pointcloud_utils.hpp"
#include <GL/freeglut.h>
#include <pcl/filters/voxel_grid.h>

pcl::PointCloud<pcl::PointXYZ> PointCloudUtils::convertToPCL(const std::vector<point3d>& cloud) const {
    pcl::PointCloud<pcl::PointXYZ> pcl_cloud;
    for (const auto& pt : cloud) {
        pcl_cloud.points.emplace_back(pt.x, pt.y, pt.z);
    }
    pcl_cloud.width = pcl_cloud.points.size();
    pcl_cloud.height = 1;
    pcl_cloud.is_dense = false;
    return pcl_cloud;
}

pcl::PointCloud<pcl::PointXYZ> PointCloudUtils::downsamplePCL(const pcl::PointCloud<pcl::PointXYZ>& input_cloud) const {
    pcl::PointCloud<pcl::PointXYZ> down_sampled;
    pcl::VoxelGrid<pcl::PointXYZ> sor;
    sor.setInputCloud(input_cloud.makeShared());
    sor.setLeafSize(0.01f, 0.01f, 0.01f);
    sor.filter(down_sampled);
    return down_sampled;
}

void PointCloudUtils::visualizePCL(const pcl::PointCloud<pcl::PointXYZ>& pcl_cloud) const {
    glBegin(GL_POINTS);
    for (const auto& pt : pcl_cloud.points) {
        glVertex3f(pt.x, pt.y, pt.z);
    }
    glEnd();
}