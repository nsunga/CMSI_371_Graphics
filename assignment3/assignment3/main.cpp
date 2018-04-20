/***
 Assignment-3: Shading via Lighting and Colors
 
 Name: Sunga, Nick
 
 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section
 
 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project.
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

#include <math.h>
#include <vector>
#include <limits>

#include<iostream>
using namespace std;
float theta = 0.0;



/**************************************************
 *              Object Model Class                *
 *                                                *
 *  Stores the points of the object as a vector   *
 *  along with the colors and normals for each    *
 *  point. Normals are computed from the points.  *
 *                                                *
 *************************************************/
class ObjectModel {
    vector<GLfloat> _points;
    vector<GLfloat> _normals;
    vector<GLfloat> _base_colors;
    vector<GLfloat> _colors;
public:
    ObjectModel();
    vector<GLfloat> get_points() { return _points; };
    vector<GLfloat> get_normals() { return _normals; };
    vector<GLfloat> get_base_colors() { return _base_colors; };
    vector<GLfloat> get_colors() { return _colors; };
    void set_points(vector<GLfloat> points) { _points = points; };
    void set_normals(vector<GLfloat> normals) { _normals = normals; };
    void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
    void set_colors(vector<GLfloat> colors) { _colors = colors; };
};

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  Using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene.*
 *                                                *
 *************************************************/

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    
    for (int i = 0; i < vec.size(); i++) { arr[i] = vec[i]; }
    
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    
    for (int i = 0; i < cartesian_coords.size(); i++) {
        result.push_back(cartesian_coords[i]);
        if ((i + 1) % 3 == 0) { result.push_back(1.0f); }
    }
    
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
    vector<GLfloat> result;
    
//    for (int i = 0; i < homogenous_coords.size(); i++) {
//        if (i == 0 || i % 3 != 0) { result.push_back(homogenous_coords[i]); }
//    }
    
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if ((i + 1) % 4 != 0) { result.push_back(homogenous_coords[i]); }
    }
    
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat = {
        1.0, 0.0, 0.0, dx,
        0.0, 1.0, 0.0, dy,
        0.0, 0.0, 1.0, dz,
        0.0, 0.0, 0.0, 1.0
    };
    
    return translate_mat;
}


// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat = {
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return scale_mat;
}

// Converts degrees to radians
float radians(float ang) { return ang * (M_PI/180); }

// Definition of a rotation matrix along the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x;
    float _sin;
    float _cos;
    float _radians = radians(theta);
    bool sin_zero = fabs(sin(_radians) - 0.0) < numeric_limits<float>::epsilon();
    bool cos_zero = fabs(cos(_radians) - 0.0) < numeric_limits<float>::epsilon();
    
    if (sin_zero) { _sin = 0.0; }
    else { _sin = sin(_radians); }
    
    if (cos_zero) { _cos = 0.0; }
    else { _cos = cos(_radians); }
    
    rotate_mat_x = {
        1.0, 0.0, 0.0, 0.0,
        0.0, _cos, -_sin, 0.0,
        0.0, _sin, _cos, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_x;
}


// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y;
    float _sin;
    float _cos;
    float _radians = radians(theta);
    bool sin_zero = fabs(sin(_radians) - 0.0) < numeric_limits<float>::epsilon();
    bool cos_zero = fabs(cos(_radians) - 0.0) < numeric_limits<float>::epsilon();
    
    if (sin_zero) { _sin = 0.0; }
    else { _sin = sin(_radians); }
    
    if (cos_zero) { _cos = 0.0; }
    else { _cos = cos(_radians); }
    
    rotate_mat_y = {
        _cos, 0.0, _sin, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -_sin, 0.0, _cos, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_y;
}


// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z;
    float _sin;
    float _cos;
    float _radians = radians(theta);
    bool sin_zero = fabs(sin(_radians) - 0.0) < numeric_limits<float>::epsilon();
    bool cos_zero = fabs(cos(_radians) - 0.0) < numeric_limits<float>::epsilon();
    
    if (sin_zero) { _sin = 0.0; }
    else { _sin = sin(_radians); }
    
    if (cos_zero) { _cos = 0.0; }
    else { _cos = cos(_radians); }
    
    rotate_mat_z = {
        _cos, -_sin, 0.0, 0.0,
        _sin, _cos, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    int change_element = 0;
    float index_value = 0.0;
    bool done_multiplying = false;
    int lower_bound = 0;
    int upper_bound = 4;
    // pts in b is related to number of columns in B
    int pts_in_b = B.size()/4;
    
    // rows of A, times columns of B => B is always passed in as columns for every
    // four values
    while (!done_multiplying) {
        vector<GLfloat> row_of_A(A.begin() + lower_bound, A.begin() + upper_bound);
        change_element = 0;
        
        for (int i = 0; i < pts_in_b; i++) {
            for (int j = 0; j < row_of_A.size(); j++) {
                index_value = index_value + (row_of_A[j] * B[change_element]);
                change_element = change_element + 1;
            }
            result.push_back(index_value);
            index_value = 0.0;
        }
        if (upper_bound == A.size()) { done_multiplying = true; }
        lower_bound = lower_bound + 4;
        upper_bound = upper_bound + 4;
    }
    
    // orient the results in a way that every four values is a point, or a column
    // of the results matrix. Too confusing to jump column from column
    // Offset determined by #of points. Next value in a point is that index, plus offset.
    int offset = 0;
    vector<GLfloat> result_pts_notation;
    
    for (int i = 0; i < result.size()/4; i++) {
        for (int j = 0; j < 4; j++) {
            result_pts_notation.push_back(result[i + offset]);
            offset = offset + result.size()/4;
        }
        offset = 0;
    }
    
    return result_pts_notation;
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    // Creates a unit cube by transforming a set of planes
    vector<GLfloat> init_plane_homog = to_homogenous_coord(init_plane());
    vector<GLfloat> back_plane = mat_mult(translation_matrix(0.0, 0.0, -0.5), mat_mult(rotation_matrix_y(180), init_plane_homog));
    vector<GLfloat> front_plane = mat_mult(translation_matrix(0.0, 0.0, 0.5), init_plane_homog);
    vector<GLfloat> right_plane = mat_mult(translation_matrix(0.5, 0.0, 0.0), mat_mult(rotation_matrix_y(90), init_plane_homog));
    vector<GLfloat> left_plane = mat_mult(translation_matrix(-0.5, 0.0, 0.0), mat_mult(rotation_matrix_y(-90), init_plane_homog));
    vector<GLfloat> top_plane = mat_mult(translation_matrix(0.0, 0.5, 0.0), mat_mult(rotation_matrix_x(-90), init_plane_homog));
    vector<GLfloat> bottom_plane = mat_mult(translation_matrix(0.0, -0.5, 0.0), mat_mult(rotation_matrix_x(90), init_plane_homog));
    vector<GLfloat> result;
    
    // every 16 values is a plane. every 4 values is a column or point of a plane
    for (int i = 0; i < front_plane.size(); i++) { result.push_back(front_plane[i]); }
    for (int i = 0; i < back_plane.size(); i++) { result.push_back(back_plane[i]); }
    for (int i = 0; i < right_plane.size(); i++) { result.push_back(right_plane[i]); }
    for (int i = 0; i < left_plane.size(); i++) { result.push_back(left_plane[i]); }
    for (int i = 0; i < top_plane.size(); i++) { result.push_back(top_plane[i]); }
    for (int i = 0; i < bottom_plane.size(); i++) { result.push_back(bottom_plane[i]); }
    // 4 x 24 essentially. 24 col => 24 points
    
    return result;
}

/**************************************************
 *           Generating Surface Normals           *
 *                                                *
 *  Generate the surface normals of the objects   *
 *  using the cross product between two vectors   *
 *  that lie on the surface (plane) of interest.  *
 *  Recall that the direction of the normal to a  *
 *  surface follows the Right Hand Rule.          *
 *                                                *
 *************************************************/

