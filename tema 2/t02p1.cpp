/**********************************
  
  The reader of this program should not limit
  himself/herself to the comments of this 
  program.

  If one wants to read more about the syntax and 
  the semantics of OpenGL functions used in this
  program, one should read the beginning of the
  paragraph 2.6 "Begin/End Paradigm", the 
  paragraph 2.6.1 "Begin and End Objects" from the 
  file glspec15.pdf at page 25/333 and the index
  from the end of that file. One could also
  read the references to the GLUT functions from
  the file glut-3.spec.pdf.
  


  H O W  T O  R E A D  T H I S  P R O G R A M ?
  
  Start from the function "main" and follow the 
  instruction flow, paying attention to the fact that
  this program belongs to the event-driven programming
  paradigm. So pay attention to what happens when 
  the user presses a key, moves the mouse or presses a
  mouse button. There are also some special events: the
  redrawing of the application window, etc.
  Identify what happens in the program when one of these
  events occurs.

  **********************************/


/**********************************
  With respect to the include-file directives, see the 
  example program from homework 1; in the following it
  was made the assumption that you are using GLUT locally,
  in your project and you didn't install it as an
  Administrator
  **********************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"

// the size of the window measured in pixels
#define dim 300

unsigned char prevKey;

// the graph of the Conchoid of Nicomedes
void Display1() {
   double xmax, ymax, xmin, ymin;
   double a = 1, b = 2;
   double pi = 4 * atan(1.0);
   double ratia = 0.05;
   double t;

   /**********************************
      Maximum/minimum values of x and y are computed.
      These values will be further used in scaling the
      original graph of the curve. 
      These values are stored in the variables
      xmax, ymax, xmin, ymin: e.g., xmax is initialized
      with a value smaller than any of possible values
      of x; if in doubt or you cannot estimate it, use 
      DBL_MIN instead (or DBL_MAX for minimum values).
      These DBL_* constants are found in <float.h> which
      need to be included.
      E.g., xmax is initialized with a - b - 1 because
      x(t) = a +/- b * cos(t) and for t in (-pi/2, pi/2),
      cos(t) is in (0, 1), so b * cos(t) is in (0, b),
      so +/- b * cos(t) is in (-b, b), so x(t) is in
      (a-b, a+b) and one can safely choose a-b-1 because
      a-b-1 < a-b. 
      For y(t) we see that in its expression appears
      tan(t) which varies in (-inf,+inf) when t
      varies in (-pi/2, pi/2).
     **********************************/
   xmax = a - b - 1; 
   xmin = a + b + 1;
   ymax = ymin = 0;
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = a + b * cos(t);
      xmax = (xmax < x1) ? x1 : xmax;
      xmin = (xmin > x1) ? x1 : xmin;

      x2 = a - b * cos(t);
      xmax = (xmax < x2) ? x2 : xmax;
      xmin = (xmin > x2) ? x2 : xmin;

      y1 = a * tan(t) + b * sin(t);
      ymax = (ymax < y1) ? y1 : ymax;
      ymin = (ymin > y1) ? y1 : ymin;

      y2 = a * tan(t) - b * sin(t);
      ymax = (ymax < y2) ? y2 : ymax;
      ymin = (ymin > y2) ? y2 : ymin;
   }

   /**********************************
      At this line, we have found that the graph of the Conchoid
      is included in the rectangle having the edges x = xmin,
      x = xmax, y = ymin and y = ymax. 
      We would like that the rectangle should be symmetric with
      respect to the Ox and Oy axes.
      We store in xmax and ymax the maximum of absolute values
      max(|xmax|,|xmin|) and max(|ymax|,|ymin|).
      Now we know that the graph of the Conchoid is included in
      the rectangle [-xmax, xmax] x [-ymax, ymax].
     **********************************/

   xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
   ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

   /**********************************
      At this line, we can perform the scaling. All the points
      scaled are visible (i.e., in the rectangle [-1,1]x[-1,1]).
      Because we have -xmax <= x <= xmax we get
      -1 <= x / xmax <= 1. Idem for y.
      In order to correctly display the graph of the Conchoid,
      one should use the exact same points that were used in
      the computation of the scaling factor.
     **********************************/

   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();

   glBegin(GL_LINE_STRIP); 
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x2,y2);
   }
   glEnd();
}

