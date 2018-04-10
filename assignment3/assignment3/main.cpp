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
using namespace std;



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
    
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if (i == 0 || i % 3 != 0) { result.push_back(homogenous_coords[i]); }
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
    
    // Perform matrix multiplication for A B
    
    return result;
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    
    // Creates a unit cube by transforming a set of planes
    
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
    
    // Note: each plane (quad) contains 4 points, choose the points
    // to generate your vectors such that the normals (given by the
    // cross product, point to the correct direction
    
    return normals;
}

// Performs the cross product between two vectors
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> C;
    
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

// Performs the dot product between two vectors
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {
    
    return 0.0;
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
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    return nullptr;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    return nullptr;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Perform display functions
    
    glFlush();			//Finish rendering
    glutSwapBuffers();
}


int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 3");
    
    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);
    // Render our world
    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}

