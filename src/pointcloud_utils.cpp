#include "pointcloud_utils.hpp"
#include <GL/freeglut.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/region_growing.h>
#include <pcl/surface/gp3.h>
#include <pcl/point_types.h>
#include <pcl/PolygonMesh.h>
#include <pcl/conversions.h>


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

pcl::PointCloud<pcl::PointXYZ> PointCloudUtils::downsamplePCL(const pcl::PointCloud<pcl::PointXYZ>& input_cloud, float downsampled_size) const {
    pcl::PointCloud<pcl::PointXYZ> down_sampled;
    pcl::VoxelGrid<pcl::PointXYZ> sor;
    sor.setInputCloud(input_cloud.makeShared());
    sor.setLeafSize(downsampled_size, downsampled_size, downsampled_size);
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

pcl::ModelCoefficients::Ptr PointCloudUtils::detectCylinder(pcl::PointCloud<pcl::PointXYZ>& cloud) const {
    // Estimate normals
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
    ne.setSearchMethod(tree);
    ne.setInputCloud(cloud.makeShared());
    ne.setKSearch(50);
    ne.compute(*cloud_normals);

    // Cylinder segmentation
    pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal> seg;
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);

    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_CYLINDER);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setNormalDistanceWeight(0.1);
    seg.setMaxIterations(100);
    seg.setDistanceThreshold(0.01);
    seg.setRadiusLimits(0.05, 0.12); // 10cm radius
    seg.setInputCloud(cloud.makeShared());
    seg.setInputNormals(cloud_normals);

    seg.segment(*inliers, *coefficients);

    if (inliers->indices.empty()) {
        std::cerr << "No cylinder found." << std::endl;
    }
    else{
        std::cout << "Cylinder found" << std::endl;
    }
    return coefficients;
}

pcl::PolygonMesh PointCloudUtils::greedyTriangulation(const pcl::PointCloud<pcl::PointXYZ>& input_cloud) const {
    // Estimate normals
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
    ne.setInputCloud(input_cloud.makeShared());
    ne.setSearchMethod(tree);
    ne.setKSearch(20);
    ne.compute(*normals);

    // Concatenate XYZ and normal fields
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields(input_cloud, *normals, *cloud_with_normals);

    // Create search tree
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud(cloud_with_normals);

    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
    pcl::PolygonMesh triangles;

    // Set the parameters
    gp3.setSearchRadius(0.025); // Adjust as needed
    gp3.setMu(2.5);
    gp3.setMaximumNearestNeighbors(100);
    gp3.setMaximumSurfaceAngle(M_PI / 4); // 45 degrees
    gp3.setMinimumAngle(M_PI / 18); // 10 degrees
    gp3.setMaximumAngle(2 * M_PI / 3); // 120 degrees
    gp3.setNormalConsistency(false);

    // Get result
    gp3.setInputCloud(cloud_with_normals);
    gp3.setSearchMethod(tree2);
    gp3.reconstruct(triangles);

    return triangles;
}

// Visualize mesh triangles as wireframe
void PointCloudUtils::visualizeMesh(const pcl::PolygonMesh& mesh) const {
    // Convert mesh cloud to PointXYZ
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromPCLPointCloud2(mesh.cloud, cloud);

    glColor3f(0.2f, 0.8f, 0.2f); // Green mesh
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe

    glBegin(GL_TRIANGLES);
    for (const auto& poly : mesh.polygons) {
        if (poly.vertices.size() == 3) {
            for (int idx : poly.vertices) {
                const auto& pt = cloud.points[idx];
                glVertex3f(pt.x, pt.y, pt.z);
            }
        }
    }
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restore fill mode
}

std::vector<pcl::PointIndices> PointCloudUtils::regionGrowingSegmentation(const pcl::PointCloud<pcl::PointXYZ>& input_cloud) const {
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    ne.setInputCloud(input_cloud.makeShared());
    ne.setSearchMethod(tree);
    ne.setKSearch(20);
    ne.compute(*normals);

    // Region growing segmentation
    pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
    reg.setMinClusterSize(50);
    reg.setMaxClusterSize(1000000);
    reg.setSearchMethod(tree);
    reg.setNumberOfNeighbours(30);
    reg.setInputCloud(input_cloud.makeShared());
    reg.setInputNormals(normals);
    reg.setSmoothnessThreshold(3.0 / 180.0 * M_PI); // 3 degrees
    reg.setCurvatureThreshold(1.0);

    std::vector<pcl::PointIndices> clusters;
    reg.extract(clusters);

    return clusters;
}