// The graph of the function 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
   double pi = 4 * atan(1.0);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;


   /**********************************
      For this example, the computa3tion of the scaling factors
      is not needed. Because x is in the interval [0, 8pi]
      the scaling factor for x is 8pi because x/(8pi) is in
      [0,1]. In the case of the exponential function we know
      that if x is [0,8pi] then sin x is in [-1,1] so
      e^(-sin x) is in [1/e, e] and thus it is safe to use 
      e^(1.1) as a scaling factor.
     **********************************/
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (double x = 0; x < xmax; x += ratia) {
      double x1, y1;
      x1 = x / xmax;
      y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();
}

double distanceToNearestInteger(double x) {
    double fractionalPart = x - floor(x);
    return fmin(fractionalPart, 1.0 - fractionalPart);
}

void Display3() {
    double xmax = 30.0;
    double ymax = 1.0; 
    

    glColor3f(1,0.1,0.1);

    glBegin(GL_LINE_STRIP);

    glVertex2f(0.0, 1.0);
    for (double x = 0.05; x <= xmax; x += 0.05) {
        double y = distanceToNearestInteger(x) / x;
        glVertex2f(x / xmax, y / ymax);
    }
    glEnd();
}
//The trisectrix of Longchamps
void Display4() {
    const auto ratio = 0.005;
    const auto a = 0.2;
    double ys[250], xs[250];
    double pi = 4 * atan(1.0);
    auto t = -pi / 2 + ratio;
    auto n = 0;

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    while (true) {

        if (t >= -pi / 6) break;

        const auto output_x = a / (4 * cos(t) * cos(t) - 3);
        const auto output_y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);

        xs[n] = output_x;
        ys[n] = output_y;
        n++;

        t += ratio;

        glVertex2f(output_x, output_y);
    }
    glEnd();

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_TRIANGLES);
    for (auto i = 1; i < n - 1; i += 3) {

        if (i > n / 4 && i < 3 * n / 4) continue;

        glVertex2f(-1.0, 1.0);
        glVertex2f(xs[i], ys[i]);
        glVertex2f(xs[i + 1], ys[i + 1]);
    }
    glEnd();
}

//The cycloid
void Display5() {
    double a = 0.1, b = 0.2;
    double xmax, ymax;
    double ratia = 0.05;
    double t;
    xmax = 1.2;
    ymax = 1;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -10; t < 10; t += ratia) {
        double x1, y1;
        x1 = (a * t - b * sin(t)) / xmax;
        y1 = (a - b * cos(t)) / ymax;
        glVertex2f(x1, y1);
    }
    glEnd();
}

//lemniscata lui bernouli
void Display6() {
    double a = 0.4;
    double pi = 4 * atan(1.0);
    double ratia = 0.01;
    double t;
    double x1, y1, x2, y2;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 4 + ratia; t < pi / 4; t += ratia) {
        double r, x, y;
        r = a * sqrt(2 * cos(2 * t));
        x = r * cos(t);
        y = r * sin(t);
        if (t == -pi / 4 + ratia) {
            x1 = x;
            y1 = y;
        }
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 4 + ratia; t < pi / 4; t += ratia) {
        double r, x, y;
        r = -(a * sqrt(2 * cos(2 * t)));
        x = r * cos(t);
        y = r * sin(t);
        if (t == -pi / 4 + ratia) {
            x2 = x;
            y2 = y;
        }
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    glVertex2f(x2, y2);
    glVertex2f(x1, y1);
    glEnd();
}

//spirala logaritmica
void Display7() {
    double a = 0.02;
    double ratia = 0.05;
    double t;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = 0; t < 100; t += ratia) {
        double r, x, y;
        r = a * exp(1 + t);
        x = r * cos(t);
        y = r * sin(t);
        glVertex2f(x, y);
    }
    glEnd();
}



void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(4);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '1':
      Display1();
      break;
   case '2':
      Display2();
      break;
   case '3':
       Display3();
       break;
   case '4': 
       Display4();
       break;
   case '5':
       Display5();
       break;
   case '6':
       Display6();
       break;
   case '7':
       Display7();
   default:
      break;
   }

   glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);
   
   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}
