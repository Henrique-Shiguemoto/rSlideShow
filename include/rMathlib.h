#ifndef RMATHLIB_H
#define RMATHLIB_H

typedef struct rm_v2f {
    float x;
    float y;
} rm_v2f;

typedef struct rm_v3f {
    float x;
    float y;
    float z;
} rm_v3f;

typedef struct rm_v4f {
    float x;
    float y;
    float z;
    float w;
} rm_v4f;

typedef struct rm_mat2f {
    float elem[4];
} rm_mat2f;

typedef struct rm_mat3f {
    float elem[9];
} rm_mat3f;

typedef struct rm_mat4f {
    float elem[16];
} rm_mat4f;

typedef rm_v2f rm_point2D;
typedef rm_v3f rm_point3D;
typedef rm_v4f rm_quat;

typedef struct rm_line2D {
    rm_v2f direction;
    rm_point2D point;
} rm_line2D;

typedef struct rm_triangle2D {
    rm_point2D a;
    rm_point2D b;
    rm_point2D c;
} rm_triangle2D;

// It's important to be consistent with the order of the vertices. The user can only use either 
//      clockwise or anticlockwise ordering for vertices. Some of the functions used for this struct
//      can give you different results if you don't use these orders.
// Same thing applies to quad3D.
typedef struct rm_quad2D {
    rm_point2D a;
    rm_point2D b;
    rm_point2D c;
    rm_point2D d;
} rm_quad2D;


/**
 * 
 * Functions with this structure as parameter assumes the following
 *      (These assumptions are also analogous to AABB3D):
 * 
 * For y-is-up scenarios:
 *                              ^  (max4)
 *                              |            (max3)
 *                              |
 *                              |
 *        (min4)                |  (min3)
 *                              |
 *                              |        (max2)
 *        --------------------- 0 --------------------->
 *                     (max1)   |
 *                              |    (min2)
 *                              |
 *                 (min1)       |
 *                              |
 *                              |
 * - min.x and min.y are below and to the left of max.x and max.y
 * - min.x != max.x and min.y != max.y (so that AABBs don't determine a line segment)
 * 
 * 
 * For y-is-down scenarios:
 *                              |
 *                              |
 *                              |
 *                              |
 *        (min1)                |  (min2)
 *                              |
 *                              |        (max2)
 *        --------------------- 0 --------------------->
 *                   (min3)     |
 *                              |    (min4)
 *                              |
 *                              |               (max4)
 *                              |  (max1)
 *                       (max3) |
 *                              v
 * 
 * - min.x and min.y are up and to the left of max.x and max.y
 * - min.x != max.x and min.y != max.y (so that AABBs don't determine a line segment)
 * 
 * */ 
typedef struct rm_AABB2D {
    rm_point2D min;
    rm_point2D max;
} rm_AABB2D;

// REFACTOR: rename function (just call it a circle)
typedef struct rm_circle {
    rm_point2D center;
    float radius;
} rm_circle;

typedef struct rm_line3D {
    rm_v3f direction;
    rm_point3D point;
} rm_line3D;

typedef struct rm_triangle3D {
    rm_point3D a;
    rm_point3D b;
    rm_point3D c;
} rm_triangle3D;

typedef struct rm_quad3D {
    rm_point3D a;
    rm_point3D b;
    rm_point3D c;
    rm_point3D d;
} rm_quad3D;

typedef struct rm_plane {
    rm_v3f normal;
    rm_point3D point;
} rm_plane;

typedef struct rm_AABB3D {
    rm_point3D min;
    rm_point3D max;
} rm_AABB3D;

typedef struct rm_sphere {
    rm_point3D center;
    float radius;
} rm_sphere;

