#include <GL/freeglut.h>
#include <cmath>
#include "picoflexx.hpp"
#include <Eigen/Dense>


float angle = 0.0f;
int numPoints = 38304;
Eigen::Matrix3Xf pointCloud(3, numPoints);

picoflexx picoflexx_;

void drawCube() {
    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();

    // Right face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    // Left face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 3.0f,  // Eye position
              0.0f, 0.0f, 0.0f,  // Look at position
              0.0f, 1.0f, 0.0f); // Up vector

    glRotatef(angle, 1.0f, 1.0f, 1.0f);
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
    angle += 2.0f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 60 FPS
}

int main(int argc, char** argv) {
    // glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // glutInitWindowSize(800, 600);
    // glutCreateWindow("OpenGL FreeGLUT Rotating Cube");


    // glEnable(GL_DEPTH_TEST);

    // glutDisplayFunc(display);
    // glutReshapeFunc(reshape);
    // glutTimerFunc(0, timer, 0);

    // glutMainLoop();
    return 0;
}