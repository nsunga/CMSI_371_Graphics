/***
 assignment1.cpp
 Assignment-1: Cartoonify
 
 Name: Sunga, Nicholas
 
 Collaborators: None bruh
 
 Project Summary: Project cartoonifies Kanye West's face. The curves are either
 basic GL_LINES or GL_LINES constructed from Bezier's algorithm. I used the
 iterative approach just so I don't blow up the stack, with t = 0.5. Uses
 init_sketch() helper function to generate points for every face component
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
    
    // first point assumed to be on curve
    points.insert(points.begin(), Vertex(control_points[0].get_x(),
                                         control_points[0].get_y()));

    // Bezier algorithm: push back all midpoints from each pair of points
    for (int i = 0; i < control_points.size()-1; i++) {
        Vertex v_0 = control_points[i];
        Vertex v_1 = control_points[i+1];
        GLfloat x_midpt = (v_0.get_x() + v_1.get_x())/2;
        GLfloat y_midpt = (v_0.get_y() + v_1.get_y())/2;
        points.push_back(Vertex(x_midpt, y_midpt));
    }
    
    // last point assumed to be on curve
    points.push_back(Vertex(control_points[control_points.size()-1].get_x(),
                            control_points[control_points.size()-1].get_y()));
    
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

vector<vector<Vertex>> init_sketch() {
    vector<vector<Vertex>> initial_points;
    
    // Top head
    vector<Vertex> top_head;
    top_head.push_back(Vertex(-0.32f, 0.24f));
    top_head.push_back(Vertex(-0.47f, 1.0f));
    top_head.push_back(Vertex(-0.32f, 0.58));
    top_head.push_back(Vertex(-0.15, 0.70f));
    top_head.push_back(Vertex(0.0f, 1.50f));
    top_head.push_back(Vertex(0.15, 0.70f));
    top_head.push_back(Vertex(0.32f, 0.58));
    top_head.push_back(Vertex(0.47f, 1.0));
    top_head.push_back(Vertex(0.32f, 0.24f));
    initial_points.push_back(top_head);
    
    // hair
    vector<Vertex> hair_left;
    hair_left.push_back(Vertex(-0.32f, 0.24f));
    hair_left.push_back(Vertex(-0.33f, 0.27f));
    hair_left.push_back(Vertex(-0.26f, 0.35f));
    initial_points.push_back(hair_left);
    glBegin(GL_LINES);
    glVertex2f(-0.26f, 0.35f);
    glVertex2f(-0.26f, 0.60f);
    glEnd();
    vector<Vertex> hair_top;
    hair_top.push_back(Vertex(-0.26f, 0.60f));
    hair_top.push_back(Vertex(0.0f, 0.45f));
    hair_top.push_back(Vertex(0.26f, 0.60f));
    initial_points.push_back(hair_top);
    glBegin(GL_LINES);
    glVertex2f(0.26f, 0.60f);
    glVertex2f(0.26f, 0.35f);
    glEnd();
    vector<Vertex> hair_right;
    hair_right.push_back(Vertex(0.26f, 0.35f));
    hair_right.push_back(Vertex(0.33f, 0.27f));
    hair_right.push_back(Vertex(0.32f, 0.24f));
    initial_points.push_back(hair_right);
    
    // right ear
    vector<Vertex> right_ear;
    right_ear.push_back(Vertex(0.32f, 0.24f));
    right_ear.push_back(Vertex(0.36f, 0.45f));
    right_ear.push_back(Vertex(0.40f, 0.24f));
    right_ear.push_back(Vertex(0.39f, 0.22f));
    right_ear.push_back(Vertex(0.35f, 0.18f));
    right_ear.push_back(Vertex(0.35f, 0.16f));
    right_ear.push_back(Vertex(0.34f, 0.05f));
    right_ear.push_back(Vertex(0.32f, 0.10f));
    initial_points.push_back(right_ear);
    
    // left ear
    vector<Vertex> left_ear;
    left_ear.push_back(Vertex(-0.32f, 0.24f));
    left_ear.push_back(Vertex(-0.36f, 0.45f));
    left_ear.push_back(Vertex(-0.40f, 0.24f));
    left_ear.push_back(Vertex(-0.39f, 0.22f));
    left_ear.push_back(Vertex(-0.35f, 0.18f));
    left_ear.push_back(Vertex(-0.35f, 0.16f));
    left_ear.push_back(Vertex(-0.34f, 0.05f));
    left_ear.push_back(Vertex(-0.32f, 0.10f));
    initial_points.push_back(left_ear);
    
    // bottom head, jaw, chin
    vector<Vertex> right_bottom_head;
    right_bottom_head.push_back(Vertex(0.32f, 0.14f));
    right_bottom_head.push_back(Vertex(0.35f, -0.08f));
    right_bottom_head.push_back(Vertex(0.28f, -0.19f));
    initial_points.push_back(right_bottom_head);
    vector<Vertex> right_jaw;
    right_jaw.push_back(Vertex(0.28f, -0.19f));
    right_jaw.push_back(Vertex(0.24f, -0.32f));
    right_jaw.push_back(Vertex(0.05f, -0.45f));
    initial_points.push_back(right_jaw);
    vector<Vertex> chin;
    chin.push_back(Vertex(0.05f, -0.45f));
    chin.push_back(Vertex(0.0f, -0.47f));
    chin.push_back(Vertex(-0.05f, -0.45f));
    initial_points.push_back(chin);
    vector<Vertex> left_jaw;
    left_jaw.push_back(Vertex(-0.05f, -0.45f));
    left_jaw.push_back(Vertex(-0.24f, -0.32f));
    left_jaw.push_back(Vertex(-0.28f, -0.19f));
    initial_points.push_back(left_jaw);
    vector<Vertex> left_bottom_head;
    left_bottom_head.push_back(Vertex(-0.28f, -0.19f));
    left_bottom_head.push_back(Vertex(-0.35f, -0.08f));
    left_bottom_head.push_back(Vertex(-0.32f, 0.14f));
    initial_points.push_back(left_bottom_head);
    
    // right eyebrow
    vector<Vertex> reyb_left;
    reyb_left.push_back(Vertex(0.1f, 0.22f));
    reyb_left.push_back(Vertex(0.17f, 0.26f));
    reyb_left.push_back(Vertex(0.23f, 0.24f));
    initial_points.push_back(reyb_left);
    vector<Vertex> reyb_top;
    reyb_top.push_back(Vertex(0.23f, 0.24f));
    reyb_top.push_back(Vertex(0.25f, 0.33f));
    reyb_top.push_back(Vertex(0.05f, 0.24f));
    initial_points.push_back(reyb_top);
    vector<Vertex> reyb_bottom;
    reyb_bottom.push_back(Vertex(0.05f, 0.24f));
    reyb_bottom.push_back(Vertex(0.08f, 0.18f));
    reyb_bottom.push_back(Vertex(0.1f, 0.22f));
    initial_points.push_back(reyb_bottom);
    
    // left eyebrow
    vector<Vertex> leyb_right;
    leyb_right.push_back(Vertex(-0.1f, 0.22f));
    leyb_right.push_back(Vertex(-0.17f, 0.26f));
    leyb_right.push_back(Vertex(-0.23f, 0.24f));
    initial_points.push_back(leyb_right);
    vector<Vertex> leyb_top;
    leyb_top.push_back(Vertex(-0.23f, 0.24f));
    leyb_top.push_back(Vertex(-0.25f, 0.33f));
    leyb_top.push_back(Vertex(-0.05f, 0.24f));
    initial_points.push_back(leyb_top);
    vector<Vertex> leyb_bottom;
    leyb_bottom.push_back(Vertex(-0.05f, 0.24f));
    leyb_bottom.push_back(Vertex(-0.08f, 0.18f));
    leyb_bottom.push_back(Vertex(-0.1f, 0.22f));
    initial_points.push_back(leyb_bottom);

    // right eye
    vector<Vertex> re_top;
    re_top.push_back(Vertex(0.07f, 0.17f));
    re_top.push_back(Vertex(0.17f, 0.27f));
    re_top.push_back(Vertex(0.22f, 0.17f));
    initial_points.push_back(re_top);
    vector<Vertex> re_bottom;
    re_bottom.push_back(Vertex(0.22f, 0.17f));
    re_bottom.push_back(Vertex(0.17f, 0.09f));
    re_bottom.push_back(Vertex(0.07f, 0.17f));
    initial_points.push_back(re_bottom);
    vector<Vertex> re_pupil;
    re_pupil.push_back(Vertex(0.11f, 0.19f));
    re_pupil.push_back(Vertex(0.15f, 0.06f));
    re_pupil.push_back(Vertex(0.18f, 0.19f));
    initial_points.push_back(re_pupil);
    
    // left eye
    vector<Vertex> le_top;
    le_top.push_back(Vertex(-0.07f, 0.17f));
    le_top.push_back(Vertex(-0.17f, 0.27f));
    le_top.push_back(Vertex(-0.22f, 0.17f));
    initial_points.push_back(le_top);
    vector<Vertex>le_bottom;
    le_bottom.push_back(Vertex(-0.07f, 0.17f));
    le_bottom.push_back(Vertex(-0.17f, 0.09f));
    le_bottom.push_back(Vertex(-0.22f, 0.17f));
    initial_points.push_back(le_bottom);
    vector<Vertex> le_pupil;
    le_pupil.push_back(Vertex(-0.11f, 0.19f));
    le_pupil.push_back(Vertex(-0.15f, 0.06f));
    le_pupil.push_back(Vertex(-0.18f, 0.19f));
    initial_points.push_back(le_pupil);
    
    // nose
    vector<Vertex> nose_right;
    nose_right.push_back(Vertex(0.04f, 0.17f));
    nose_right.push_back(Vertex(0.0f, 0.15f));
    nose_right.push_back(Vertex(0.03f, 0.13f));
    nose_right.push_back(Vertex(0.0f, 0.12f));
    nose_right.push_back(Vertex(0.075f, -0.05f));
    nose_right.push_back(Vertex(0.1f, -0.082f));
    nose_right.push_back(Vertex(0.060f, -0.1f));
    initial_points.push_back(nose_right);
    vector<Vertex> nose_nostrils;
    nose_nostrils.push_back(Vertex(0.060f, -0.1f));
    nose_nostrils.push_back(Vertex(0.050f, -0.1f));
    nose_nostrils.push_back(Vertex(0.045f, -0.045f));
    nose_nostrils.push_back(Vertex(0.030f, -0.1f));
    nose_nostrils.push_back(Vertex(0.0f, -0.15f));
    nose_nostrils.push_back(Vertex(-0.030f, -0.1f));
    nose_nostrils.push_back(Vertex(-0.045f, -0.045f));
    nose_nostrils.push_back(Vertex(-0.050f, -0.1f));
    nose_nostrils.push_back(Vertex(-0.060f, -0.1f));
    initial_points.push_back(nose_nostrils);
    vector<Vertex> nose_left;
    nose_left.push_back(Vertex(-0.060f, -0.1f));
    nose_left.push_back(Vertex(-0.1f, -0.082f));
    nose_left.push_back(Vertex(-0.075f, -0.05f));
    nose_left.push_back(Vertex(-0.075f, -0.05f));
    nose_left.push_back(Vertex(0.0f, 0.12f));
    nose_left.push_back(Vertex(-0.03f, 0.13f));
    nose_left.push_back(Vertex(-0.03f, 0.13f));
    nose_left.push_back(Vertex(0.0f, 0.15f));
    nose_left.push_back(Vertex(-0.04f, 0.17f));
    initial_points.push_back(nose_left);

    // goatee baby
    vector<Vertex> outer_goatee_left;
    outer_goatee_left.push_back(Vertex(-0.15f, -0.35f));
    outer_goatee_left.push_back(Vertex(-0.15f, -0.25f));
    outer_goatee_left.push_back(Vertex(-0.12f, -0.2f));
    initial_points.push_back(outer_goatee_left);
    vector<Vertex> outer_goatee_top;
    outer_goatee_top.push_back(Vertex(-0.12f, -0.2f));
    outer_goatee_top.push_back(Vertex(0.0f, 0.025f));
    outer_goatee_top.push_back(Vertex(0.12f, -0.2f));
    initial_points.push_back(outer_goatee_top);
    vector<Vertex> outer_goatee_right;
    outer_goatee_right.push_back(Vertex(0.12f, -0.2f));
    outer_goatee_right.push_back(Vertex(0.15f, -0.25f));
    outer_goatee_right.push_back(Vertex(0.15f, -0.35f));
    initial_points.push_back(outer_goatee_right);
    vector<Vertex> inner_goatee_left;
    inner_goatee_left.push_back(Vertex(-0.13f, -0.375f));
    inner_goatee_left.push_back(Vertex(-0.13f, -0.25f));
    inner_goatee_left.push_back(Vertex(-0.10f, -0.2f));
    initial_points.push_back(inner_goatee_left);
    vector<Vertex> inner_goatee_top;
    inner_goatee_top.push_back(Vertex(-0.10f, -0.2f));
    inner_goatee_top.push_back(Vertex(0.0f, -0.1f));
    inner_goatee_top.push_back(Vertex(0.10f, -0.2f));
    initial_points.push_back(inner_goatee_top);
    vector<Vertex> inner_goatee_right;
    inner_goatee_right.push_back(Vertex(0.10f, -0.2f));
    inner_goatee_right.push_back(Vertex(0.13f, -0.25f));
    inner_goatee_right.push_back(Vertex(0.13f, -0.375f));
    initial_points.push_back(inner_goatee_right);
    vector<Vertex> lip_line;
    lip_line.push_back(Vertex(-0.12f, -0.325f));
    lip_line.push_back(Vertex(0.0f, -0.4f));
    lip_line.push_back(Vertex(0.12f, -0.325f));
    initial_points.push_back(lip_line);
    vector<Vertex> stubble;
    stubble.push_back(Vertex(-0.025f, -0.325f));
    stubble.push_back(Vertex(0.0f, -0.35f));
    stubble.push_back(Vertex(0.025f, -0.325f));
    initial_points.push_back(stubble);
    
    // lips
    vector<Vertex> top_lip_left;
    top_lip_left.push_back(Vertex(-0.1f, -0.25f));
    top_lip_left.push_back(Vertex(-0.05f, -0.15f));
    top_lip_left.push_back(Vertex(-0.025f, -0.20f));
    initial_points.push_back(top_lip_left);
    vector<Vertex> top_lip_mid;
    top_lip_mid.push_back(Vertex(-0.025f, -0.20f));
    top_lip_mid.push_back(Vertex(0.0f, -0.25f));
    top_lip_mid.push_back(Vertex(0.025f, -0.20f));
    initial_points.push_back(top_lip_mid);
    vector<Vertex> top_lip_right;
    top_lip_right.push_back(Vertex(0.025f, -0.20f));
    top_lip_right.push_back(Vertex(0.05f, -0.15f));
    top_lip_right.push_back(Vertex(0.1f, -0.25f));
    initial_points.push_back(top_lip_right);
    vector<Vertex> lip_divider;
    lip_divider.push_back(Vertex(0.1f, -0.25f));
    lip_divider.push_back(Vertex(0.0f, -0.5f));
    lip_divider.push_back(Vertex(-0.1f, -0.25f));
    initial_points.push_back(lip_divider);
    vector<Vertex> bottom_lip;
    bottom_lip.push_back(Vertex(-0.1f, -0.25f));
    bottom_lip.push_back(Vertex(0.0f, -0.30f));
    bottom_lip.push_back(Vertex(0.1f, -0.25f));
    initial_points.push_back(bottom_lip);

    return initial_points;
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);
    
    vector<vector<Vertex>> initial_points = init_sketch();
    for (int i = 0; i < initial_points.size(); i++) {
        vector<Vertex> points_i = initial_points[i];
        draw_curve(points_i, 9);
    }
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
