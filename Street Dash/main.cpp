#include<stdlib.h> 
#include<math.h> 
#include<time.h> 
#include<string.h> 
#include<GL/glut.h> 
using namespace std;

GLint countx, county;
void robot_movement();
int flag = 0; // flag for movement
int flagw = 1; // flagw for no. of roads crossed
GLint count = 3;
float l = 0; // speed modifier

// Car vertices and components
GLfloat cv[][3] = { { -1.0,-0.75,0 },{ -1.25,-0.65,0.0 },{ -1.25,0.65,0 },{ -1.0,0.75,0.0 },{ 0.8,0.75,0.0 },{ 1.25,0.5,0.0 },{ 1.25,-0.5,0.0 },{ 0.8,-0.75,0.0 } };
GLfloat cs[][3] = { { -1.35,-0.5,0 },{ -1.35,0.5,0 },{ -1.25,0.5,0 },{ -1.25,-0.5,0 } };
GLfloat mrr[][3] = { { 0.15,-0.6,0 },{ 0.15,0.6,0 },{ 0.7,0.6,0 },{ 0.7,-0.6,0 } };
GLfloat light[][3] = { { 0,0,0 },{ 0,0.12,0 },{ 0.12,0.12,0 },{ 0.12,0,0 } };
GLfloat w[][3] = { { 0,0,0 },{ 0,0.12,0 },{ 0.33,0.12,0 },{ 0.33,0,0 } };

// Road components
GLfloat rv[4][3] = { { -15.0,0.0,0.0 },{ -15.0,2.0,0.0 },{ 15.0,2.0,0.0 },{ 15.0,0.0,0.0 } };
GLfloat rm[4][3] = { { 0.0,0.0,0.0 },{ 0.0,0.05,0.0 },{ 0.6,0.05,0.0 },{ 0.6,0.0,0.0 } };
GLfloat rd[4][3] = { { 0.0,0.0,0.0 },{ 0.0,0.250,0.0 },{ 2.0,0.250,0.0 },{ 2.0,0.0,0.0 } };

// Colors
GLfloat colors[][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 1.0,0.0,1.0 },{ 1.0,0.78,0.530 },{ 0.0,0.750,0.5 },{ 0.50,0.0,1.0 },{ 0.3,0.0,0.0 },{ 0.2,0.9,1.9 },{ 1.0,0.444,1.0 } };
GLfloat bw[][3] = { {0.0,0.0,0.0},{0.55,0.50,0.0} };
GLfloat mirclr[][3] = { { 0,0,1.0 },{ 1,1,1 } };

// Robot position - inital
GLfloat theta = 0, delta = 0, a[20][2], kx = 0.0, ky = -9.0;

// Vehicle position - inital
static float x = -10.0, y = 0.0;
static float x1 = -10.0, y11 = 4.50;
static float x2 = -10.0, y2 = -4.50;
static float x3 = 10.0, y3 = 2.25;
static float x4 = 10.0, y4 = -6.75;
static float x5 = 10.0, y5 = -2.25;
static float x6 = 10.0, y6 = 6.75;
static float x7 = -16.0, y7 = -4.50;
static float x8 = 16.0, y8 = -6.75;
static float x9 = 22.0, y9 = -6.75;

// arrow keys
void move_key(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT: flag = 1; break;  // Left arrow
    case GLUT_KEY_RIGHT: flag = 2; break; // Right arrow
    case GLUT_KEY_UP: flag = 3; break;    // Up arrow
    case GLUT_KEY_DOWN: flag = 4; break;  // Down arrow
    }
}

// keyboard input
void keyboard(unsigned char key, int x, int y) {
    if (key == 32) flagw++; // Space bar
}