// Generates the normals to each surface (plane)
vector<GLfloat> generate_normals(vector<GLfloat> points) {
    vector<GLfloat> normals;
    bool done_generating = false;
    int lower_bound = 0;
    int upper_bound = 12;


    while (!done_generating) {
        vector<GLfloat> plane(points.begin() + lower_bound, points.begin() + upper_bound);
        vector<GLfloat> q0(plane.begin(), points.begin() + 3);
        vector<GLfloat> q1(plane.begin() + 3, points.begin() + 6);
        vector<GLfloat> q3(plane.begin() + 6, points.begin() + 9);
        vector<GLfloat> q4(plane.begin() + 9, points.begin() + 12);
        
        // generate here
        
        if (upper_bound == points.size()) { done_generating = true; }
        lower_bound += 12;
        upper_bound += 12;
    }
    
    // Note: each plane (quad) contains 4 points, choose the points
    // to generate your vectors such that the normals (given by the
    // cross product, point to the correct direction
    
    return normals;
}

// Performs the cross product between two vectors
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> C;
    C.push_back(A[1]*B[2]-A[2]*B[1]);
    C.push_back(A[2]*B[0]-A[0]*B[2]);
    C.push_back(A[0]*B[1]-A[1]*B[0]);
    return C;
}


/**************************************************
 *         Shading via Lighting and Color         *
 *                                                *
 *  Generate the set of colors for each face of   *
 *  the planes that compose the objects in the    *
 *  scene. Based on the light source and surface  *
 *  normals, render the colors of the objects by  *
 *  applying the shading equation.                *
 *                                                *
 *************************************************/

// Initializes the base color of a plane to a single color
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    return base_color;
}

// Initializes the base color of a plane by specifying the color of each point
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
    // This enables OpenGL to use interpolation for (Gouraud) shading the plane
    vector<GLfloat> base_color = {
        r0,   g0,   b0,
        r1,   g1,   b1,
        r2,   g2,   b2,
        r3,   g3,   b3
    };
    return base_color;
}

// Generates the colors of a set of surfaces based on the light source,
// surface normals, and base color of the surface
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera) {
    vector<GLfloat> colors;
    
    object_model.set_colors(colors);
    return object_model;
}

// Allows for ambience (a vector of 3 values), diffusion (vector of 3 x n values) and specular (vector of 3 x n values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec) {
    vector<GLfloat> colors;
    
    object_model.set_colors(colors);
    return object_model;
}

// Performs the dot product between two vectors
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {
    float result = 0.0;
    for (int i = 0; i < B.size(); i++) { result += B[i] * A[i]; }
    return result;
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  Create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by using setting *
 *  the projection viewing matrices               *
 *                                                *
 *************************************************/

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 1.0, 3.0, 12.0);
    //    gluLookAt(2.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(6.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    return nullptr;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    vector<GLfloat> colors = {
        // Front plane
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        // Back plane
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        // Right
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        // Left
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        // Top
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        // Bottom
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
    };
    
    GLfloat* results_colors = vector2array(colors);
    return results_colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(theta, 0.0, 1.0, 0.0);
    
    GLfloat *results_vertices = vector2array(to_cartesian_coord(build_cube()));
    GLfloat *colors = init_color();

    glVertexPointer(3, GL_FLOAT, 0, results_vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays(GL_QUADS, 0, 4*6);
    delete results_vertices;
    // Perform display functions
    
    glFlush();			//Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    theta = theta+0.3;
    display_func();
}

int main (int argc, char **argv) {
//    // Initialize GLUT
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(800, 600);
//    // Create a window with rendering context and everything else we need
//    glutCreateWindow("Assignment 3");
//    
//    setup();
//    init_camera();
//    
//    // Set up our display function
//    glutDisplayFunc(display_func);
//    glutIdleFunc(idle_func);
//    // Render our world
//    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}

