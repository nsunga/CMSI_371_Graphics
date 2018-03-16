/***
 Assignment-2: Geometric Modeling of a Scene
 
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
#include <iostream> /* TODO: REMOVE */
#include <string>
#include <limits>
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
//    vector<GLfloat> vertices = {
//        +1.0,   +1.0,   +0.0,
//        -1.0,   +1.0,   +0.0,
//        -1.0,   -1.0,   +0.0,
//        +1.0,   -1.0,   +0.0
//    };
    return vertices;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
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
    int counter = 0;
    
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if ((i + 1) % 4 != 0) { result.push_back(homogenous_coords[i]); }
        else {
            cout << "value: " << homogenous_coords[i] << endl;
            counter = counter + 1;
        }
    }
    cout << "counter: " << counter << endl;
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
    bool sin_adjusted = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_x;
    
//    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
//        sin_ = 0.0;
//        sin_adjusted = true;
//        cout << "less than eps sin" << endl;
//    } else { sin_ = sin(radians_value); }
//
//    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
//        cout << "less than eps cos" << endl;
//        cos_ = 0.0;
//    } else { cos_ = cos(radians_value); }
//
//    if (sin_adjusted) {
//        rotate_mat_x = {
//            1.0, 0.0, 0.0, 0.0,
//            0.0, cos_, sin_, 0.0,
//            0.0, sin_, cos_, 0.0,
//            0.0, 0.0, 0.0, 1.0
//        };
//    } else {
//        rotate_mat_x = {
//            1.0, 0.0, 0.0, 0.0,
//            0.0, cos_, -sin_, 0.0,
//            0.0, sin_, cos_, 0.0,
//            0.0, 0.0, 0.0, 1.0
//        };
//    }
    sin_ = sin(radians_value);
    cos_ = cos(radians_value);
    rotate_mat_x = {
        1.0, 0.0, 0.0, 0.0,
        0.0, cos_, -sin_, 0.0,
        0.0, sin_, cos_, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    return rotate_mat_x;
}


// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    // takes care of negative zero cause thats so dumb
    bool sin_adjusted = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_y;
    
//    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
//        sin_ = 0.0;
//        sin_adjusted = true;
//        cout << "less than eps sin" << endl;
//    } else { sin_ = sin(radians_value); }
//    
//    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
//        cout << "less than eps cos" << endl;
//        cos_ = 0.0;
//    } else { cos_ = cos(radians_value); }
//    
//    if (sin_adjusted) {
//        rotate_mat_y = {
//            cos_, 0.0, sin_, 0.0,
//            0.0, 1.0, 0.0, 0.0,
//            sin_, 0.0, cos_, 0.0,
//            0.0, 0.0, 0.0, 1.0
//        };
//    } else {
//        rotate_mat_y = {
//            cos_, 0.0, sin_, 0.0,
//            0.0, 1.0, 0.0, 0.0,
//            -sin_, 0.0, cos_, 0.0,
//            0.0, 0.0, 0.0, 1.0
//        };
//    }

    sin_ = sin(radians_value);
    cos_ = cos(radians_value);
    rotate_mat_y = {
        cos_, 0.0, sin_, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin_, 0.0, cos_, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    return rotate_mat_y;
}


// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    // takes care of negative zero cause thats so dumb
    bool sin_adjusted = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_z;
    
//    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
//        sin_ = 0.0;
//        sin_adjusted = true;
//        cout << "less than eps sin" << endl;
//    } else { sin_ = sin(radians_value); }
//    
//    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
//        cout << "less than eps cos" << endl;
//        cos_ = 0.0;
//    } else { cos_ = cos(radians_value); }
//    
//    if (sin_adjusted) {
//        rotate_mat_z = {
//            cos_, sin_, 0.0, 0.0,
//            sin_, cos_, 0.0, 0.0,
//            0.0, 0.0, 1.0, 0.0,
//            0.0, 0.0, 0.0, 1.0
//        };
//    } else {
//        rotate_mat_z = {
//            cos_, -sin_, 0.0, 0.0,
//            sin_, cos_, 0.0, 0.0,
//            0.0, 0.0, 1.0, 0.0,
//            0.0, 0.0, 0.0, 1.0
//        };
//    }
    sin_ = sin(radians_value);
    cos_ = cos(radians_value);
    rotate_mat_z = {
        cos_, -sin_, 0.0, 0.0,
        sin_, cos_, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    return rotate_mat_z;
}

