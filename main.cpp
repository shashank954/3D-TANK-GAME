#include <iostream>
#include <GL/glu.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
using namespace std;
// #define VW 50
// #define VH 50
/* cos */

#define PI 3.14159265
#define g 0.25
const float DEG2RAD = 3.14159 / 180;
float circle_time = 0;

void drawCircle(float radius, float r)
{
    glPushMatrix();
    glTranslatef(1, r, 10);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glVertex3f(cos(degInRad) * radius, sin(degInRad) * radius, 0);
    }

    glEnd();
    glPopMatrix();
}
class tank
{
public:
    float x;
    float y;
    float z;
    float length;
    float height;
    float width;
    float turret_angle;
    float bullet_x;
    float bullet_y;
    float bullet_speed = 1;
    float vbx;
    float vby;
    float maxx;
    float maxy;
    float minx;
    float miny;
    float bullet_time;
    float z_angle;
    bool bullet;
    float health;
    bool show_trajectory;
    int wind_direction;
    float wind_speed;
    int dir;
    tank(int dir)
    {
        z_angle = 0;
        health = 0;
        wind_direction = rand() % 2;
        if (wind_direction == 0)
            wind_direction--;
        wind_speed = ((rand() % 10)) + 2;
        if (dir == 1)
        {
            this->dir = 1;
            show_trajectory = false;
            turret_angle = 0;
            bullet_time = 0;
            bullet = false;
            maxx = 20;
            maxy = 10;
            minx = 0;
            miny = 0;
            vbx = 2;
            vby = 2;
            length = 1;
            width = 1;
            height = 1;
            x = 0;
            y = 0;
            z = 0;
        }
        else
        {
            this->dir = -1;
            show_trajectory = false;
            turret_angle = 0;
            bullet_time = 0;
            bullet = false;
            maxx = 20;
            minx = 0;
            maxy = 30;
            miny = 20;
            vbx = 2;
            vby = 2;
            length = 1;
            width = 1;
            height = 1;
            x = 20;
            y = 30;
            z = 0;
        }
    }
    void updatex(float inc)
    {
        x += inc;
    }
    void updatey(float inc)
    {
        y += inc;
    }
    void updateangle(float inc)
    {
        turret_angle += inc;
    }
    void updateBulletX(float x1, float y1)
    {
        bullet_x = x1;
        bullet_y = y1;
    }
};
void init()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading
}
tank *tank1, *tank2, *current_tank;
bool trajectory_state = 0, finished = false;
int bullet_frame = 0;
float rotate_x, rotate_y;
void drawHealthBars()
{
    glPushMatrix();
    for (int i = 1; i <= tank1->health; i++)
    {
        glColor3f(0, 0.5, 0.5);
        glPushMatrix();
        glPointSize(11);
        glBegin(GL_POINTS);
        glVertex3f(0, 0, 28 + (i - 1));
        glEnd();
        glPopMatrix();
    }
    for (int i = 1; i <= tank2->health; i++)
    {
        glColor3f(1, 0.4, 0.3);
        glPushMatrix();
        glPointSize(11);
        glBegin(GL_POINTS);
        glVertex3f(0, 0, 20 + (i - 1));
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
}
void drawSphere(tank *t)
{
    glPushMatrix();
    glTranslatef(t->x, t->y, t->z + 10);
    glColor3f(1, 1, 0);
    glutSolidTorus(0.5, 1, 5, 20);
    glPopMatrix();
}
void drawWind(tank *t)
{
    glPushMatrix();
    if (t == tank1)
    {
        glBegin(GL_LINES);
        glColor3f(0, 0.5, 0.5);
        glVertex3f(0, 5, 20);
        glVertex3f(0, 5 + t->wind_speed* t->wind_direction, 20);
        glEnd();
        glBegin(GL_TRIANGLES);
        float adj;
        adj = 5 + t->wind_speed * t->wind_direction;
        glVertex3f(0,adj,20);
        glVertex3f(0,adj-1*t->wind_direction,22);
        glVertex3f(0,adj-1*t->wind_direction,18);
        glEnd();
    }
    else
    {
        glBegin(GL_LINES);
        glColor3f(1, 0.4, 0.3);
        glVertex3f(0, 20, 20);
        glVertex3f(0, 20 + t->wind_speed * t->wind_direction, 20);
        glEnd();
        glBegin(GL_TRIANGLES);
        float adj;
        adj = 20 + t->wind_speed * t->wind_direction;
        glVertex3f(0,adj,20);
        glVertex3f(0,adj-1*t->wind_direction,22);
        glVertex3f(0,adj-1*t->wind_direction,18);
        glEnd();
    }
    glEnd();

    glPopMatrix();
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_LINES);

    glVertex3f(0, 0, 1);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);
    glEnd();
    drawHealthBars();
    if (tank1->health == 5)
    {
        drawSphere(tank1);
        finished = true;
    }
    if (tank2->health == 5)
    {
        drawSphere(tank2);
        finished = true;
    }
    drawWind(tank1);
    drawWind(tank2);
    glBegin(GL_QUADS);
    glColor3f(0, 0.5, 0.5);
    glVertex3f(0, 0, -1);
    glVertex3f(22, 0, -1);
    glVertex3f(22, 12, -1);
    glVertex3f(0, 12, -1);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0, 0.8, -1);
    glVertex3f(0, 12, -1);
    glVertex3f(22, 12, -1);
    glVertex3f(22, 20, -1);
    glVertex3f(0, 20, -1);
    glEnd();

    
    glBegin(GL_QUADS);
    glColor3f(1, 0.4, 0.3);
    glVertex3f(0, 20, -1);
    glVertex3f(22, 20, -1);
    glVertex3f(22, 32, -1);
    glVertex3f(0, 32, -1);
    glEnd();
    glPushMatrix();
    glTranslatef(tank1->x, tank1->y, tank1->z);
    glRotatef(tank1->z_angle, 0, 0, 1);
    glPushMatrix();
    if (tank1->show_trajectory && current_tank == tank1 && !finished)
    {
        glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        float uy = (tank1->bullet_speed) * cos((tank1->turret_angle) * PI / 180) ;        
        float uz = (tank1->bullet_speed) * sin((tank1->turret_angle) * PI / 180);
        float Rfinal = 1 + 2 * uy * uz / g;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float degInRad = i * DEG2RAD;
            glVertex3f(1 + cos(degInRad) * 0.5, Rfinal + sin(degInRad) * 0.5, 0);
        }

        glEnd();

        float y = 1;
        float z = 1;
        float inc = Rfinal / 10;
        glPointSize(5);
        while (z >= 0)
        {
            glBegin(GL_POINTS);
            glColor3f(1, 0, 0);
            glVertex3f(0.5, y, z);
            glEnd();
            y += inc;
            z = 1 + (uz / uy) * (y - 1) - 0.5 * g * pow((y - 1) / uy, 2);
        }
    }
    glPopMatrix();

    if (tank1->bullet == true && current_tank == tank1 && !finished)
    {

        int t = tank1->bullet_time;
        float uy = (tank1->bullet_speed) * cos((tank1->turret_angle) * PI / 180)+0.1*tank1->wind_speed*tank1->wind_direction;
        float uz = (tank1->bullet_speed) * sin((tank1->turret_angle) * PI / 180);
        float y_b = 1 + (uy)*t;
        float z_b = 1 + (uz)*t - 0.5 * g * t * t;

        float bull_x = tank1->x - (uy)*sin(tank1->z_angle * PI / 180) * t;
        float bull_y = tank1->y + (uy)*cos(tank1->z_angle * PI / 180) * t;

        if (z_b >= 0)
        {

            glColor3f(0, 0, 10);
            glPushMatrix();
            glTranslatef(1, y_b, z_b);
            glutSolidSphere(1, 50, 50);
            glPopMatrix();
            tank1->bullet_time += 0.05;
        }
        else
        {
            if (circle_time <= 1000)
            {
                glBegin(GL_LINE_LOOP);
                for (int i = 0; i < 360; i++)
                {
                    float degInRad = i * DEG2RAD;
                    glColor3f(1,0, 0);
                    glVertex3f(1 + cos(degInRad) * 2, y_b + sin(degInRad) * 2, 0);
                    glVertex3f(1 + cos(degInRad) * 1.5, y_b + sin(degInRad) * 1.5, 0);

                }

                glEnd();
                circle_time++;
            }
            else
            {

                tank1->bullet_time = 0;
                circle_time = 0;
                tank1->bullet = false;
                bullet_frame = 0;
                current_tank = tank2;
                current_tank->show_trajectory = false;
                trajectory_state = 0;
                if (pow((tank2->y - bull_y), 2) + pow((tank2->x - bull_x), 2) <= 5)
                {
                    tank1->health++;
                }
            }
        }
    }
    glPushMatrix();
    glTranslatef(0, 1, 1);
    glRotatef(tank1->turret_angle - 90, 1, 0, 0);
    glPushMatrix();
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glColor3f(0.4, 0, 0);
    gluCylinder(quadratic, 0.2f, 0.2f, 5.0f, 32, 32);
    glPopMatrix();
    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.0f, 0.2f); // Red
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glColor3f(0.0f, 0.0f, 0.2f); // Orange
    glVertex3f(1.0 * tank1->length, 1.0 * tank1->height, 1.0 * tank1->height);
    glVertex3f(1.0 * tank1->length, 1.0 * tank1->height, -1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, 1.0 * tank1->height, -1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, 1.0 * tank1->height, 1.0 * tank1->width);

    glColor3f(0.0f, 0.0f, 0.2f); // Orange
    glVertex3f(1.0 * tank1->length, -1.0 * tank1->height, 1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, -1.0 * tank1->height, 1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, -1.0 * tank1->height, -1.0 * tank1->height);
    glVertex3f(1.0 * tank1->length, -1.0 * tank1->height, -1.0 * tank1->width);

    // Front face  (z = 1.0f)
    glColor3f(0.0f, 0.0f, 0.2f); // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(0.0f, 0.0f, 0.2f); // Red
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)

    // Right face (x = 1.0f)
    glColor3f(0.4f, 0.0f, 0.2f); // Red// Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glPopMatrix();
    glEnd();

    glPopMatrix();

    ////////////////////////////2nd Tank/////////////////////////////////////////////
    glPushMatrix();
    glTranslatef(tank2->x, tank2->y, tank2->z);
    glRotatef(tank2->z_angle, 0, 0, 1);

    glPushMatrix();
    if (tank2->show_trajectory && current_tank == tank2 && !finished)
    {
        glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        float uy = -(tank2->bullet_speed) * cos((tank2->turret_angle) * PI / 180);
        float uz = (tank2->bullet_speed) * sin((tank2->turret_angle) * PI / 180);
        float Rfinal = -1 + 2 * uy * uz / g;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float degInRad = i * DEG2RAD;
            glVertex3f(0.5 + cos(degInRad) * 0.5, Rfinal + sin(degInRad) * 0.5, 0);
        }
        glEnd();
        float y = -1;
        float z = 1;
        float inc = Rfinal / 10;
        glPointSize(5);
        while (z >= 0)
        {
            glBegin(GL_POINTS);
            glColor3f(1, 0, 0);
            glVertex3f(0.5, y, z);
            glEnd();
            y += inc;
            z = 1 + (uz / uy) * (y - 1) - 0.5 * g * pow((y - 1) / uy, 2);
        }
    }
    glPopMatrix();

    if (tank2->bullet == true && current_tank == tank2 && !finished)
    {

        int t = tank2->bullet_time;
        float uy = -(tank2->bullet_speed) * cos((tank2->turret_angle) * PI / 180)+0.1*tank2->wind_speed*tank2->wind_direction;
        float uz = (tank2->bullet_speed) * sin((tank2->turret_angle) * PI / 180);
        float y_b = 1 + (uy)*t;
        float z_b = 1 + (uz)*t - 0.5 * g * t * t;
        float bull_x = tank2->x - (uy)*sin(tank2->z_angle * PI / 180) * t;
        float bull_y = tank2->y + (uy)*cos(tank2->z_angle * PI / 180) * t;

        if (z_b >= 0)
        {

            glColor3f(1, 0.4, 0.3);
            glPushMatrix();
            glTranslatef(1, y_b, z_b);
            glutSolidSphere(1, 50, 50);
            glPopMatrix();
            tank2->bullet_time += 0.05;
        }
        else
        {
            if (circle_time <= 1000)
            {
                glBegin(GL_LINE_LOOP);
                for (int i = 0; i < 360; i++)
                {
                    float degInRad = i * DEG2RAD;
                    glColor3f(1,0, 0);
                    glVertex3f(1 + cos(degInRad) * 2, y_b + sin(degInRad) * 2, 0);
                    glVertex3f(1 + cos(degInRad) * 1.5, y_b + sin(degInRad) * 1.5, 0);
                }

                glEnd();
                circle_time++;
            }
            else
            {
                tank2->bullet_time = 0;
                circle_time = 0;
                tank2->bullet = false;
                bullet_frame = 0;
                current_tank = tank1;
                current_tank->show_trajectory = false;
                trajectory_state = 0;
                if (pow((tank1->y - bull_y), 2) + pow((tank1->x - bull_x), 2) <= 5)
                {
                    tank2->health++;
                }
            }
        }
    }
    glPushMatrix();
    glTranslatef(0.5, -1, 1);
    glRotatef(-tank2->turret_angle + 90, 1, 0, 0);
    glPushMatrix();
    GLUquadricObj *quadratic2;
    quadratic2 = gluNewQuadric();
    glColor3f(0.4, 0, 0);
    gluCylinder(quadratic2, 0.1f, 0.1f, 5.0f, 32, 32);
    glPopMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.2f); // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glColor3f(0.4, 0.0, 0.2); // Orange
    glVertex3f(1.0 * tank1->length, 1.0 * tank1->height, 1.0 * tank1->height);
    glVertex3f(1.0 * tank1->length, 1.0 * tank1->height, -1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, 1.0 * tank1->height, -1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, 1.0 * tank1->height, 1.0 * tank1->width);

    glColor3f(0.4, 0.0, 0.2); // Orange
    glVertex3f(1.0 * tank1->length, -1.0 * tank1->height, 1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, -1.0 * tank1->height, 1.0 * tank1->height);
    glVertex3f(-1.0 * tank1->length, -1.0 * tank1->height, -1.0 * tank1->height);
    glVertex3f(1.0 * tank1->length, -1.0 * tank1->height, -1.0 * tank1->width);

    // Front face  (z = 1.0f)
    glColor3f(.4f, .0f, .2f); // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(.4f, .0f, .2f); // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)

    // Right face (x = 1.0f)
    glColor3f(.0f, .0f, 0.2f); // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glPopMatrix();
    glEnd();
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}
void normakKeys(unsigned char key, int x, int y)
{
    if (key == 't')
    {
        if (trajectory_state == 0)
        {
            current_tank->show_trajectory = true;
        }
        else
        {
            current_tank->show_trajectory = false;
        }
        trajectory_state = 1 - trajectory_state;
    }
    if (key == 'f')
    {
        current_tank->bullet = true;
    }
    if (key == 'a')
    {
        if (current_tank->bullet_speed <= 5)
            current_tank->bullet_speed += 0.5;
    }
    if (key == 'd')
    {
        if (current_tank->bullet_speed >= 1)
            current_tank->bullet_speed -= 0.5;
    }
    if (key == 'w')
    {
        if (current_tank->turret_angle <= 85)
            current_tank->updateangle(2);
    }
    if (key == 's')
    {
        if (current_tank->turret_angle >= 5)
            current_tank->updateangle(-2);
    }
    if (key == 'j')
    {
        if (current_tank->z_angle >= -85)
        {
            current_tank->z_angle -= 5;
        }
    }
    if (key == 'l')
    {
        if (current_tank->z_angle <= 85)
        {
            current_tank->z_angle += 5;
        }
    }
}
void specialKeys(int key, int x, int y)
{

    if (key == GLUT_KEY_RIGHT)
    {
        if (current_tank->x >= current_tank->minx + 1)
            current_tank->updatex(-1);
    }
    else if (key == GLUT_KEY_LEFT)
    {
        if (current_tank->x <= current_tank->maxx - 1)
            current_tank->updatex(1);
    }
    else if (key == GLUT_KEY_UP)
    {
        if (current_tank->y >= current_tank->miny + 1)
            current_tank->updatey(-1);
    }
    else if (key == GLUT_KEY_DOWN)
    {
        if (current_tank->y <= current_tank->maxy - 1)
            current_tank->updatey(1);
    }
    glutPostRedisplay();
}
void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(100 / 60, timer, 0);

}
void reshape(int x, int y)
{
    if (y == 0 || x == 0)
        return;

    GLfloat aspect = (GLfloat)(x / y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 10.0f, 10000.0f);
    gluLookAt(50, 50, 50, 0.0, 0.1, 0.0, 0, 0, 1);
    glViewport(0, 0, x, y);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char **argv)
{
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Assignment 1");
    tank1 = new tank(1);
    tank2 = new tank(-1);
    current_tank = tank1;
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(normakKeys);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();
    glutMainLoop();
}