#define RMATH_INFINITY ((float)(1e+300)*(1e+300))
#define RMATH_NAN (((float)(1e+300)*(1e+300))*(0.0f))
#define RMATH_U32_MAX 4294967295
#define RMATH_PI 3.141592653589793
#define RMATH_DEGREE_IN_RAD (RMATH_PI/180)
#define RMATH_SQRT_2 1.414213562373095
#define RMATH_SQRT_3 1.732050807568877
#define RMATH_LOW_PRECISION 0.00001
#define RMATH_HIGH_PRECISION 0.0000000001
#define RMATH_BIAS32 127
#define RMATH_BIAS64 1023
#define RMATH_TRUE 1
#define RMATH_FALSE 0

// Vector Operations

rm_v2f rm_add_v2f(rm_v2f u, rm_v2f v);
rm_v2f rm_sub_v2f(rm_v2f u, rm_v2f v);
rm_v2f rm_scale_v2f(rm_v2f u, float scalar);
float rm_dot_v2f(rm_v2f u, rm_v2f v);
float rm_mag_v2f(rm_v2f u);
rm_v2f rm_unit_v2f(rm_v2f u);
int rm_compare_v2f(rm_v2f u, rm_v2f v, float errorMargin);

rm_v3f rm_add_v3f(rm_v3f u, rm_v3f v);
rm_v3f rm_sub_v3f(rm_v3f u, rm_v3f v);
rm_v3f rm_scale_v3f(rm_v3f u, float scalar);
float rm_dot_v3f(rm_v3f u, rm_v3f v);
rm_v3f rm_cross_v3f(rm_v3f u, rm_v3f v);
float rm_mag_v3f(rm_v3f u);
rm_v3f rm_unit_v3f(rm_v3f u);
int rm_compare_v3f(rm_v3f u, rm_v3f v, float errorMargin);
rm_v2f rm_convert_v3f_to_v2f(rm_v3f u);
float rm_mixed_product_v3f(rm_v3f a, rm_v3f b, rm_v3f c);

rm_v4f rm_add_v4f(rm_v4f u, rm_v4f v);
rm_v4f rm_sub_v4f(rm_v4f u, rm_v4f v);
rm_v4f rm_scale_v4f(rm_v4f u, float scalar);
float rm_dot_v4f(rm_v4f u, rm_v4f v);
float rm_mag_v4f(rm_v4f u);
rm_v4f rm_unit_v4f(rm_v4f u);
int rm_compare_v4f(rm_v4f u, rm_v4f v, float errorMargin);
rm_v3f rm_convert_v4f_to_v3f(rm_v4f u);

//Matrix Operations

rm_mat2f rm_add_mat2f(rm_mat2f m1, rm_mat2f m2);
rm_mat2f rm_sub_mat2f(rm_mat2f m1, rm_mat2f m2);
rm_mat2f rm_scale_mat2f(float scalar, rm_mat2f m1);
rm_mat2f rm_mult_mat2f(rm_mat2f m1, rm_mat2f m2);
float rm_det_mat2f(rm_mat2f m1);
rm_mat2f rm_transpose_mat2f(rm_mat2f m1);
rm_mat2f rm_inverse_mat2f(rm_mat2f m1);
rm_v2f rm_mult_mat2f_v2f(rm_mat2f m1, rm_v2f u);
rm_mat2f rm_identity_mat2f(void);
int rm_compare_mat2f(rm_mat2f m1, rm_mat2f m2, float errorMargin);

rm_mat3f rm_add_mat3f(rm_mat3f m1, rm_mat3f m2);
rm_mat3f rm_sub_mat3f(rm_mat3f m1, rm_mat3f m2);
rm_mat3f rm_scale_mat3f(float scalar, rm_mat3f m1);
rm_mat3f rm_mult_mat3f(rm_mat3f m1, rm_mat3f m2);
float rm_det_mat3f(rm_mat3f m1);
rm_mat3f rm_transpose_mat3f(rm_mat3f m1);
rm_mat3f rm_inverse_mat3f(rm_mat3f m1);
rm_v3f rm_mult_mat3f_v3f(rm_v3f u, rm_mat3f m1);
rm_mat3f rm_identity_mat3f(void);
int rm_compare_mat3f(rm_mat3f m1, rm_mat3f m2, float errorMargin);

