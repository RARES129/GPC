#define _CRT_SECURE_NO_WARNINGS
# define M_PI 3.14159265358979323846
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "glut.h"
#include <vector>
#include<iostream>
using namespace std;
// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

void Init(void)
{

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void MouseFunc(int button, int state, int x, int y)
{
}

class Cartezian
{
public:
    int lines;
    int columns;
    double radius = 0.04;
    double xmin, xmax, ymin, ymax;


    Cartezian(int lines, int columns, double xmin, double ymin, double xmax, double ymax)
    {
        this->lines = lines;
        this->columns = columns;
        this->xmin = xmin;
        this->xmax = xmax;
        this->ymin = ymin;
        this->ymax = ymax;
        // dimensiune celule
        cellWidth = double(xmax - xmin) / double(columns);
        cellHeight = double(ymax - ymin) / double(lines);
    }

    void afiseazaGrila()
    {
        // linii
        for (double i = ymin; i <= ymax; i += cellHeight)
        {
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);
            glVertex2d(xmin, i);
            glVertex2d(xmax, i);
            glEnd();
        }

        // coloane
        for (double j = xmin; j <= xmax; j += cellWidth)
        {
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);
            glVertex2d(j, ymin);
            glVertex2d(j, ymax);
            glEnd();
        }
    }
    void drawCircle(double cx, double cy, double radius, int circlePoints = 100)
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        glColor3f(0, 0, 0);
        glBegin(GL_POLYGON);
        const double PI = 3.1415926;
        for (int i = 0; i < circlePoints; i++)
        {
            // current angle
            double theta = (2.0 * PI * i) / double(circlePoints);
            double x = radius * cos(theta);
            double y = radius * sin(theta);
            glVertex2d(x + cx, y + cy);
        }
        glEnd();
    }

    void drawBetween(int coord_x, int coord_y, int w) {
        for (auto i = -w / 2; i <= w / 2; i++) {
            drawCircle(xmin + coord_x * cellWidth, -1 + (coord_y + i) * cellHeight, this->radius);
        }

    }

    vector<pair<int, int>> vertex_calc(float x0, float y0, float xn, float yn, int index) {
        vector<pair<int, int>> vertexes;
        float dx = fabs(xn - x0);
        float dy = fabs(yn - y0);
        int x = x0, y = y0;
        if (x0 - x > 0.5) {
            vertexes.push_back({ x + 1, y });
        }
        else {
            vertexes.push_back({ x, y });
        }
        int d = 2.0f * dy - dx;
        int dE = 2.0f * dy;
        int dNE = 2.0f * (dy - dx);
        while (x < xn)
        {
            if (d <= 0)
            {
                d += dE;
                x++;
            }
            else
            {
                d += dNE;
                y = y + index;
                x++;
            }
            vertexes.push_back({ x, y });
        }
        return vertexes;
    }

    vector<pair<int, int>> AfisareSegmentDreapta3(float x0, float y0, float xn, float yn)
    {
        vector<pair<int, int>> vertexes;
        int index = 0;
        if (x0 < xn && y0 < yn) {
            index = 1;
            vertexes = vertex_calc(x0, y0, xn, yn, index);
        }
        else {
            if (x0 < xn && y0 > yn) {
                index = -1;
                vertexes = vertex_calc(x0, y0, xn, yn, index);
            }
        }

        return vertexes;
    }

    void drawSegment(float x0, float y0, float xn, float yn, int width) {
        for (auto& points : AfisareSegmentDreapta3(x0, y0, xn, yn)) {
            drawBetween(points.first, points.second, width);
        }
        glColor3f(1, 0.1, 0.1);
        glBegin(GL_LINES);
        glVertex2d(xmin + x0 * cellWidth, ymin + y0 * cellHeight);
        glVertex2d(xmin + xn * cellWidth, ymin + yn * cellHeight);
        glEnd();
    }
private:
    double cellWidth;
    double cellHeight;
};

// aici vom desena grila si liniile
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Cartezian gc = Cartezian(15, 15, -1.0, -1.0, 1.0, 1.0);
    gc.afiseazaGrila();

    gc.drawSegment(0, 0, 15, 7, 1);
    gc.drawSegment(0, 15, 15, 10, 3);
    glFlush();
}
void KeyboardFunc(unsigned char key, int x, int y)
{
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}