#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "glut.h"

// the size of the window measured in pixels
#define dim 300
// the maximum number of iterations for the Mandelbrot set
#define NRITER_M 1000 // Increased for more detail
// the maximum value of M for the Mandelbrot set
#define MODMAX_M 4 // Adjusted to correct escape radius
// increments used in Mandelbrot set calculations
#define RX_M 0.005 // Increased resolution
#define RY_M 0.005 // Increased resolution

unsigned char prevKey;

class CComplex {
public:
    CComplex() : re(0.0), im(0.0) {}
    CComplex(double re1, double im1) : re(re1), im(im1) {}
    CComplex(const CComplex& c) : re(c.re), im(c.im) {}
    ~CComplex() {}

    CComplex& operator=(const CComplex& c)
    {
        re = c.re;
        im = c.im;
        return *this;
    }

    double getRe() const { return re; }
    void setRe(double re1) { re = re1; }

    double getIm() const { return im; }
    void setIm(double im1) { im = im1; }

    double getModul() const { return sqrt(re * re + im * im); }

    int operator==(const CComplex& c1) const
    {
        return (re == c1.re) && (im == c1.im);
    }

    CComplex operator+(const CComplex& c) const
    {
        return CComplex(re + c.re, im + c.im);
    }

    CComplex pow2() const
    {
        return CComplex(re * re - im * im, 2 * re * im);
    }

private:
    double re, im;
};

class MandelbrotSet {
public:
    MandelbrotSet() : nriter(NRITER_M), modmax(MODMAX_M) {}

    int isIn(const CComplex& c) const
    {
        CComplex z(0, 0);
        for (int i = 0; i < 15; ++i) {
            z = z.pow2() + c;
            if (z.getModul() > 2)  // Check if modulus exceeds 2
                return i;
        }
        return nriter;
    }

    void display(double xmin, double ymin, double xmax, double ymax) const
    {
        glBegin(GL_POINTS);
        for (double x = xmin; x <= xmax; x += RX_M) {
            for (double y = ymin; y <= ymax; y += RY_M) {
                CComplex c(x, y);
                int iter = isIn(c);
                if (iter == nriter) {
                    glColor3f(1.0f, 0.0f, 0.0f); // Inside: Red
                    glVertex2d(x + 0.52, y);
                }
                else {
                    // Map iteration count to a color
                    float hue = (float)iter / (float)nriter;
                    glColor3f(1.0f - hue, 1.0f - hue, 1.0f - hue); // Gradient from red to white
                    glVertex2d(x + 0.52, y);
                }
            }
        }
        glEnd();
    }



private:
    int nriter;
    double modmax;
};

void DisplayMandelbrotSet() {
    MandelbrotSet mandelbrot;
    mandelbrot.display(-2, -1.5, 1, 1.5); // Adjust display window for Mandelbrot set
}

void Init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(1);
    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayMandelbrotSet();
    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {
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