rm_mat4f rm_add_mat4f(rm_mat4f m1, rm_mat4f m2);
rm_mat4f rm_sub_mat4f(rm_mat4f m1, rm_mat4f m2);
rm_mat4f rm_scale_mat4f(float scalar, rm_mat4f m1);
rm_mat4f rm_mult_mat4f(rm_mat4f m1, rm_mat4f m2);
float rm_det_mat4f(rm_mat4f m1);
rm_mat4f rm_transpose_mat4f(rm_mat4f m1);
rm_mat4f rm_inverse_mat4f(rm_mat4f m1);
rm_v4f rm_mult_mat4f_v4f(rm_v4f u, rm_mat4f m1);
rm_mat4f rm_identity_mat4f(void);
int rm_compare_mat4f(rm_mat4f m1, rm_mat4f m2, float errorMargin);

//Trig Functions

float rm_sin32(float rad);
float rm_cos32(float rad);
float rm_tg32(float rad);
float rm_cosec32(float rad);
float rm_sec32(float rad);
float rm_cotg32(float rad);
double rm_sin64(double rad);
double rm_cos64(double rad);
double rm_tg64(double rad);
double rm_cosec64(double rad);
double rm_sec64(double rad);
double rm_cotg64(double rad);

//Other convenient functions

int rm_compare32(float x, float y, float errorMargin);
float rm_deg_to_rad32(float deg);
float rm_rad_to_deg32(float rad);
float rm_sqrt32(float x);
float rm_abs32(float x);
float rm_clamp32(float min, float max, float value);
float rm_max32(float a, float b);
float rm_min32(float a, float b);

/**
 * 
 * RETURNS THE REMAINDER OF f1 WHEN DIVIDED BY f2 WITH 32 BIT FLOAT RETURN
 * 		IF positiveResult = TRUE, THEN THE RETURN VALUE WILL ALWAYS BE POSITIVE OR 0
 * 		OTHERWISE THE RETURN VALUES COULD BE NEGATIVE
 * 
 * */

float rm_mod32(float f1, float f2, int positiveResult);
int rm_ceil32(float x);
int rm_floor32(float x);
int rm_round32(float x);

int rm_compare64(double x, double y, double errorMargin);
double rm_deg_to_rad64(double deg);
double rm_rad_to_deg64(double rad);
double rm_sqrt64(double x);
double rm_abs64(double x);
double rm_clamp64(double min, double max, double value);
double rm_max64(double a, double b);
double rm_min64(double a, double b);

/**
 * 
 * RETURNS THE REMAINDER OF f1 WHEN DIVIDED BY f2 WITH 64 BIT FLOAT RETURN
 * 		IF positiveResult = TRUE, THEN THE RETURN VALUE WILL ALWAYS BE POSITIVE OR 0
 * 		OTHERWISE THE RETURN VALUES COULD BE NEGATIVE
 * 
 * */

double rm_mod64(double f1, double f2, int positiveResult);
long int rm_ceil64(double x);
long int rm_floor64(double x);
long int rm_round64(double x);
int rm_sign(double x);

//RANDOM FUNCTIONS

unsigned int rm_random_u32(void);
unsigned int rm_random_u32_in_interval(unsigned int min, unsigned int max);
int rm_random_bool(void);
int rm_random_sign(void);
float rm_random_f(void);              // returns float from 0.0f to 1.0f
rm_v2f rm_random_v2f(void);           // elements are from 0.0f to 1.0f
rm_v3f rm_random_v3f(void);           // elements are from 0.0f to 1.0f
rm_v4f rm_random_v4f(void);           // elements are from 0.0f to 1.0f
rm_mat2f rm_random_mat2f(void);       // elements are from 0.0f to 1.0f
rm_mat3f rm_random_mat3f(void);       // elements are from 0.0f to 1.0f
rm_mat4f rm_random_mat4f(void);       // elements are from 0.0f to 1.0f

//GRAPHICS FUNCTIONS