// printing on screen
void output(int x, int y, const char* string) {
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

// draw square
void sqr(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d) {
    glBegin(GL_QUADS);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glVertex3fv(d);
    glEnd();
}

// car mirror
void carmirror() {
    glColor3fv(mirclr[0]);
    sqr(mrr[0], mrr[1], mrr[2], mrr[3]);
}

// car headlight
void carlight() {
    sqr(light[0], light[1], light[2], light[3]);
}

// car wheels
void wheel() {
    sqr(w[0], w[1], w[2], w[3]);
}

// circle - using polar coordinates
void circle(GLfloat r, GLfloat kx, GLfloat ky) {
    int i, n = 20;
    double MPI = 3.1415926;
    delta = (360 / n) * (MPI / 180);

    for (i = 0; i < n; i++) {
        a[i][0] = r * cos(theta) + kx;
        a[i][1] = r * sin(theta) + ky;
        theta += delta;
    }
    glBegin(GL_POLYGON);
    for (i = 0; i < n; i++)
        glVertex2fv(a[i]);
    glEnd();
}

// robot
void robot() {
    // head 
    glColor3f(1.0f, 0.75f, 0.8f);
    glRectf(kx - 0.3, ky + 0.5, kx + 0.3, ky + 1.1);

    // eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    circle(0.05, kx - 0.15, ky + 0.85);
    circle(0.05, kx + 0.15, ky + 0.85);

    // body
    glColor3f(1.0f, 0.6f, 0.7f);
    glRectf(kx - 0.4, ky - 0.5, kx + 0.4, ky + 0.5);

    // arms
    glColor3f(1.0f, 0.5f, 0.6f);
    glRectf(kx - 0.6, ky + 0.2, kx - 0.4, ky - 0.2);
    glRectf(kx + 0.4, ky + 0.2, kx + 0.6, ky - 0.2);

    // legs
    glColor3f(0.9f, 0.4f, 0.5f);
    glRectf(kx - 0.25, ky - 0.5, kx - 0.05, ky - 0.9);
    glRectf(kx + 0.05, ky - 0.5, kx + 0.25, ky - 0.9);
}

// car
void car() {
    // car's body
    glBegin(GL_POLYGON);
    glVertex3fv(cv[0]);
    glVertex3fv(cv[1]);
    glVertex3fv(cv[2]);
    glVertex3fv(cv[3]);
    glVertex3fv(cv[4]);
    glVertex3fv(cv[5]);
    glVertex3fv(cv[6]);
    glVertex3fv(cv[7]);
    glEnd();

    // car detailing
    glColor3f(0.0, 0.0, 0.0);
    sqr(cs[0], cs[1], cs[2], cs[3]);

    // headlights
    glColor3f(1, 1, 0);
    glTranslatef(1.065, 0.3, 0);
    carlight();
    glTranslatef(-1.065, -0.3, 0);
    glTranslatef(1.065, -0.45, 0);
    carlight();
    glTranslatef(-1.065, 0.45, 0);

    // mirror
    carmirror();
    glTranslatef(-1, -0, 0);
    carmirror();
    glTranslatef(1, 0, 0);

    // wheels
    glColor3f(0.0, 0.0, 0);
    glTranslatef(0.35, 0.75, 0);
    wheel();
    glTranslatef(-0.35, -0.75, 0);
    glTranslatef(0.35, -0.82, 0);
    wheel();
    glTranslatef(-0.35, 0.82, 0);
    glTranslatef(-0.8, 0.75, 0);
    wheel();
    glTranslatef(0.8, -0.75, 0);
    glTranslatef(-0.8, -0.82, 0);
    wheel();
    glTranslatef(0.8, 0.82, 0);
}

// roads
void drawroad(GLfloat x, GLfloat y) {
    int c = 0;
    int n = 7;
    while (n) {
        // lane divider
        while (x < 10.0) {
            glTranslatef(x, y, 0.0);
            glColor3fv(bw[c % 2]);
            sqr(rd[0], rd[1], rd[2], rd[3]);
            glTranslatef(-x, -y, -0.0);
            x = x + 2.0;
            c++;
        }

        // road (grey)
        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(0.0, (y + 0.25), 0.0);
        sqr(rv[0], rv[1], rv[2], rv[3]);
        glTranslatef(0.0, -(y + 0.25), 0.0);

        // central lining
        x = -10.0;
        while (x < 10.0) {
            glTranslatef(x, (y + 1.25), 0.0);
            glColor3f(1.0, 1.0, 1.0);
            sqr(rm[0], rm[1], rm[2], rm[3]);
            glTranslatef(-x, -(y + 1.25), 0.0);
            x = x + 1.250;
        }

        n--;
        y = y + 2.250;
        x = -10.0;
    }

    // last lane divider
    while (x < 10.0) {
        glTranslatef(x, y, 0.0);
        glColor3fv(bw[c % 2]);
        sqr(rd[0], rd[1], rd[2], rd[3]);
        glTranslatef(-x, -y, -0.0);
        x = x + 2.0;
        c++;
    }
}

// delay
void delay(int n1) {
    for (int i = 0; i < 9999 * n1; i++)
        for (int j = 0; j < 999 * n1; j++);
}

// life of robot
void dead() {
    flag = 100;
    delay(7);
}

// translation of vehicles
void vehicle() {
    // Right cars
    glColor3fv(colors[1]);
    glTranslatef(x, y, 0.0);
    car();
    glTranslatef(-x, -y, 0.0);
    x = x + 0.0210 + l;
    if (x >= 13.0) x = -10.0;

    glColor3fv(colors[2]);
    glTranslatef(x1, y11, 0.0);
    car();
    glTranslatef(-x1, -y11, 0.0);
    x1 = x1 + 0.0425 + l;
    if (x1 >= 13.0) x1 = -13.0;

    glColor3fv(colors[3]);
    glTranslatef(x2, y2, 0.0);
    car();
    glTranslatef(-x2, -y2, 0.0);
    x2 = x2 + 0.0220 + l;
    if (x2 >= 16.0) x2 = -10.0;

    glColor3fv(colors[0]);
    glTranslatef(x7, y7, 0.0);
    car();
    glTranslatef(-x7, -y7, 0.0);
    x7 = x7 + 0.0220 + l;
    if (x7 >= 10.0) x7 = -16.0;

    // Left cars
    glColor3fv(colors[4]);
    glTranslatef(x3, y3, 0.0);
    glRotatef(180, 0, 0, 1);
    car();
    glRotatef(180, 0, 0, 1);
    glTranslatef(-x3, -y3, 0.0);
    x3 = x3 - 0.0250 - l;
    if (x3 <= -13.0) x3 = 13.0;

    glColor3fv(colors[5]);
    glTranslatef(x4, y4, 0.0);
    glRotatef(180, 0, 0, 1);
    car();
    glRotatef(180, 0, 0, 1);
    glTranslatef(-x4, -y4, 0.0);
    x4 = x4 - 0.01250 - l;
    if (x4 <= -24.0) x4 = 10.0;

    glColor3fv(colors[6]);
    glTranslatef(x8, y8, 0.0);
    glRotatef(180, 0, 0, 1);
    car();
    glRotatef(180, 0, 0, 1);
    glTranslatef(-x8, -y8, 0.0);
    x8 = x8 - 0.01250 - l;
    if (x8 <= -18.0) x8 = 16.0;

    glColor3fv(colors[7]);
    glTranslatef(x9, y9, 0.0);
    glRotatef(180, 0, 0, 1);
    car();
    glRotatef(180, 0, 0, 1);
    glTranslatef(-x9, -y9, 0.0);
    x9 = x9 - 0.01250 - l;
    if (x9 <= -12.0) x9 = 22.0;

    glColor3fv(colors[8]);
    glTranslatef(x5, y5, 0.0);
    glRotatef(180, 0, 0, 1);
    car();
    glRotatef(180, 0, 0, 1);
    glTranslatef(-x5, -y5, 0.0);
    x5 = x5 - 0.01450 - l;
    if (x5 <= -13.0) x5 = 13.0;

    glColor3fv(colors[0]);
    glTranslatef(x6, y6, 0.0);
    glRotatef(180, 0, 0, 1);
    car();
    glRotatef(180, 0, 0, 1);
    glTranslatef(-x6, -y6, 0.0);
    x6 = x6 - 0.0210 - l;
    if (x6 <= -13.0) x6 = 13.0;
}

// robot movement
void robot_movement() {
    if (flag == 3 && ky < 9) { // Up
        ky = ky + 2.25;
        robot();
        flag = 0;
    }
    if (flag == 1 && kx > -9) { // Left
        kx = kx - 1.0;
        robot();
        flag = 0;
    }
    if (flag == 2 && kx < 9) { // Right
        kx = kx + 1.0;
        robot();
        flag = 0;
    }
    if (flag == 4 && ky > -9) { // Down
        ky = ky - 2.25;
        robot();
        flag = 0;
    }
}

// robot car collision check function
void check() {
    if (ky == y && ((x - 1 <= kx - 0.6 && kx - 0.6 <= x + 1) || (x - 1 <= kx + 0.6 && kx + 0.6 <= x + 1))) {
        dead();
    }
    if (ky == y11 && ((x1 - 1 <= kx - 0.6 && kx - 0.6 <= x1 + 1) || (x1 - 1 <= kx + 0.6 && kx + 0.6 <= x1 + 1))) {
        dead();
    }
    if (ky == y2 && ((x2 - 1 <= kx - 0.6 && kx - 0.6 <= x2 + 1) || (x2 - 1 <= kx + 0.6 && kx + 0.6 <= x2 + 1))) {
        dead();
    }
    if (ky == y7 && ((x7 - 1 <= kx - 0.6 && kx - 0.6 <= x7 + 1) || (x7 - 1 <= kx + 0.6 && kx + 0.6 <= x7 + 1))) {
        dead();
    }
    if (ky == y3 && ((x3 - 1 <= kx - 0.6 && kx - 0.6 <= x3 + 1) || (x3 - 1 <= kx + 0.6 && kx + 0.6 <= x3 + 1))) {
        dead();
    }
    if (ky == y4 && ((x4 - 1 <= kx - 0.6 && kx - 0.6 <= x4 + 1) || (x4 - 1 <= kx + 0.6 && kx + 0.6 <= x4 + 1))) {
        dead();
    }
    if (ky == y5 && ((x5 - 1 <= kx - 0.6 && kx - 0.6 <= x5 + 1) || (x5 - 1 <= kx + 0.6 && kx + 0.6 <= x5 + 1))) {
        dead();
    }
    if (ky == y6 && ((x6 - 1 <= kx - 0.6 && kx - 0.6 <= x6 + 1) || (x6 - 1 <= kx + 0.6 && kx + 0.6 <= x6 + 1))) {
        dead();
    }
    if (ky == y8 && ((x8 - 1 <= kx - 0.6 && kx - 0.6 <= x8 + 1) || (x8 - 1 <= kx + 0.6 && kx + 0.6 <= x8 + 1))) {
        dead();
    }
    if (ky == y9 && ((x9 - 1 <= kx - 0.6 && kx - 0.6 <= x9 + 1) || (x9 - 1 <= kx + 0.6 && kx + 0.6 <= x9 + 1))) {
        dead();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (flag == 100) { // Game over screen
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex2f(-10, -10);
        glVertex2f(-10, 10);
        glVertex2f(10, 10);
        glVertex2f(10, -10);
        glEnd();

        glColor3f(0.83, 0.69, 0.22);
        output(-2, 1, "--------------------");
        output(-1, 0, "GAME  OVER");
        output(-2, -1, "--------------------");
    }
    else if (ky >= 9.0) { // Win
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex2f(-10, -10);
        glVertex2f(-10, 10);
        glVertex2f(10, 10);
        glVertex2f(10, -10);
        glEnd();

        glColor3f(0.83, 0.69, 0.22);
        output(-2, 1, "---------------------------");
        output(-1.5, 0, "YOU WON! - FULL MARKS");
        output(-2, -1, "---------------------------");
    }
    else if (flagw == 1) { // Home screen
        glColor3f(0.83, 0.69, 0.22);
        output(-1, 3, "STREET DASH");
        output(-2, -5, "Project by:");
        output(1, -6, "102203103");
        output(1, -7, "102203520");
        output(-2, -6, "Krishnansh Verma");
        output(-2, -7, "Dhanishta Jaggi");
        output(-2, -3, "..use SPACE BAR to continue..");
    }
    else if (flagw == 2) { // Instructions screen
        glColor3f(0.83, 0.69, 0.22);
        output(-2, 3, "         INSTRUCTION");
        output(-3, 1, "use UP arrow key to move FORWARD");
        output(-3, 0, "use DOWN arrow key to move BACKWARD");
        output(-3, -1, "use LEFT arrow key to move LEFT");
        output(-3, -2, "use RIGHT arrow key to move RIGHT");
        output(-3, -4, "          use SPACE BAR to continue");
    }
    else {
        drawroad(-10.0, -8.0);
        check();
        vehicle();
        robot();

        if (flag == 1 || flag == 2 || flag == 3 || flag == 4) {
            robot_movement();
        }
    }

    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

// Reshape function
void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(75, 0);
    glutInitWindowSize(1400, 800);
    glutCreateWindow("Street Dash");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(move_key);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutMainLoop();
    return 0;
}