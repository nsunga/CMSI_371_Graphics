/***
 assignment1.cpp
 Assignment-1: Cartoonify
 
 Name: Sunga, Nicholas
 
 Collaborators: None bruh
 
 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project: e.g. did you use the iterative or recursive approach?
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
        std::cout << y_midpoint << std::endl;
        std::cout << x_midpoint << std::endl;

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
    
    vector<Vertex>points;
    points.push_back(Vertex(-0.25f, 0.0f));
    points.push_back(Vertex(0.0f, 0.25f));
    points.push_back(Vertex(0.25f, 0.0f));
    //points.push_back(Vertex(0.0f, -0.25f));
    
    draw_curve(points, 5);

//    glBegin(GL_LINES);
//    for(int i = 0; i < points.size() - 1; i++) {
//        glVertex2f(points[i].get_x(), points[i].get_y());
//        glVertex2f(points[i+1].get_x(), points[i+1].get_y());
//
//    }
//    glEnd();

    
//    vector<Vertex> control_point = generate_points(points);
//    control_point.push_back(Vertex(0.25f, 0.0f));
//    control_point.insert(control_point.begin(), Vertex(-0.25f, 0.0f));
//
//    glBegin(GL_LINES);
//    for (int i = 0; i < control_point.size()-1; i++) {
//        glVertex2f(control_point[i].get_x(), control_point[i].get_y());
//        glVertex2f(control_point[i+1].get_x(), control_point[i+1].get_y());
//    }
//    glEnd();
//    
//    control_point = generate_points(control_point);
//    control_point.push_back(Vertex(0.25f, 0.0f));
//    control_point.insert(control_point.begin(), Vertex(-0.25f, 0.0f));
//    
//    glBegin(GL_LINES);
//    for (int i = 0; i < control_point.size()-1; i++) {
//        glVertex2f(control_point[i].get_x(), control_point[i].get_y());
//        glVertex2f(control_point[i+1].get_x(), control_point[i+1].get_y());
//    }
//    glEnd();
//    control_point = generate_points(control_point);
//    control_point.push_back(Vertex(0.25f, 0.0f));
//    control_point.insert(control_point.begin(), Vertex(-0.25f, 0.0f));
//    
//    glBegin(GL_LINES);
//    for (int i = 0; i < control_point.size()-1; i++) {
//        glVertex2f(control_point[i].get_x(), control_point[i].get_y());
//        glVertex2f(control_point[i+1].get_x(), control_point[i+1].get_y());
//    }
//    glEnd();
//    control_point = generate_points(control_point);
//    control_point.push_back(Vertex(0.25f, 0.0f));
//    control_point.insert(control_point.begin(), Vertex(-0.25f, 0.0f));
//    
//    glBegin(GL_LINES);
//    for (int i = 0; i < control_point.size()-1; i++) {
//        glVertex2f(control_point[i].get_x(), control_point[i].get_y());
//        glVertex2f(control_point[i+1].get_x(), control_point[i+1].get_y());
//    }
//    glEnd();
//    control_point = generate_points(control_point);
//    control_point.push_back(Vertex(0.25f, 0.0f));
//    control_point.insert(control_point.begin(), Vertex(-0.25f, 0.0f));
//    
//    glBegin(GL_LINES);
//    for (int i = 0; i < control_point.size()-1; i++) {
//        glVertex2f(control_point[i].get_x(), control_point[i].get_y());
//        glVertex2f(control_point[i+1].get_x(), control_point[i+1].get_y());
//    }
//    glEnd();
//    glBegin(GL_POINTS);-
//    glVertex2f(0.0f, 0.5f);
//    glVertex2f(-0.5f, -0.5f);
//    glVertex2f(0.5f, -0.5f);
//    glEnd();
//    
//    // Draw 2 lines
//    glBegin(GL_LINES);
//    glVertex2f(0.0f, 0.5f);
//    glVertex2f(-0.5f, -0.5f);
//    
//    
//    glVertex2f(0.0f, 0.5f);
//    glVertex2f(0.5f, -0.5f);
    
    
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
