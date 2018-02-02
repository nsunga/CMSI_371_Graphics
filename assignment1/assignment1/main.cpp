/***
 assignment1.cpp
 Assignment-1: Cartoonify
 
 Name: Sunga, Nicholas
 
 Collaborators: None bruh
 
 Project Summary: Project cartoonifies Kanye West's face. The curves are either
 basic GL_LINES or GL_LINES constructed from Bezier's algorithm. I used the
 iterative approach just so I don't blow up the stack.
 ***/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <iostream>
using namespace std;

class Vertex {
    GLfloat x, y;
    public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};

Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y;
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> points;
    // Generate points for a given Bezier curve iteration
    for (int i = 0; i < control_points.size()-1; i++) {
        Vertex v_0 = control_points[i];
        Vertex v_1 = control_points[i+1];
        
        GLfloat x_midpoint = (v_0.get_x() + v_1.get_x())/2;
        GLfloat y_midpoint = (v_0.get_y() + v_1.get_y())/2;

        points.push_back(Vertex(x_midpoint, y_midpoint));
    }
    
    // first and last points are assumed to be on the curve
    points.insert(points.begin(), Vertex(control_points[0].get_x(), control_points[0].get_y()));
    points.push_back(Vertex(control_points[control_points.size()-1].get_x(), control_points[control_points.size()-1].get_y()));
    
    return points;
}