rm_mat3f rm_translation_2D(rm_v2f posDelta);
rm_mat3f rm_scaling_2D_set_origin(rm_v2f scale, rm_v2f origin);
rm_mat3f rm_scaling_2D(rm_v2f scale);
rm_mat3f rm_rotation_2D_around_point(float rad, rm_v2f pos);
rm_mat3f rm_rotation_2D(float rad);

rm_mat4f rm_translation_3D(rm_v3f posDelta);
rm_mat4f rm_scaling_3D(rm_v3f scale);
rm_mat4f rm_euler_rotationX_3D(float rad);
rm_mat4f rm_euler_rotationY_3D(float rad);
rm_mat4f rm_euler_rotationZ_3D(float rad);

/**
 * 
 * CREATES A 4x4 MATRIX WHICH ROTATES A 3D POINT (IN HOMOGENEOUS COORDINATE FORM) IN ALL THE AXIS
 *      (AXIS HIERARCHY IS X => Y => Z, Z IS THE MOST SIGNIFICANT)
 * 
 * */
rm_mat4f rm_euler_rotationXYZ_3D(rm_v3f rads);
rm_mat4f rm_parallel_projection_3D(float l, float r, float t, float b, float f, float n);
rm_mat4f rm_perspective_projection_3D(float fovY, float aspectRatio, float f, float n);

// GEOMETRY FUNCTIONS

float rm_distance_points2D(rm_point2D p, rm_point2D q);
float rm_distance_points3D(rm_point3D p, rm_point3D q);
float rm_distance_point_line2D(rm_point2D p, rm_line2D line);
float rm_distance_lines2D(rm_line2D line1, rm_line2D line2);
float rm_distance_point_Line3D(rm_point3D p, rm_line3D line);
float rm_distance_lines3D(rm_line3D line1, rm_line3D line2);
float rm_distance_point_plane(rm_point3D p, rm_plane pl);
float rm_distance_line_plane(rm_line3D line, rm_plane pl);
float rm_distance_planes(rm_plane pl1, rm_plane pl2);

int rm_parallel_lines2D(rm_line2D l1, rm_line2D l2);
int rm_parallel_lines3D(rm_line3D l1, rm_line3D l2);
int rm_intersecting_lines2D(rm_line2D l1, rm_line2D l2);
int rm_intersecting_lines3D(rm_line3D l1, rm_line3D l2);
int rm_skew_lines(rm_line3D l1, rm_line3D l2);

int rm_collision_AABB2D(rm_AABB2D r1, rm_AABB2D r2);
int rm_collision_point_AABB2D(rm_point2D p, rm_AABB2D r);
int rm_collision_point_circle(rm_point2D p, rm_circle s);
int rm_collision_circles(rm_circle s1, rm_circle s2);
int rm_collision_AABB3D(rm_AABB3D r1, rm_AABB3D r2);
int rm_collision_point_AABB3D(rm_point3D p, rm_AABB3D r);
int rm_collision_point_sphere(rm_point3D p, rm_sphere s);
int rm_collision_spheres(rm_sphere s1, rm_sphere s2);

float rm_area_triangle2D(rm_triangle2D triangle);
float rm_area_circle(rm_circle sphere);
float rm_area_AABB2D(rm_AABB2D aabb);
float rm_area_Quad2D(rm_quad2D quad);
float rm_perimeter_circle(rm_circle circle);
float rm_perimeter_AABB2D(rm_AABB2D aabb);
float rm_perimeter_quad2D(rm_quad2D quad);
float rm_perimeter_triangle2D(rm_triangle2D triangle);
float rm_area_triangle3D(rm_triangle3D triangle);
float rm_area_quad3D(rm_quad3D quad);
float rm_volume_sphere(rm_sphere sphere);
float rm_volume_AABB3D(rm_AABB3D aabb);
float rm_perimeter_triangle3D(rm_triangle3D triangle);
float rm_surface_area_sphere(rm_sphere sphere);
float rm_surface_area_AABB3D(rm_AABB3D aabb);

#endif // RMATHLIB_H
