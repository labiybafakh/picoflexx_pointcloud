#include <GL/freeglut.h>
#include <cmath>
#include "picoflexx.hpp"
#include <Eigen/Dense>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/filters/voxel_grid.h>



float angle = 0.0f;
int numPoints = 38304;
Eigen::Matrix3Xf pointCloud(3, numPoints);

picoflexx picoflexx_;


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 3.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    const auto& cloud = picoflexx_.getPointCloud();

    // Convert to PCL point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    for (const auto& pt : cloud) {
        pcl_cloud->points.emplace_back(pt.x, pt.y, pt.z);
    }
    pcl_cloud->width = pcl_cloud->points.size();
    pcl_cloud->height = 1;
    pcl_cloud->is_dense = false;

    // Downsample using VoxelGrid
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::VoxelGrid<pcl::PointXYZ> sor;
    sor.setInputCloud(pcl_cloud);
    sor.setLeafSize(0.01f, 0.01f, 0.01f); // Adjust voxel size as needed
    sor.filter(*cloud_filtered);

    glBegin(GL_POINTS);
    for (const auto& pt : cloud_filtered->points) {
        glVertex3f(pt.x, pt.y, pt.z);
    }
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(1, timer, 0); // 1000 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Picoflexx");


    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1, timer, 0);

    glutMainLoop();
    return 0;
}