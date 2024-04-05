#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

#define dim 300
#define NRITER_M 20
#define MODMAX_M 2
#define RX_M 0.005
#define RY_M 0.005


unsigned char prevKey;
int nivel = 0;

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
        for (int i = 0; i < nriter; ++i) {
            z = z.pow2() + c;
            if (z.getModul() > modmax)
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
                    glColor3f(0.0f, 0.0f, 0.0f);
                }
                else {
                    float normalizedIter = (float)iter / (float)nriter;
                    float colorR, colorG, colorB;

                    if (normalizedIter < 0.5f) {
                        colorR = 2.0f * normalizedIter; 
                        colorG = 0.0f; 
                    }
                    else {
                        colorR = 1.0f; 
                        colorG = 6.0f * (normalizedIter - 0.2f); 
                        if (colorG > 1.0f) {
                            colorG = 1.0f; 
                        }
                    }
                    colorB = 6.0f - normalizedIter; 

                    glColor3f(colorR, colorG, colorB);
                }
                glVertex2d(x, y);
            }
        }
        glEnd();
    }



private:
    int nriter;
    double modmax;
};

class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};

class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}

    CPunct& operator=(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }
    SDate getM() const { return m; }

    // Setter for m
    void setM(double x, double y) {
        m.x = x;
        m.y = y;
    }

    void getxy(double& x, double& y)
    {
        SDate m_val = getM();
        x = m_val.x;
        y = m_val.y;
    }

    int operator==(CPunct& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector& operator=(CVector& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    // it is used for normalizing a vector
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};

class CLinieFractal {
public:
    void line(double lungime, int nivel, CPunct& p, CVector& v, double unghi) {
        if (nivel == 0) {
            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);
            return;
        }

        lungime = lungime / 2.0;
        line(lungime, nivel - 1, p, v, -unghi);
        v.rotatie(unghi);
        line(lungime, nivel - 1, p, v, unghi);
        v.rotatie(unghi);
        line(lungime, nivel - 1, p, v, -unghi);
    }

    void afisare(double lungime, int nivel) {
        CVector v(0.0, 1.0);
        CPunct p(-0.7, -0.7);

        if (nivel % 2 == 0)
            line(lungime, nivel, p, v, 60);
        else
            line(lungime, nivel, p, v, -60);
    }
};

class CSquareFractal {
public:
    void squareFractal(double lungime, int nivel, CPunct& p, double spacing) {
        if (nivel == 0) return;

        double x, y;
        p.getxy(x, y);

        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_LOOP);
        glVertex2d(x - lungime / 2, y - lungime / 2);
        glVertex2d(x + lungime / 2, y - lungime / 2);
        glVertex2d(x + lungime / 2, y + lungime / 2);
        glVertex2d(x - lungime / 2, y + lungime / 2);
        glEnd();

        double newLungime = lungime / 3.0; 
        CPunct newCenter;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;

                double newX = x + i * (lungime / 3.0 + spacing);
                double newY = y + j * (lungime / 3.0 + spacing);

                glColor3f(1.0, 0.1, 0.1); 
                glBegin(GL_LINE_LOOP);
                glVertex2d(newX - newLungime / 2, newY - newLungime / 2);
                glVertex2d(newX + newLungime / 2, newY - newLungime / 2);
                glVertex2d(newX + newLungime / 2, newY + newLungime / 2);
                glVertex2d(newX - newLungime / 2, newY + newLungime / 2);
                glEnd();

                newCenter = CPunct(newX, newY);
                squareFractal(newLungime, nivel - 1, newCenter, 0.1);
            }
        }
    }

    void afisare(double lungime, int nivel, double spacing) {
        CPunct p(0.0, 0.0); 
        squareFractal(lungime, nivel, p, spacing);
    }
};

class CTreeFractal {
public:
    void tree(double lungime, int nivel, CPunct& p, CVector& v) {
        if (nivel == 0)
            return;

        const float raport = 0.53;
        lungime *= raport;

        v.rotatie(-45);
        v.deseneaza(p, lungime);
        CPunct p1 = v.getDest(p, lungime);
        tree(lungime, nivel - 1, p1, v);

        v.rotatie(90);
        v.deseneaza(p, lungime);

        p = v.getDest(p, lungime);
        //lungime *= raport;
        v.rotatie(25);
        v.deseneaza(p, lungime);
        p1 = v.getDest(p, lungime);
        tree(lungime, nivel - 1, p1, v);
        tree(lungime, nivel - 1, p1, v);

        v.rotatie(-70);
        v.deseneaza(p, lungime);
        p1 = v.getDest(p, lungime);
        tree(lungime, nivel - 1, p1, v);

        p = v.getDest(p, lungime);
        lungime *= raport;
        v.rotatie(-90);
        v.deseneaza(p, lungime);
        p1 = v.getDest(p, lungime);
        tree(lungime, nivel - 1, p1, v);

        v.rotatie(135);
        p1 = v.getDest(p, lungime);
        v.deseneaza(p, lungime);
        tree(lungime, nivel - 1, p1, v);

        v.rotatie(-45);
    }

    void afisare(double lungime, int nivel) {
        CVector v(0.0, -1.0);
        CPunct p(0, 0.5);

        v.deseneaza(p, lungime / 4);
        p = v.getDest(p, lungime / 4);

        tree(lungime, nivel, p, v);
    }
};

void Display1() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(-2.0, 1.0, -1.5, 1.5, -1.0, 1.0);

    MandelbrotSet mandelbrot;
    mandelbrot.display(-2.0, -1.5, 1.0, 1.5);
    glFlush();
}

void Display2() {
    if (nivel > 6) nivel = 0;
    CSquareFractal csf;
    
    csf.afisare(0.5, nivel, 0.35);
    glColor3f(1.0f, 0.0f, 0.0f); 

    float minX = -0.8f, minY = -0.8f; 
    float maxX = 0.8f, maxY = 0.8f;   

    glBegin(GL_LINE_LOOP);
    glVertex2f(minX, minY);
    glVertex2f(maxX, minY);
    glVertex2f(maxX, maxY);
    glVertex2f(minX, maxY);
    glEnd();

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

    nivel++;
    glFlush();
    
}

void Display3() {
    if (nivel > 6) nivel = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    CTreeFractal ctf;
    ctf.afisare(1, nivel); 

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'f');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');

    nivel++;
    glFlush();
}

void Display4() {
    if (nivel > 6) nivel = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();


    glColor3f(1.0, 0.0, 0.0); 

    CLinieFractal clf;
    clf.afisare(1.0, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'f');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');

    nivel++; 
    glFlush(); 
}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    switch (prevKey)
    {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        nivel = 0;
        fprintf(stderr, "nivel = %d\n", nivel);
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display1();
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        Display2();
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        Display3();
        break;
    case '4':
        glClear(GL_COLOR_BUFFER_BIT);
		Display4();
		break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    prevKey = key;
    if (key == '1') {
        Display1();
    }
    else if (key == 27) {
        exit(0);
    }
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
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


