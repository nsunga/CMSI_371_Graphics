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

vector<GLfloat> build_bed() {
    vector<GLfloat> bottom_frame = to_cartesian_coord(mat_mult(scaling_matrix(2.0, 0.5, 4.0), build_cube()));
    vector<GLfloat> actual_bed = to_cartesian_coord(mat_mult(scaling_matrix(1.5, 0.7, 3.5),
                                                             mat_mult(translation_matrix(0.0, 0.85, 0.0), build_cube())));
    vector<GLfloat> back_frame = to_cartesian_coord(mat_mult(translation_matrix(0.0, 0.7, 1.8),
                                                             mat_mult(rotation_matrix_x(90),
                                                             mat_mult(scaling_matrix(2.3, 0.4, 1.3), build_cube()))));
    
    vector<GLfloat> conjoined;
    for (int i = 0; i < bottom_frame.size(); i++) { conjoined.push_back(bottom_frame[i]); }
    for (int i = 0; i < actual_bed.size(); i++) { conjoined.push_back(actual_bed[i]); }
    for (int i = 0; i < back_frame.size(); i++) { conjoined.push_back(back_frame[i]); }

    return conjoined;
}

vector<GLfloat> build_dresser() {
    vector<GLfloat> dresser = to_cartesian_coord(mat_mult(translation_matrix(-2.5, 0.3, 1.5), build_cube()));
    vector<GLfloat> table_top = to_cartesian_coord(mat_mult(translation_matrix(-2.5, 0.8, 1.5),
                                                            mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    vector<GLfloat> table_top_two = to_cartesian_coord(mat_mult(translation_matrix(-2.8, 0.8, 1.5),
                                                                mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    vector<GLfloat> table_top_three = to_cartesian_coord(mat_mult(translation_matrix(-2.55, 0.8, 1.2),
                                                                mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    
    vector<GLfloat> conjoined;
    for (int i = 0; i < dresser.size(); i++) { conjoined.push_back(dresser[i]); }
    for (int i = 0; i < table_top.size(); i++) { conjoined.push_back(table_top[i]); }
    for (int i = 0; i < table_top_two.size(); i++) { conjoined.push_back(table_top_two[i]); }
    for (int i = 0; i < table_top_three.size(); i++) { conjoined.push_back(table_top_three[i]); }

    return conjoined;
}

vector<GLfloat> build_chair() {
    vector<GLfloat> leg = to_cartesian_coord(mat_mult(translation_matrix(2.5, -0.1, -2.5),
                                                      mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    vector<GLfloat> leg_two = to_cartesian_coord(mat_mult(translation_matrix(2.0, -0.1, -2.5),
                                                      mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    vector<GLfloat> leg_three = to_cartesian_coord(mat_mult(translation_matrix(2.5, -0.1, -2.0),
                                                          mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    vector<GLfloat> leg_four = to_cartesian_coord(mat_mult(translation_matrix(2.0, -0.1, -2.0),
                                                          mat_mult(scaling_matrix(0.2, 0.2, 0.2), build_cube())));
    vector<GLfloat> chair_body = to_cartesian_coord(mat_mult(translation_matrix(2.2, 0.5, -2.2), build_cube()));
    
    vector<GLfloat> conjoined;
    for (int i = 0; i < leg.size(); i++) { conjoined.push_back(leg[i]); }
    for (int i = 0; i < leg_two.size(); i++) { conjoined.push_back(leg_two[i]); }
    for (int i = 0; i < leg_three.size(); i++) { conjoined.push_back(leg_three[i]); }
    for (int i = 0; i < leg_four.size(); i++) { conjoined.push_back(leg_four[i]); }
    for (int i = 0; i < chair_body.size(); i++) { conjoined.push_back(chair_body[i]); }

    
    return conjoined;
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
    gluPerspective(70.0, 1.0, 3.0, 12.0);
//    gluLookAt(2.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(6.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    vector<GLfloat> bed = build_bed();
    vector<GLfloat> dresser = build_dresser();
    vector<GLfloat> chair = build_chair();
    vector<GLfloat> conjoined;
    for (int i = 0; i < bed.size(); i++) { conjoined.push_back(bed[i]); }
    for (int i = 0; i < dresser.size(); i++) { conjoined.push_back(dresser[i]); }
    for (int i = 0; i < chair.size(); i++) { conjoined.push_back(chair[i]); }

    GLfloat* results = vector2array(conjoined);
    return results;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    vector<GLfloat> colors = {
        // Bottom frame
        // Front plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Back plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Right
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Left
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Top
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Bottom
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // End Bottom frame
        
        // Bed
        // Front plane
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        // Back plane
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        // Right
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        // Left
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        // Top
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        // Bottom
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        0.980, 0.922, 0.843,
        // End Bed
        
        // Back frame
        // Front plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Back plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Right
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Left
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Top
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Bottom
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // End Back frame
        
        // Dresser
        // Front plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Back plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Right
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Left
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Top
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Bottom
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // End Dresser
        
        // Table Top
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Table Top
        
        // Table Top
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Table Top
        
        // Table Top
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Table Top
        
        // Chair Leg
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Chair Leg
        
        // Chair Leg
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Chair Leg
        
        // Chair Leg
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Chair Leg
        
        // Chair Leg
        // Front plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Back plane
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Right
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Left
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Top
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // Bottom
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        0.663, 0.663, 0.663,
        // End Chair Leg
        
        // Bottom frame
        // Front plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Back plane
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Right
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Left
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Top
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // Bottom
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        0.5f, 0.35f, 0.05f,
        // End Bottom frame
    };

    GLfloat* results_colors = vector2array(colors);
    return results_colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(theta, 0.0, 1.0, 0.0);
//    glRotatef(theta, 1.0, 0.0, 0.0);

    GLfloat *results_vertices = init_scene();
    GLfloat *colors = init_color();

    glVertexPointer(3, GL_FLOAT, 0, results_vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays(GL_QUADS, 0, 4*72);
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

