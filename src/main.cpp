#include <GL/freeglut.h>
#include <cmath>
#include "picoflexx.hpp"
#include <Eigen/Dense>
#include "pointcloud_utils.hpp"

float angle = 0.0f;
int numPoints = 38304;
Eigen::Matrix3Xf pointCloud(3, numPoints);

picoflexx picoflexx_;

PointCloudUtils pc_utils;

int win_downsampled, win_mesh;


void displayDownsampled() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    auto cloud = picoflexx_.getPointCloud();
    auto pcl_cloud = pc_utils.convertToPCL(cloud);
    auto downsampled_pcl_cloud = pc_utils.downsamplePCL(pcl_cloud, 0.012);

    auto clusters = pc_utils.regionGrowingSegmentation(downsampled_pcl_cloud);
    std::cout << "Clusters: " << clusters.size() << std::endl;
    int colorIdx = 0;
    for (const auto& cluster : clusters) {
        float r = static_cast<float>((colorIdx * 77) % 255) / 255.0f;
        float g = static_cast<float>((colorIdx * 151) % 255) / 255.0f;
        float b = static_cast<float>((colorIdx * 211) % 255) / 255.0f;
        glColor3f(r, g, b);
        glBegin(GL_POINTS);
        for (int idx : cluster.indices) {
            const auto& pt = downsampled_pcl_cloud.points[idx];
            glVertex3f(pt.x, pt.y, pt.z);
        }
        glEnd();
        colorIdx++;
    }

    glutSwapBuffers();
}

void displayMesh() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    auto cloud = picoflexx_.getPointCloud();
    auto pcl_cloud = pc_utils.convertToPCL(cloud);
    auto downsampled_pcl_cloud = pc_utils.downsamplePCL(pcl_cloud, 0.012);

    pcl::PolygonMesh mesh = pc_utils.greedyTriangulation(downsampled_pcl_cloud);
    pc_utils.visualizeMesh(mesh);

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
    glutSetWindow(win_downsampled);
    glutPostRedisplay();
    glutSetWindow(win_mesh);
    glutPostRedisplay();
    glutTimerFunc(60, timer, 0); // ~60 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // First window: Downsampled point cloud
    glutInitWindowSize(800, 600);
    win_downsampled = glutCreateWindow("Downsampled Point Cloud");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(displayDownsampled);
    glutReshapeFunc(reshape);

    // Second window: Mesh
    glutInitWindowSize(800, 600);
    win_mesh = glutCreateWindow("Mesh");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(displayMesh);
    glutReshapeFunc(reshape);

    // Start timer ONCE (it will update both windows)
    glutTimerFunc(60, timer, 0);

    glutMainLoop();
    return 0;
}