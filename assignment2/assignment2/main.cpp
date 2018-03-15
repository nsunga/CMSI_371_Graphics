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

        if ((i + 1) % 3 == 0) {
            result.push_back(1.0f);
        }
    }
    
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
    vector<GLfloat> result;
    
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if ((i + 1) % 4 != 0) {
            result.push_back(homogenous_coords[i]);
        }
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
    bool sin_adjusted = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_x;
    
    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        sin_ = 0.0;
        sin_adjusted = true;
        cout << "less than eps sin" << endl;
    } else {
        sin_ = sin(radians_value);
    }

    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        cout << "less than eps cos" << endl;
        cos_ = 0.0;
    } else {
        cos_ = cos(radians_value);
    }

    if (sin_adjusted) {
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
    bool sin_adjusted = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_y;
    
    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        sin_ = 0.0;
        sin_adjusted = true;
        cout << "less than eps sin" << endl;
    } else {
        sin_ = sin(radians_value);
    }
    
    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        cout << "less than eps cos" << endl;
        cos_ = 0.0;
    } else {
        cos_ = cos(radians_value);
    }
    
    if (sin_adjusted) {
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
    bool sin_adjusted = false;
    float sin_;
    float cos_;
    float radians_value = degrees_to_radians(theta);
    vector<GLfloat> rotate_mat_z;
    
    
    if (fabs(sin(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        sin_ = 0.0;
        sin_adjusted = true;
        cout << "less than eps sin" << endl;
    } else {
        sin_ = sin(radians_value);
    }
    
    if (fabs(cos(radians_value) - 0.0) < numeric_limits<float>::epsilon()) {
        cout << "less than eps cos" << endl;
        cos_ = 0.0;
    } else {
        cos_ = cos(radians_value);
    }
    
    if (sin_adjusted) {
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
    vector<GLfloat> b_homog = to_homogenous_coord(B);
    vector<GLfloat> result;
    int change_element_counter = 0;
    float index_value = 0.0;
    bool done_multiplying = false;
    int lower_bound = 0;
    int upper_bound = 4;
    
    while (!done_multiplying) {
        vector<GLfloat> row_of_A(A.begin() + lower_bound, A.begin() + upper_bound);
        for (int i = 0; i < row_of_A.size(); i++) {
            cout << row_of_A[i] << " " ;
            if (upper_bound == A.size()) {
                done_multiplying = true;
            }
        }
        cout << "done" << endl;
        lower_bound = lower_bound + 4;
        upper_bound = upper_bound + 4;
    }
    
//    for (int i = 0; i < A.size(); i++) {
//        if (change_element_counter % 4 == 0 && change_element_counter != 0) {
//            result.push_back(index_value);
////            cout << "pushed" << index_value << endl;;
//            index_value = 0.0;
//        }
//        if (change_element_counter + 1 == b_homog.size()) {
//            index_value = index_value + A[i] * b_homog[change_element_counter++];
//            result.push_back(index_value);
//
//            break;
//        }
////        cout << A[i] << " * " << b_homog[change_element_counter] << endl;
//        index_value = index_value + A[i] * b_homog[change_element_counter++];
//    }
    
//    for (int i = 0; i < result.size(); i++) {
//        cout << result[i] << " ";
//    }
//    
    cout << "done" << endl;
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

/*TODO: DELETE THIS */
void print_homog_vector(vector<GLfloat> print_this) {
    int new_line_counter = 0;
    for (int i = 0; i < print_this.size(); i++) {
        cout << print_this[i] << ", ";
        new_line_counter = new_line_counter + 1;
        if (new_line_counter == 4) {
            cout << endl;
            new_line_counter = 0;
        }
    }
}

/*TODO: DELETE THIS */
void print_cart_vector(vector<GLfloat> print_this) {
    int new_line_counter = 0;
    for (int i = 0; i < print_this.size(); i++) {
        cout << print_this[i] << ", ";
        new_line_counter = new_line_counter + 1;
        if (new_line_counter == 3) {
            cout << endl;
            new_line_counter = 0;
        }
    }

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
    // Render our world
    //rotation_matrix_x(45.0);
    //print_homog_vector(rotation_matrix_z(90.0));
    vector<GLfloat> test_vector = {23.0, 42.0, 42.0, 0.0, 9.0, 11.0, 1.0, 0.0, 4.0, 13.0, 20.0, 7.0};
//    1.0, 0.0, 0.0, dx,
//    0.0, 1.0, 0.0, dy,
    mat_mult(translation_matrix(1.0, 1.0, 1.0), test_vector);
    glutMainLoop();
    cout << "passed main loop" << endl;

    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}

