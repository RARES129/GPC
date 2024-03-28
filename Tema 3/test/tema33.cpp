#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"


#define dim 300

unsigned char prevKey;
int level = 0;

// Utility function to draw a square
void drawSquare(double x, double y, double size) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y - size);
    glVertex2f(x, y - size);
    glEnd();
}

// Function to recursively draw the Sierpinski Carpet
void sierpinskiCarpet(int level, double x, double y, double size) {
    if (level == 0) {
        drawSquare(x, y, size);
    }
    else {
        double newSize = size / 3.0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == 1 && j == 1) continue; // Skip the center square
                sierpinskiCarpet(level - 1, x + i * newSize, y - j * newSize, newSize);
            }
        }
    }
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    sierpinskiCarpet(level, -1.0, 1.0, 2.0);
    glFlush();
}

// Keyboard callback function
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        level = (level + 1) % 7; // Increase level and wrap around after 6
        break;
    case 'e':
        level = (level - 1 + 7) % 7; // Decrease level and wrap around
        break;
    case 27: // Escape key
        exit(0);
    }
    glutPostRedisplay();
}

// Initialization function
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(dim, dim);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Sierpinski Carpet");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardFunc);
    glutMainLoop();

    return 0;
}
