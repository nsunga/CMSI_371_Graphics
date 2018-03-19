/***
 Assignment-2: Geometric Modeling of a Scene
 
 Name: Sunga, Nick
 
 Collaborators: Daniel Sjarif, Alejandro Zapata
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section
 
 Project Summary: I COULD NOT CREATE A SCENE. All I have to show is the unit
 cube with the camera rotating around it. I tried making transformations onto
 the cube in 'init_scene()' to create more interesting objects but the canvas
 was just a mess and looked like "modern art". All methods have been
 implemented though...Pls don't grade too hard.
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
#include <iostream>
using namespace std;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/
float theta = 0.0;

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
    
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if ((i + 1) % 4 != 0) { result.push_back(homogenous_coords[i]); }
    }
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate = {
        1.0, 0.0, 0.0, dx,
        0.0, 1.0, 0.0, dy,
        0.0, 0.0, 1.0, dz,
        0.0, 0.0, 0.0, 1.0
    };
    return translate;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale = {
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return scale;
}

// Converts degrees to radians
float degrees_to_radians(float theta) {
    return theta * (M_PI/180);
}

// Definition of a rotation matrix along the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    // takes care of negative zero cause thats so dumb
    bool sin_negat_zero = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_x;
    
    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        sin_ = 0.0;
        sin_negat_zero = true;
    } else { sin_ = sin(radians_value); }

    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        cos_ = 0.0;
    } else { cos_ = cos(radians_value); }

    if (sin_negat_zero) {
        rotate_mat_x = {
            1.0, 0.0, 0.0, 0.0,
            0.0, cos_, sin_, 0.0,
            0.0, sin_, cos_, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    } else {
        rotate_mat_x = {
            1.0, 0.0, 0.0, 0.0,
            0.0, cos_, -sin_, 0.0,
            0.0, sin_, cos_, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    }

    return rotate_mat_x;
}


// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    // takes care of negative zero cause thats so dumb
    bool sin_negat_zero = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_y;
    
    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        sin_ = 0.0;
        sin_negat_zero = true;
    } else { sin_ = sin(radians_value); }
    
    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        cos_ = 0.0;
    } else { cos_ = cos(radians_value); }
    
    if (sin_negat_zero) {
        rotate_mat_y = {
            cos_, 0.0, sin_, 0.0,
            0.0, 1.0, 0.0, 0.0,
            sin_, 0.0, cos_, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    } else {
        rotate_mat_y = {
            cos_, 0.0, sin_, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sin_, 0.0, cos_, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    }

    return rotate_mat_y;
}


// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    // takes care of negative zero cause thats so dumb
    bool sin_negat_zero = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_z;
    
    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        sin_ = 0.0;
        sin_negat_zero = true;
    } else { sin_ = sin(radians_value); }
    
    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        cos_ = 0.0;
    } else { cos_ = cos(radians_value); }
    
    if (sin_negat_zero) {
        rotate_mat_z = {
            cos_, sin_, 0.0, 0.0,
            sin_, cos_, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    } else {
        rotate_mat_z = {
            cos_, -sin_, 0.0, 0.0,
            sin_, cos_, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    }

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
    int offset = 0;
    vector<GLfloat> result_pts_notation;
    for (int i = 0; i < result.size()/4; i++) {
        for (int j = 0; j < 4; j++) {
            result_pts_notation.push_back(result[i + offset]);
            offset = offset + 4;
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
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
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
    gluPerspective(50.0, 1.0, 3.0, 7.0);
    gluLookAt(2.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {

//    tried doing tranformations to the cube but canvas was a mess
//    vector<GLfloat> cube_points = build_cube();
    
//    for (int i = 0; i < cube_points.size(); i++) {
//        cout << cube_points[i] << ", ";
//        if ((i + 1) % 4 == 0) { cout << endl; }
//    }
    
//    vector<GLfloat> result = mat_mult(translation_matrix(1.0, 1.0, 1.0), build_cube());
    
//    
//    for (int i = 0; i < result.size(); i++) {
//        cout << result[i] << ", ";
//        if ((i + 1) % 4 == 0) { cout << endl; }
//    }
    // print this out
//    GLfloat* results_vertices = vector2array(to_cartesian_coord(mat_mult(translation_matrix(1.0, 1.0, 1.0), build_cube())));
    GLfloat* results_vertices = vector2array(to_cartesian_coord(build_cube()));
    return results_vertices;
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
    glRotatef(theta, 1.0, 0.0, 0.0);

    GLfloat *results_vertices = init_scene();
    GLfloat *colors = init_color();

    glVertexPointer(3, GL_FLOAT, 0, results_vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays(GL_QUADS, 0, 4*6);
    delete results_vertices;
    delete colors;
    glFlush();			//Finish rendering
    glutSwapBuffers();
}
void idle_func() {
    theta = theta+0.3;
    display_func();
}

int main (int argc, char **argv) {

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 2");
    
    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);

    // Render our world
    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}

