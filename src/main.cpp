#include <GL/freeglut.h>
#include <cmath>
#include "picoflexx.hpp"
#include <Eigen/Dense>


float angle = 0.0f;
int numPoints = 38304;
Eigen::Matrix3Xf pointCloud(3, numPoints);

picoflexx picoflexx_;


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 3.0f,  // Eye position
              0.0f, 0.0f, 0.0f,  // Look at position
              0.0f, 1.0f, 0.0f); // Up vector

    // glRotatef(angle, 1.0f, 1.0f, 1.0f);
    // drawCube();
    picoflexx_.displayData();

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