void draw_curve(vector<Vertex> control_points, int n_iter) {
    vector<Vertex> points = control_points;

    while (n_iter > 0) {
        points = generate_points(points);
        n_iter--;
    }
    
    // Draw a Bezier curve based on the given control points
    glBegin(GL_LINES);
    for (int i = 0; i < points.size()-1; i++) {
        glVertex2f(points[i].get_x(), points[i].get_y());
        glVertex2f(points[i+1].get_x(), points[i+1].get_y());
    }
    glEnd();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);
    // Draw cartoon
    
    // Top head
    vector<Vertex>points;
    points.push_back(Vertex(-0.32f, 0.24f));
    points.push_back(Vertex(-0.47f, 1.0f));
    points.push_back(Vertex(-0.32f, 0.58));
    points.push_back(Vertex(-0.15, 0.70f));
    points.push_back(Vertex(0.0f, 1.50f));
    points.push_back(Vertex(0.15, 0.70f));
    points.push_back(Vertex(0.32f, 0.58));
    points.push_back(Vertex(0.47f, 1.0));
    points.push_back(Vertex(0.32f, 0.24f));
    draw_curve(points, 9);
    
    // hair
    points.clear();
    points.push_back(Vertex(-0.32f, 0.24f));
    points.push_back(Vertex(-0.33f, 0.27f));
    points.push_back(Vertex(-0.26f, 0.35f));
    draw_curve(points, 9);
    
    glBegin(GL_LINES);
    glVertex2f(-0.26f, 0.35f);
    glVertex2f(-0.26f, 0.60f);
    glEnd();
    
    points.clear();
    points.push_back(Vertex(-0.26f, 0.60f));
    points.push_back(Vertex(0.0f, 0.45f));
    points.push_back(Vertex(0.26f, 0.60f));
    draw_curve(points, 9);
    
    glBegin(GL_LINES);
    glVertex2f(0.26f, 0.60f);
    glVertex2f(0.26f, 0.35f);
    glEnd();
    
    points.clear();
    points.push_back(Vertex(0.26f, 0.35f));
    points.push_back(Vertex(0.33f, 0.27f));
    points.push_back(Vertex(0.32f, 0.24f));
    draw_curve(points, 9);

    // right ear
    points.clear();
    points.push_back(Vertex(0.32f, 0.24f));
    points.push_back(Vertex(0.36f, 0.45f));
    points.push_back(Vertex(0.40f, 0.24f));
    points.push_back(Vertex(0.39f, 0.22f));
    points.push_back(Vertex(0.35f, 0.18f));
    points.push_back(Vertex(0.35f, 0.16f));
    points.push_back(Vertex(0.34f, 0.05f));
    points.push_back(Vertex(0.32f, 0.10f));
    draw_curve(points, 9);
    
    // left ear
    points.clear();
    points.push_back(Vertex(-0.32f, 0.24f));
    points.push_back(Vertex(-0.36f, 0.45f));
    points.push_back(Vertex(-0.40f, 0.24f));
    points.push_back(Vertex(-0.39f, 0.22f));
    points.push_back(Vertex(-0.35f, 0.18f));
    points.push_back(Vertex(-0.35f, 0.16f));
    points.push_back(Vertex(-0.34f, 0.05f));
    points.push_back(Vertex(-0.32f, 0.10f));
    draw_curve(points, 9);
    
    // bottom head, jaw, chin
    points.clear();
    points.push_back(Vertex(0.32f, 0.14f));
    points.push_back(Vertex(0.35f, -0.08f));
    points.push_back(Vertex(0.28f, -0.19f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.28f, -0.19f));
    points.push_back(Vertex(0.24f, -0.32f));
    points.push_back(Vertex(0.05f, -0.45f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.05f, -0.45f));
    points.push_back(Vertex(0.0f, -0.47f));
    points.push_back(Vertex(-0.05f, -0.45f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.05f, -0.45f));
    points.push_back(Vertex(-0.24f, -0.32f));
    points.push_back(Vertex(-0.28f, -0.19f));
    draw_curve(points, 9);

    points.clear();
    points.push_back(Vertex(-0.28f, -0.19f));
    points.push_back(Vertex(-0.35f, -0.08f));
    points.push_back(Vertex(-0.32f, 0.14f));
    draw_curve(points, 9);

    // right eyebrow
    points.clear();
    points.push_back(Vertex(0.1f, 0.22f));
    points.push_back(Vertex(0.17f, 0.26f));
    points.push_back(Vertex(0.23f, 0.24f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.23f, 0.24f));
    points.push_back(Vertex(0.25f, 0.33f));
    points.push_back(Vertex(0.05f, 0.24f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.05f, 0.24f));
    points.push_back(Vertex(0.08f, 0.18f));
    points.push_back(Vertex(0.1f, 0.22f));
    draw_curve(points, 9);
    
    // left eyebrow
    points.clear();
    points.push_back(Vertex(-0.1f, 0.22f));
    points.push_back(Vertex(-0.17f, 0.26f));
    points.push_back(Vertex(-0.23f, 0.24f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.23f, 0.24f));
    points.push_back(Vertex(-0.25f, 0.33f));
    points.push_back(Vertex(-0.05f, 0.24f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.05f, 0.24f));
    points.push_back(Vertex(-0.08f, 0.18f));
    points.push_back(Vertex(-0.1f, 0.22f));
    draw_curve(points, 9);
    
    // right eye
    points.clear();
    points.push_back(Vertex(0.07f, 0.17f));
    points.push_back(Vertex(0.17f, 0.27f));
    points.push_back(Vertex(0.22f, 0.17f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.22f, 0.17f));
    points.push_back(Vertex(0.17f, 0.09f));
    points.push_back(Vertex(0.07f, 0.17f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.11f, 0.19f));
    points.push_back(Vertex(0.15f, 0.06f));
    points.push_back(Vertex(0.18f, 0.19f));
    draw_curve(points, 9);
    
    // left eye
    points.clear();
    points.push_back(Vertex(-0.07f, 0.17f));
    points.push_back(Vertex(-0.17f, 0.27f));
    points.push_back(Vertex(-0.22f, 0.17f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.07f, 0.17f));
    points.push_back(Vertex(-0.17f, 0.09f));
    points.push_back(Vertex(-0.22f, 0.17f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.11f, 0.19f));
    points.push_back(Vertex(-0.15f, 0.06f));
    points.push_back(Vertex(-0.18f, 0.19f));
    draw_curve(points, 9);
    
    // nose
    points.clear();
    points.push_back(Vertex(0.04f, 0.17f));
    points.push_back(Vertex(0.0f, 0.15f));
    points.push_back(Vertex(0.03f, 0.13f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.03f, 0.13f));
    points.push_back(Vertex(0.0f, 0.12f));
    points.push_back(Vertex(0.075f, -0.05f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.075f, -0.05f));
    points.push_back(Vertex(0.1f, -0.082f));
    points.push_back(Vertex(0.060f, -0.1f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.060f, -0.1f));
    points.push_back(Vertex(0.045f, -0.075f));
    points.push_back(Vertex(0.030f, -0.1f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.030f, -0.1f));
    points.push_back(Vertex(0.0f, -0.15f));
    points.push_back(Vertex(-0.030f, -0.1f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.030f, -0.1f));
    points.push_back(Vertex(-0.045f, -0.075f));
    points.push_back(Vertex(-0.060f, -0.1f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.060f, -0.1f));
    points.push_back(Vertex(-0.1f, -0.082f));
    points.push_back(Vertex(-0.075f, -0.05f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.075f, -0.05f));
    points.push_back(Vertex(0.0f, 0.12f));
    points.push_back(Vertex(-0.03f, 0.13f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.03f, 0.13f));
    points.push_back(Vertex(0.0f, 0.15f));
    points.push_back(Vertex(-0.04f, 0.17f));
    draw_curve(points, 9);
    
    // goatee baby
    points.clear();
    points.push_back(Vertex(-0.15f, -0.35f));
    points.push_back(Vertex(-0.15f, -0.25f));
    points.push_back(Vertex(-0.12f, -0.2f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.12f, -0.2f));
    points.push_back(Vertex(0.0f, 0.025f));
    points.push_back(Vertex(0.12f, -0.2f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.12f, -0.2f));
    points.push_back(Vertex(0.15f, -0.25f));
    points.push_back(Vertex(0.15f, -0.35f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.13f, -0.375f));
    points.push_back(Vertex(-0.13f, -0.25f));
    points.push_back(Vertex(-0.10f, -0.2f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.10f, -0.2f));
    points.push_back(Vertex(0.0f, -0.1f));
    points.push_back(Vertex(0.10f, -0.2f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.10f, -0.2f));
    points.push_back(Vertex(0.13f, -0.25f));
    points.push_back(Vertex(0.13f, -0.375f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.12f, -0.325f));
    points.push_back(Vertex(0.0f, -0.4f));
    points.push_back(Vertex(0.12f, -0.325f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.025f, -0.325f));
    points.push_back(Vertex(0.0f, -0.35f));
    points.push_back(Vertex(0.025f, -0.325f));
    draw_curve(points, 9);
    
    // lips
    points.clear();
    points.push_back(Vertex(-0.1f, -0.25f));
    points.push_back(Vertex(-0.05f, -0.15f));
    points.push_back(Vertex(-0.025f, -0.20f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.025f, -0.20f));
    points.push_back(Vertex(0.0f, -0.25f));
    points.push_back(Vertex(0.025f, -0.20f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.025f, -0.20f));
    points.push_back(Vertex(0.05f, -0.15f));
    points.push_back(Vertex(0.1f, -0.25f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(0.1f, -0.25f));
    points.push_back(Vertex(0.0f, -0.5f));
    points.push_back(Vertex(-0.1f, -0.25f));
    draw_curve(points, 9);
    
    points.clear();
    points.push_back(Vertex(-0.1f, -0.25f));
    points.push_back(Vertex(0.0f, -0.30f));
    points.push_back(Vertex(0.1f, -0.25f));
    draw_curve(points, 9);
    
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,600); // Set your own window size
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