// Transforms B into homog if not already homog
vector<GLfloat> mat_mult_helper(vector<GLfloat> B) {
    if (B.size() != 16) {
        cout << "IN MMH => turn into homog" << endl;
        return to_homogenous_coord(B);
    }
    return B;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> b_homog = mat_mult_helper(B);
    vector<GLfloat> result;
    int change_element_counter = 0;
    float index_value = 0.0;
    bool done_multiplying = false;
    int lower_bound = 0;
    int upper_bound = 4;
    int pts_in_b = b_homog.size()/4;
    
    while (!done_multiplying) {
        vector<GLfloat> row_of_A(A.begin() + lower_bound, A.begin() + upper_bound);
        change_element_counter = 0;
        
        for (int i = 0; i < pts_in_b; i++) {
            for (int i = 0; i < row_of_A.size(); i++) {
                index_value = index_value + (row_of_A[i] * b_homog[change_element_counter]);
                change_element_counter = change_element_counter + 1;
                
                if (i == 3) {
                    result.push_back(index_value);
                    index_value = 0.0;
                }
                
                if (upper_bound == A.size()) { done_multiplying = true; }
            }
        }
        lower_bound = lower_bound + 4;
        upper_bound = upper_bound + 4;
    }
    
    int offset = 0;
    vector<GLfloat> result_pts_notation;
    for (int i = 0; i < 4; i++) {
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
    vector<GLfloat> back_plane = mat_mult(translation_matrix(0.0, 0.0, -1.0), mat_mult(rotation_matrix_y(180), init_plane()));
    vector<GLfloat> front_plane = mat_mult(translation_matrix(0.0, 0.0, 1.0), init_plane());
    vector<GLfloat> right_plane = mat_mult(translation_matrix(1.0, 0.0, 0.0), mat_mult(rotation_matrix_y(90), init_plane()));
    vector<GLfloat> left_plane = mat_mult(translation_matrix(-1.0, 0.0, 0.0), mat_mult(rotation_matrix_y(-90), init_plane()));
    vector<GLfloat> top_plane = mat_mult(translation_matrix(0.0, 1.0, 0.0), mat_mult(rotation_matrix_x(-90), init_plane()));
    vector<GLfloat> bottom_plane = mat_mult(translation_matrix(0.0, -1.0, 0.0), mat_mult(rotation_matrix_x(90), init_plane()));
    vector<GLfloat> result;
    
//    for (int i = 0; i < front_plane.size(); i++) {
//        cout << front_plane[i] << ", ";
//        if ((i + 1) % 4 == 0) {
//            cout << "front plane col point" << endl;
//        }
//    }
    
    cout << endl;
    cout << endl;
    vector<GLfloat> front_plane_cart = to_cartesian_coord(front_plane);
//    for (int i = 0; i < front_plane_cart.size(); i++) {
//        cout << front_plane_cart[i] << ", ";
//        if ((i + 1) % 3 == 0) {
//            cout << "FPC col point" << endl;
//        }
//    }
    // every 16 points is a plane. every 4 points is a column of a plane
    for (int i = 0; i < front_plane.size(); i++) { result.push_back(front_plane[i]); }
    for (int i = 0; i < back_plane.size(); i++) { result.push_back(back_plane[i]); }
    for (int i = 0; i < right_plane.size(); i++) { result.push_back(right_plane[i]); }
    for (int i = 0; i < left_plane.size(); i++) { result.push_back(left_plane[i]); }
    for (int i = 0; i < top_plane.size(); i++) { result.push_back(top_plane[i]); }
    for (int i = 0; i < bottom_plane.size(); i++) { result.push_back(bottom_plane[i]); }
    // Creates a unit cube by transforming a set of planes
    
//    cout << "result size: " << result.size() << endl;
    
    return to_cartesian_coord(result);
//    return front_plane_cart;
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
    return nullptr;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    return nullptr;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(theta, 0.0, 1.0, 0.0);
    glRotatef(theta, 1.0, 0.0, 0.0);

    vector<GLfloat> front_plane_cart = build_cube();
//    GLfloat fpc_vertices[front_plane_cart.size()];
    GLfloat *fpc_vertices = vector2array(front_plane_cart);
//    for (int i = 0; i < 12; i++) {
//        cout << *(fpc_vertices + i) << ", ";
//        if ((i + 1) % 3 == 0) {
//            cout << "point fpc" << endl;
//        }
//    }
    // Perform display functions
//    GLfloat vertices[] = new GLfloat[12];
    
    GLfloat colors[] = {
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
//    GLfloat colors[] = {
//        // Front plane
//        0.5,    0.0,    0.0,
//        0.5,    0.0,    0.0,
//        0.5,    0.0,    0.0,
//        0.5,    0.0,    0.0,
//        // Back plane
//        0.0,    0.5,    0.0,
//        0.0,    0.5,    0.0,
//        0.0,    0.5,    0.0,
//        0.0,    0.5,    0.0,
//        // Right
//        0.0,    0.0,    0.5,
//        0.0,    0.0,    0.5,
//        0.0,    0.0,    0.5,
//        0.0,    0.0,    0.5,
//        // Left
//        0.5,    0.5,    0.0,
//        0.5,    0.5,    0.0,
//        0.5,    0.5,    0.0,
//        0.5,    0.5,    0.0,
//        // Top
//        0.5,    0.0,    0.5,
//        0.5,    0.0,    0.5,
//        0.5,    0.0,    0.5,
//        0.5,    0.0,    0.5,
//        // Bottom
//        0.0,    0.5,    0.5,
//        0.0,    0.5,    0.5,
//        0.0,    0.5,    0.5,
//        0.0,    0.5,    0.5,
//    };
    glVertexPointer(3, GL_FLOAT, 0, fpc_vertices);
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from
    glDrawArrays(GL_QUADS, 0, 4*6);
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
//    glutIdleFunc(idle_func);

    // Render our world
//    vector<GLfloat> test_vector = {23.0, 42.0, 42.0, 0.0, 9.0, 11.0, 0.0, 1.0, 2.0};
//    mat_mult(translation_matrix(1.0, 1.0, 1.0), test_vector);
//    test_vector = build_cube();
    
    glutMainLoop();
    cout << "passed main loop" << endl;

    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}

