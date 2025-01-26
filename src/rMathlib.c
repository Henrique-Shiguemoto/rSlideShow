#include "rMathlib.h"

//GLOBALS
static int seeded = RMATH_FALSE;
static unsigned int random_seed = 0;

//Checking which OS we're running on for absolute time query
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

static int systemFrequencySet = RMATH_FALSE;
static double systemFrequency = 0;

static double _rm_get_platform_time_for_rng(void){
    if(systemFrequencySet == RMATH_FALSE){
        LARGE_INTEGER t1;
        QueryPerformanceFrequency(&t1);   
        systemFrequency = 1.0 / t1.QuadPart;
    }

    LARGE_INTEGER t2;
    QueryPerformanceCounter(&t2);
    
    return (double)t2.QuadPart * systemFrequency;
}

#elif defined(__linux__) || defined(__gnu_linux__)

//Need to add this so we have access to clock_gettime() and CLOCK_MONOTONIC
#define _POSIX_C_SOURCE 199309L
#include <time.h>

double rm_get_platform_time_for_rng(void){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + (t.tv_nsec * 0.000000001);
}

#endif

//V2 IMPLEMENTATIONS

rm_v2f rm_add_v2f(rm_v2f u, rm_v2f v)       { return (rm_v2f){u.x + v.x, u.y + v.y}; }
rm_v2f rm_sub_v2f(rm_v2f u, rm_v2f v)       { return (rm_v2f){u.x - v.x, u.y - v.y}; }
rm_v2f rm_scale_v2f(rm_v2f u, float scalar) { return (rm_v2f){scalar*u.x, scalar*u.y}; }
float rm_dot_v2f(rm_v2f u, rm_v2f v)        { return (u.x * v.x) + (u.y * v.y); }
float rm_mag_v2f(rm_v2f u)                  { return rm_sqrt64((u.x*u.x) + (u.y*u.y)); }
rm_v2f rm_unit_v2f(rm_v2f u){ 
    double mag = rm_mag_v2f(u);
    if(mag == 0.0f) return (rm_v2f){0};
    return (rm_v2f){ u.x / mag, u.y / mag};
}
int rm_compare_v2f(rm_v2f u, rm_v2f v, float errorMargin){
    return rm_compare32(u.x, v.x, errorMargin) && 
           rm_compare32(u.y, v.y, errorMargin);
}

//V3 IMPLEMENTATIONS

rm_v3f rm_add_v3f(rm_v3f u, rm_v3f v)               { return (rm_v3f){u.x + v.x, u.y + v.y, u.z + v.z}; }
rm_v3f rm_sub_v3f(rm_v3f u, rm_v3f v)               { return (rm_v3f){u.x - v.x, u.y - v.y, u.z - v.z}; }
rm_v3f rm_scale_v3f(rm_v3f u, float scalar)         { return (rm_v3f){scalar*u.x, scalar*u.y, scalar*u.z}; }
float rm_dot_v3f(rm_v3f u, rm_v3f v)                { return (u.x * v.x) + (u.y * v.y) + (u.z * v.z); }
rm_v3f rm_cross_v3f(rm_v3f u, rm_v3f v)             { return (rm_v3f){ (u.y*v.z - u.z*v.y), -(u.x*v.z - u.z*v.x), (u.x*v.y - v.x*u.y)}; }
float rm_mag_v3f(rm_v3f u)                          { return rm_sqrt64((u.x*u.x) + (u.y*u.y) + (u.z*u.z)); }
rm_v3f rm_unit_v3f(rm_v3f u) {
    double mag = rm_mag_v3f(u);
    if(mag == 0.0f) return (rm_v3f){0};
    return (rm_v3f){ u.x / mag, u.y / mag, u.z / mag};
}
int rm_compare_v3f(rm_v3f u, rm_v3f v, float errorMargin) {
    return rm_compare32(u.x, v.x, errorMargin) && 
           rm_compare32(u.y, v.y, errorMargin) && 
           rm_compare32(u.z, v.z, errorMargin);
}
rm_v2f rm_convert_v3f_to_v2f(rm_v3f u)              { return (rm_v2f){ u.x, u.y }; }
float rm_mixed_product_v3f(rm_v3f a, rm_v3f b, rm_v3f c) { return rm_dot_v3f(rm_cross_v3f(a, b), c); }

//V4 IMPLEMENTATIONS

rm_v4f rm_add_v4f(rm_v4f u, rm_v4f v)       { return (rm_v4f){u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w}; }
rm_v4f rm_sub_v4f(rm_v4f u, rm_v4f v)       { return (rm_v4f){u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w}; }
rm_v4f rm_scale_v4f(rm_v4f u, float scalar) { return (rm_v4f){scalar*u.x, scalar*u.y, scalar*u.z, scalar*u.w}; }
float rm_dot_v4f(rm_v4f u, rm_v4f v)        { return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w; }
float rm_mag_v4f(rm_v4f u)                  { return rm_sqrt64((u.x*u.x) + (u.y*u.y) + (u.z*u.z) + (u.w*u.w)); }
rm_v4f rm_unit_v4f(rm_v4f u) {
    double mag = rm_mag_v4f(u);
    if(mag == 0) return (rm_v4f){0};
    return (rm_v4f){u.x / mag, u.y / mag, u.z / mag, u.w / mag};
}
int rm_compare_v4f(rm_v4f u, rm_v4f v, float errorMargin) {
    return rm_compare32(u.x, v.x, errorMargin) && 
           rm_compare32(u.y, v.y, errorMargin) && 
           rm_compare32(u.z, v.z, errorMargin) && 
           rm_compare32(u.w, v.w, errorMargin);
}
rm_v3f rm_convert_v4f_to_v3f(rm_v4f u) { return (rm_v3f){ u.x, u.y, u.z };}

//2x2 MATRIX SUPPORT

rm_mat2f rm_add_mat2f(rm_mat2f m1, rm_mat2f m2){
    rm_mat2f output = { 0 };
    for(int i = 0; i < 4; i++)
        output.elem[i] = m1.elem[i] + m2.elem[i];
    return output;
}

rm_mat2f rm_sub_mat2f(rm_mat2f m1, rm_mat2f m2){
    rm_mat2f output = { 0 };
    for(int i = 0; i < 4; i++)
        output.elem[i] = m1.elem[i] - m2.elem[i];
    return output;
}

rm_mat2f rm_scale_mat2f(float scalar, rm_mat2f m1){
    rm_mat2f output = {0};
    for(int i = 0; i < 4; i++)
        output.elem[i] = scalar*m1.elem[i];
    return output;
}

rm_mat2f rm_mult_mat2f(rm_mat2f m1, rm_mat2f m2){
    rm_mat2f output = {0};
    output.elem[0] = m1.elem[0]*m2.elem[0] + m1.elem[1]*m2.elem[2];
    output.elem[1] = m1.elem[0]*m2.elem[1] + m1.elem[1]*m2.elem[3];
    output.elem[2] = m1.elem[2]*m2.elem[0] + m1.elem[3]*m2.elem[2];
    output.elem[3] = m1.elem[2]*m2.elem[1] + m1.elem[3]*m2.elem[3];
    return output;
}

float rm_det_mat2f(rm_mat2f m1){
    return m1.elem[0]*m1.elem[3] - 
           m1.elem[1]*m1.elem[2];
}

rm_mat2f rm_transpose_mat2f(rm_mat2f m1){
    rm_mat2f output = { 0 };
    output.elem[0] = m1.elem[0];
    output.elem[1] = m1.elem[2];
    output.elem[2] = m1.elem[1];
    output.elem[3] = m1.elem[3];
    return output;
}

rm_mat2f rm_inverse_mat2f(rm_mat2f m1){
    double det = rm_det_mat2f(m1);
    if(det == 0) return (rm_mat2f){0};

    rm_mat2f adjugate = { 0 };
    adjugate.elem[0] = m1.elem[3];
    adjugate.elem[1] = -m1.elem[1];
    adjugate.elem[2] = -m1.elem[2];
    adjugate.elem[3] = m1.elem[0];

    return rm_scale_mat2f(1/det, adjugate);
}

rm_v2f rm_mult_mat2f_v2f(rm_mat2f m1, rm_v2f u){
    return (rm_v2f){u.x * m1.elem[0] + u.y * m1.elem[2], 
                    u.x * m1.elem[1] + u.y * m1.elem[3]};
}

rm_mat2f rm_identity_mat2f(void){
    rm_mat2f output = { 0 };
    
    output.elem[0] = 1.0;
    output.elem[3] = 1.0;

    return output;
}

int rm_compare_mat2f(rm_mat2f m1, rm_mat2f m2, float errorMargin){
    for(int i = 0; i < 4; i++)
        if(!rm_compare32(m1.elem[i], m2.elem[i], errorMargin)) return RMATH_FALSE;
    return RMATH_TRUE;
}

//3x3 MATRIX SUPPORT

rm_mat3f rm_add_mat3f(rm_mat3f m1, rm_mat3f m2){
    rm_mat3f output = {0};
    for(int i = 0; i < 9; i++)
        output.elem[i] = m1.elem[i] + m2.elem[i];
    return output;
}

rm_mat3f rm_sub_mat3f(rm_mat3f m1, rm_mat3f m2){
    rm_mat3f output = {0};
    for(int i = 0; i < 9; i++)
        output.elem[i] = m1.elem[i] - m2.elem[i];
    return output;
}

rm_mat3f rm_scale_mat3f(float scalar, rm_mat3f m1){
    rm_mat3f output = {0};
    for(int i = 0; i < 9; i++)
        output.elem[i] = scalar*m1.elem[i];
    return output;
}

rm_mat3f rm_mult_mat3f(rm_mat3f m1, rm_mat3f m2){
    rm_mat3f output = { 0 };

    output.elem[0] = m1.elem[0]*m2.elem[0] + m1.elem[1]*m2.elem[3] + m1.elem[2]*m2.elem[6];
    output.elem[1] = m1.elem[0]*m2.elem[1] + m1.elem[1]*m2.elem[4] + m1.elem[2]*m2.elem[7];
    output.elem[2] = m1.elem[0]*m2.elem[2] + m1.elem[1]*m2.elem[5] + m1.elem[2]*m2.elem[8];
    output.elem[3] = m1.elem[3]*m2.elem[0] + m1.elem[4]*m2.elem[3] + m1.elem[5]*m2.elem[6];
    output.elem[4] = m1.elem[3]*m2.elem[1] + m1.elem[4]*m2.elem[4] + m1.elem[5]*m2.elem[7];
    output.elem[5] = m1.elem[3]*m2.elem[2] + m1.elem[4]*m2.elem[5] + m1.elem[5]*m2.elem[8];
    output.elem[6] = m1.elem[6]*m2.elem[0] + m1.elem[7]*m2.elem[3] + m1.elem[8]*m2.elem[6];
    output.elem[7] = m1.elem[6]*m2.elem[1] + m1.elem[7]*m2.elem[4] + m1.elem[8]*m2.elem[7];
    output.elem[8] = m1.elem[6]*m2.elem[2] + m1.elem[7]*m2.elem[5] + m1.elem[8]*m2.elem[8];

    return output;
}

float rm_det_mat3f(rm_mat3f m1){
    return m1.elem[0]*m1.elem[4]*m1.elem[8] + 
           m1.elem[1]*m1.elem[5]*m1.elem[6] + 
           m1.elem[2]*m1.elem[3]*m1.elem[7] -
           m1.elem[6]*m1.elem[4]*m1.elem[2] -
           m1.elem[7]*m1.elem[5]*m1.elem[0] -
           m1.elem[8]*m1.elem[3]*m1.elem[1];
}

rm_mat3f rm_transpose_mat3f(rm_mat3f m1){
    rm_mat3f output = { 0 };
    output.elem[0] = m1.elem[0];
    output.elem[1] = m1.elem[3];
    output.elem[2] = m1.elem[6];
    output.elem[3] = m1.elem[1];
    output.elem[4] = m1.elem[4];
    output.elem[5] = m1.elem[7];
    output.elem[6] = m1.elem[2];
    output.elem[7] = m1.elem[5];
    output.elem[8] = m1.elem[8];
    return output;
}

rm_mat3f rm_inverse_mat3f(rm_mat3f m1){
    double det = rm_det_mat3f(m1);
    if(det == 0) return (rm_mat3f){0};

    rm_mat3f t = rm_transpose_mat3f(m1);

    double det00 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[4], t.elem[5], t.elem[7], t.elem[8]}});
    double det01 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[3], t.elem[5], t.elem[6], t.elem[8]}});
    double det02 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[3], t.elem[4], t.elem[6], t.elem[7]}});
    double det10 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[1], t.elem[2], t.elem[7], t.elem[8]}});
    double det11 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[0], t.elem[2], t.elem[6], t.elem[8]}});
    double det12 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[0], t.elem[1], t.elem[6], t.elem[7]}});
    double det20 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[1], t.elem[2], t.elem[4], t.elem[5]}});
    double det21 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[0], t.elem[2], t.elem[3], t.elem[5]}});
    double det22 = rm_det_mat2f((rm_mat2f){.elem = {t.elem[0], t.elem[1], t.elem[3], t.elem[4]}});

    rm_mat3f adjugate = {.elem = {det00, -det01,  det02, 
                               -det10,  det11, -det12, 
                                det20, -det21,  det22}};

    return rm_scale_mat3f(1/det, adjugate);
}

rm_v3f rm_mult_mat3f_v3f(rm_v3f u, rm_mat3f m1){
    return (rm_v3f){u.x * m1.elem[0] + u.y * m1.elem[3] + u.z * m1.elem[6],
                    u.x * m1.elem[1] + u.y * m1.elem[4] + u.z * m1.elem[7],
                    u.x * m1.elem[2] + u.y * m1.elem[5] + u.z * m1.elem[8]};
}

rm_mat3f rm_identity_mat3f(void){
    rm_mat3f output = { 0 };

    output.elem[0] = 1.0f;
    output.elem[4] = 1.0f;
    output.elem[8] = 1.0f;

    return output;
}

int rm_compare_mat3f(rm_mat3f m1, rm_mat3f m2, float errorMargin){
    for(int i = 0; i < 9; i++)
        if(!rm_compare32(m1.elem[i], m2.elem[i], errorMargin)) return RMATH_FALSE;
    return RMATH_TRUE;
}

//4x4 MATRIX SUPPORT

rm_mat4f rm_add_mat4f(rm_mat4f m1, rm_mat4f m2){
    rm_mat4f output = {0};
    for(int i = 0; i < 16; i++)
        output.elem[i] = m1.elem[i] + m2.elem[i];
    return output;
}

rm_mat4f rm_sub_mat4f(rm_mat4f m1, rm_mat4f m2){
    rm_mat4f output = {0};
    for(int i = 0; i < 16; i++)
        output.elem[i] = m1.elem[i] - m2.elem[i];
    return output;
}

rm_mat4f rm_scale_mat4f(float scalar, rm_mat4f m1){
    rm_mat4f output = {0};
    for(int i = 0; i < 16; i++)
        output.elem[i] = scalar*m1.elem[i];
    return output;
}

rm_mat4f rm_mult_mat4f(rm_mat4f m1, rm_mat4f m2){
    rm_mat4f output = { 0 };

    output.elem[0] = m1.elem[0]*m2.elem[0] + m1.elem[1]*m2.elem[4] + m1.elem[2]*m2.elem[8] + m1.elem[3]*m2.elem[12];
    output.elem[1] = m1.elem[0]*m2.elem[1] + m1.elem[1]*m2.elem[5] + m1.elem[2]*m2.elem[9] + m1.elem[3]*m2.elem[13];
    output.elem[2] = m1.elem[0]*m2.elem[2] + m1.elem[1]*m2.elem[6] + m1.elem[2]*m2.elem[10] + m1.elem[3]*m2.elem[14];
    output.elem[3] = m1.elem[0]*m2.elem[3] + m1.elem[1]*m2.elem[7] + m1.elem[2]*m2.elem[11] + m1.elem[3]*m2.elem[15];
    output.elem[4] = m1.elem[4]*m2.elem[0] + m1.elem[5]*m2.elem[4] + m1.elem[6]*m2.elem[8] + m1.elem[7]*m2.elem[12];
    output.elem[5] = m1.elem[4]*m2.elem[1] + m1.elem[5]*m2.elem[5] + m1.elem[6]*m2.elem[9] + m1.elem[7]*m2.elem[13];
    output.elem[6] = m1.elem[4]*m2.elem[2] + m1.elem[5]*m2.elem[6] + m1.elem[6]*m2.elem[10] + m1.elem[7]*m2.elem[14];
    output.elem[7] = m1.elem[4]*m2.elem[3] + m1.elem[5]*m2.elem[7] + m1.elem[6]*m2.elem[11] + m1.elem[7]*m2.elem[15];
    output.elem[8] = m1.elem[8]*m2.elem[0] + m1.elem[9]*m2.elem[4] + m1.elem[10]*m2.elem[8] + m1.elem[11]*m2.elem[12];
    output.elem[9] = m1.elem[8]*m2.elem[1] + m1.elem[9]*m2.elem[5] + m1.elem[10]*m2.elem[9] + m1.elem[11]*m2.elem[13];
    output.elem[10] = m1.elem[8]*m2.elem[2] + m1.elem[9]*m2.elem[6] + m1.elem[10]*m2.elem[10] + m1.elem[11]*m2.elem[14];
    output.elem[11] = m1.elem[8]*m2.elem[3] + m1.elem[9]*m2.elem[7] + m1.elem[10]*m2.elem[11] + m1.elem[11]*m2.elem[15];
    output.elem[12] = m1.elem[12]*m2.elem[0] + m1.elem[13]*m2.elem[4] + m1.elem[14]*m2.elem[8] + m1.elem[15]*m2.elem[12];
    output.elem[13] = m1.elem[12]*m2.elem[1] + m1.elem[13]*m2.elem[5] + m1.elem[14]*m2.elem[9] + m1.elem[15]*m2.elem[13];
    output.elem[14] = m1.elem[12]*m2.elem[2] + m1.elem[13]*m2.elem[6] + m1.elem[14]*m2.elem[10] + m1.elem[15]*m2.elem[14];
    output.elem[15] = m1.elem[12]*m2.elem[3] + m1.elem[13]*m2.elem[7] + m1.elem[14]*m2.elem[11] + m1.elem[15]*m2.elem[15];

    return output;
}

float rm_det_mat4f(rm_mat4f m1){
    return m1.elem[0]*m1.elem[5]*m1.elem[10]*m1.elem[15] + 
           m1.elem[0]*m1.elem[9]*m1.elem[14]*m1.elem[7] + 
           m1.elem[0]*m1.elem[13]*m1.elem[6]*m1.elem[11] - 
           m1.elem[0]*m1.elem[13]*m1.elem[10]*m1.elem[7] - 
           m1.elem[0]*m1.elem[9]*m1.elem[6]*m1.elem[15] - 
           m1.elem[0]*m1.elem[5]*m1.elem[14]*m1.elem[11] - 
           m1.elem[4]*m1.elem[1]*m1.elem[10]*m1.elem[15] - 
           m1.elem[8]*m1.elem[1]*m1.elem[14]*m1.elem[7] - 
           m1.elem[12]*m1.elem[1]*m1.elem[6]*m1.elem[11] + 
           m1.elem[12]*m1.elem[1]*m1.elem[10]*m1.elem[7] + 
           m1.elem[8]*m1.elem[1]*m1.elem[6]*m1.elem[15] + 
           m1.elem[4]*m1.elem[1]*m1.elem[14]*m1.elem[11] + 
           m1.elem[4]*m1.elem[9]*m1.elem[2]*m1.elem[15] + 
           m1.elem[8]*m1.elem[13]*m1.elem[2]*m1.elem[7] + 
           m1.elem[12]*m1.elem[5]*m1.elem[2]*m1.elem[11] - 
           m1.elem[12]*m1.elem[9]*m1.elem[2]*m1.elem[7] - 
           m1.elem[8]*m1.elem[5]*m1.elem[2]*m1.elem[15] - 
           m1.elem[4]*m1.elem[13]*m1.elem[2]*m1.elem[11] - 
           m1.elem[4]*m1.elem[9]*m1.elem[14]*m1.elem[3] - 
           m1.elem[8]*m1.elem[13]*m1.elem[6]*m1.elem[3] - 
           m1.elem[12]*m1.elem[5]*m1.elem[10]*m1.elem[3] + 
           m1.elem[12]*m1.elem[9]*m1.elem[6]*m1.elem[3] + 
           m1.elem[8]*m1.elem[5]*m1.elem[14]*m1.elem[3] + 
           m1.elem[4]*m1.elem[13]*m1.elem[10]*m1.elem[3];
}

rm_mat4f rm_transpose_mat4f(rm_mat4f m1){
    rm_mat4f output = { 0 };

    output.elem[0] = m1.elem[0];
    output.elem[1] = m1.elem[4];
    output.elem[2] = m1.elem[8];
    output.elem[3] = m1.elem[12];
    output.elem[4] = m1.elem[1];
    output.elem[5] = m1.elem[5];
    output.elem[6] = m1.elem[9];
    output.elem[7] = m1.elem[13];
    output.elem[8] = m1.elem[2];
    output.elem[9] = m1.elem[6];
    output.elem[10] = m1.elem[10];
    output.elem[11] = m1.elem[14];
    output.elem[12] = m1.elem[3];
    output.elem[13] = m1.elem[7];
    output.elem[14] = m1.elem[11];
    output.elem[15] = m1.elem[15];

    return output;
}

rm_mat4f rm_inverse_mat4f(rm_mat4f m1){
    float det = rm_det_mat4f(m1);
    if(det == 0) return (rm_mat4f){0};

    rm_mat4f t = rm_transpose_mat4f(m1);

    float det00 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[5], t.elem[6], t.elem[7],
                                               t.elem[9], t.elem[10], t.elem[11],
                                               t.elem[13], t.elem[14], t.elem[15]}});
    float det01 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[4], t.elem[6], t.elem[7],
                                               t.elem[8], t.elem[10], t.elem[11],
                                               t.elem[12], t.elem[14], t.elem[15]}});
    float det02 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[4], t.elem[5], t.elem[7],
                                               t.elem[8], t.elem[9], t.elem[11],
                                               t.elem[12], t.elem[13], t.elem[15]}});
    float det03 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[4], t.elem[5], t.elem[6],
                                               t.elem[8], t.elem[9], t.elem[10],
                                               t.elem[12], t.elem[13], t.elem[14]}});
    float det10 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[1], t.elem[2], t.elem[3],
                                               t.elem[9], t.elem[10], t.elem[11],
                                               t.elem[13], t.elem[14], t.elem[15]}});
    float det11 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[2], t.elem[3],
                                               t.elem[8], t.elem[10], t.elem[11],
                                               t.elem[12], t.elem[14], t.elem[15]}});
    float det12 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[1], t.elem[3],
                                               t.elem[8], t.elem[9], t.elem[11],
                                               t.elem[12], t.elem[13], t.elem[15]}});
    float det13 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[1], t.elem[2],
                                               t.elem[8], t.elem[9], t.elem[10],
                                               t.elem[12], t.elem[13], t.elem[14]}});
    float det20 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[1], t.elem[2], t.elem[3],
                                               t.elem[5], t.elem[6], t.elem[7],
                                               t.elem[13], t.elem[14], t.elem[15]}});
    float det21 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[2], t.elem[3],
                                               t.elem[4], t.elem[6], t.elem[7],
                                               t.elem[12], t.elem[14], t.elem[15]}});
    float det22 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[1], t.elem[3],
                                               t.elem[4], t.elem[5], t.elem[7],
                                               t.elem[12], t.elem[13], t.elem[15]}});
    float det23 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[1], t.elem[2],
                                               t.elem[4], t.elem[5], t.elem[6],
                                               t.elem[12], t.elem[13], t.elem[14]}});
    float det30 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[1], t.elem[2], t.elem[3],
                                               t.elem[5], t.elem[6], t.elem[7],
                                               t.elem[9], t.elem[10], t.elem[11]}});
    float det31 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[2], t.elem[3],
                                               t.elem[4], t.elem[6], t.elem[7],
                                               t.elem[8], t.elem[10], t.elem[11]}});
    float det32 = -rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[1], t.elem[3],
                                               t.elem[4], t.elem[5], t.elem[7],
                                               t.elem[8], t.elem[9], t.elem[11]}});
    float det33 = rm_det_mat3f((rm_mat3f){.elem = {t.elem[0], t.elem[1], t.elem[2],
                                               t.elem[4], t.elem[5], t.elem[6],
                                               t.elem[8], t.elem[9], t.elem[10]}});

    rm_mat4f adjugate = {.elem = { det00, det01, det02, det03, 
                                 det10, det11, det12, det13,
                                 det20, det21, det22, det23, 
                                 det30, det31, det32, det33}};
    
    return rm_scale_mat4f(1/det, adjugate);
}

rm_v4f rm_mult_mat4f_v4f(rm_v4f u, rm_mat4f m1){
    return (rm_v4f) { u.x * m1.elem[0] + u.y * m1.elem[4] + u.z * m1.elem[8] + u.w * m1.elem[12],
                      u.x * m1.elem[1] + u.y * m1.elem[5] + u.z * m1.elem[9] + u.w * m1.elem[13],
                      u.x * m1.elem[2] + u.y * m1.elem[6] + u.z * m1.elem[10] + u.w * m1.elem[14],
                      u.x * m1.elem[3] + u.y * m1.elem[7] + u.z * m1.elem[11] + u.w * m1.elem[15]};
}

rm_mat4f rm_identity_mat4f(void){
    rm_mat4f output = { 0 };

    output.elem[0] = 1.0;
    output.elem[5] = 1.0;
    output.elem[10] = 1.0;
    output.elem[15] = 1.0;

    return output;
}

int rm_compare_mat4f(rm_mat4f m1, rm_mat4f m2, float errorMargin){
    for(int i = 0; i < 16; i++)
        if(!rm_compare32(m1.elem[i], m2.elem[i], errorMargin)) return RMATH_FALSE;
    return RMATH_TRUE;
}

//TRIG FUNCTIONS IMPLEMENTATIONS
static double _rm_sin64(double angle){
    //McLaurin's Series with Horner's Scheme
    double y = angle*angle;

    double if3 = -1.0/6.0;
    double if5 = 1.0/120.0;
    double if7 = -1.0/5040.0;
    double if9 = 1.0/362880.0;
    double if11 = -1.0/39916800.0;

    return (angle + angle*y*(if3 + y*(if5 + y*(if7 + y*(if9 + y*if11)))));
}

static double _rm_cos64(double angle){
    //McLaurin's Series with Horner's Scheme
    double y = angle*angle;

    double if2 = -0.5;
    double if4 = 1.0/24.0;
    double if6 = -1.0/720.0;
    double if8 = 1.0/40320.0;
    double if10 = -1.0/3628800.0;
    double if12 = 1.0/479001600.0;

    return (1 + y*(if2 + y*(if4 + y*(if6 + y*(if8 + y*(if10 + y*if12))))));
}

float rm_sin32(float rad)       { return (float)rm_sin64(rad); }
float rm_cos32(float rad)       { return (float)rm_cos64(rad); }
float rm_tg32(float rad)        { return (float)rm_tg64(rad); }
float rm_cosec32(float rad)     { return (float)rm_cosec64(rad); }
float rm_sec32(float rad)       { return (float)rm_sec64(rad); }
float rm_cotg32(float rad)      { return (float)rm_cotg64(rad); }

double rm_sin64(double rad) {
    //For precision purposes I want to make sure that rad is in a good interval
    //This interval for us is going to be [-PI/4, PI/4]
    //(the bigger rad, the less precise our calculations are)
    double angle = rm_mod64(rad, 2*RMATH_PI, RMATH_TRUE);
    
    if ((RMATH_PI < angle) && (angle < 2*RMATH_PI)) angle = -(2*RMATH_PI - angle);

    if((-RMATH_PI/4 <= angle) && (angle <= RMATH_PI/4)){
        return _rm_sin64(angle);
    }else if((RMATH_PI/4 < angle) && (angle <= 3*RMATH_PI/4)){
        return _rm_cos64(angle - RMATH_PI/2);
    }else if((3*RMATH_PI/4 < angle) && (angle <= RMATH_PI)){
        return _rm_sin64(angle - RMATH_PI)*(-1);
    }else if((-3*RMATH_PI/4 < angle) && (angle < -RMATH_PI/4)){
        return _rm_cos64(RMATH_PI/2 + angle)*(-1);
    }

    return _rm_sin64(RMATH_PI + angle)*(-1);
}

double rm_cos64(double rad) {
    //For precision purposes I want to make sure that rad is in a good interval
    //This interval for us is going to be [-PI/4, PI/4]
    //(the bigger rad, the less precise our calculations are)
    double angle = rm_mod64(rad, 2*RMATH_PI, RMATH_TRUE);
    
    if ((RMATH_PI < angle) && (angle < 2*RMATH_PI)) angle = -(2*RMATH_PI - angle);

    if((-RMATH_PI/4 <= angle) && (angle <= RMATH_PI/4)){
        return _rm_cos64(angle);
    }else if((RMATH_PI/4 < angle) && (angle <= 3*RMATH_PI/4)){
        return _rm_sin64(angle - RMATH_PI/2)*(-1);
    }else if((-RMATH_PI/4 > angle) && (angle >= -3*RMATH_PI/4)){
        return _rm_sin64(angle + RMATH_PI/2);
    }else if((3*RMATH_PI/4 < angle) && (angle <= RMATH_PI)){
        return _rm_cos64(angle - RMATH_PI)*(-1);
    }
    return _rm_cos64(angle + RMATH_PI)*(-1);
}

//Unfortunetely I couldn't get it to be as precise as Sin and Cos.
//All the 4 next functions are not as precise as Sin and Cos, but we still have good results for
//      simples problems at least, We are getting error in the 6+th decimal place
//All the series expansions I tried are only good for very limited intervals of angles 
//      (Which I definetely don't want)
double rm_tg64(double rad) {
    double s = rm_sin64(rad);
    double c = rm_cos64(rad);
    if(c == 0.0f) return RMATH_INFINITY;
    return s / c;
}

double rm_cosec64(double rad) {
    double s = rm_sin64(rad);
    if(s == 0.0) return RMATH_NAN;
    return 1 / s;
}

double rm_sec64(double rad) {
    double c = rm_cos64(rad);
    if(c == 0.0) return RMATH_NAN;
    return 1.0 / c;
}

double rm_cotg64(double rad) {
    double t = rm_tg64(rad);
    if(t == 0.0) return RMATH_NAN;
    return 1.0 / t;
}

//CONVENIENT FUNCTIONS IMPLEMENTATIONS

//This is for the implementation of static_Sqrt64
typedef union _rm_union_double{
    double n;
    struct{
        unsigned long long mantissa : 52;
        unsigned long long exponent : 11;
        unsigned long long sign     : 1;
    }bits;
} _rm_union_double;

static double _rm_sqrt64(_rm_union_double x){
    //The number we want to know the square root of
    double arg = x.n;

    //We are extracting the exponent part of the floating point number (We are also getting rid of the BIAS)
    int e = x.bits.exponent - RMATH_BIAS64;
    
    //This is going to be the number (1 + f) which is the mantissa plus 1. 
    //This is useful to get a great initial guess for the Newton-Raphson algorithm. 
    double f = 0;

    double rest = 1;
    
    if(e > 0){
        f = arg / (2 << (e - 1));
        
        //if exponent is even
        if((e & 1) == 0){
            rest = 1 << (e/2);
        }else{
            rest = RMATH_SQRT_2*(1 << ((e-1)/2));
        }
    }
    else if(e < 0){
        f = arg * (2 << (-e - 1));
        
        //if exponent is even
        if((e & 1) == 0){
            rest = 1 / (1 << (-e/2));
        }else{
            rest = (RMATH_SQRT_2 / 2)*(1.0 / (1 << -((e+1)/2)));
        }
    }
    //This is for the case where the input argument is already in the correct scientific notation
    //For example, 1.5 (base 10) = 1.1 (base 2) = 1.1 * 2^(0) (e = 0) . No need to change the number at all.
    else{
        f = arg;
    }

    //Newton Raphson Algorithm
    double xk = ((f - 1) / 2) + 1;
    double xk1 = (xk + (f/xk))/2;
    double error = rm_abs64(xk1 - xk);

    while (error > RMATH_HIGH_PRECISION){
        xk = xk1;
        xk1 = (xk + (f/xk))/2;
        error = rm_abs64(xk1 - xk);
    }

    return xk1*rest;
}

int rm_compare32(float x, float y, float errorMargin)       { return (rm_abs32(x - y) < errorMargin); }
float rm_deg_to_rad32(float deg)                            { return (float)rm_deg_to_rad64(deg); }
float rm_rad_to_deg32(float rad)                            { return (float)rm_rad_to_deg64(rad); }
float rm_sqrt32(float x)                                    { return (float)rm_sqrt64(x); }
float rm_abs32(float x)                                     { return (float)rm_abs64(x); }
float rm_clamp32(float min, float max, float value)         { return (float)rm_clamp64(min, max, value); }
float rm_max32(float a, float b)                            { return (float)rm_max64(a, b); }
float rm_min32(float a, float b)                            { return (float)rm_min64(a, b); }
float rm_mod32(float f1, float f2, int positiveResult)      { return (float)rm_mod64(f1, f2, positiveResult); }
int rm_ceil32(float x)                                      { return (float)rm_ceil64(x); }
int rm_floor32(float x)                                     { return (int)rm_floor64(x); }
int rm_round32(float x)                                     { return (int)rm_round64(x); }

int rm_compare64(double x, double y, double errorMargin){
    return (rm_abs64(x - y) < errorMargin);
}

double rm_deg_to_rad64(double deg){
    return deg*RMATH_DEGREE_IN_RAD;
}

double rm_rad_to_deg64(double rad){
    return rad*(180/RMATH_PI);
}

double rm_sqrt64(double x){
    if(x == 0.0f || x == 1.0f) return x;
    if(x < 0.0f) return RMATH_NAN;

    _rm_union_double x_union = {.n = x};

    return _rm_sqrt64(x_union);
}

double rm_abs64(double x){
    if(x < 0) return -x;
    return x;
}

double rm_clamp64(double min, double max, double value){
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

double rm_max64(double a, double b){
    if(a > b) return a;
    return b;
}

double rm_min64(double a, double b){
    if(a < b) return a;
    return b;
}

double rm_mod64(double f1, double f2, int positiveResult){
    double result = f1 - (int)(f1 / f2) * f2;
    if(positiveResult && result < 0) return (f2 + result);
    return result;
}

long int rm_ceil64(double x){
    if(x < 0) return (int)x;

    int i_x = (int)x;
    if(x == (double) i_x) return i_x;
    return i_x + 1;
}

long int rm_floor64(double x){
    int n = (int)x;
    double d = (double)n;
    if (d == x || x >= 0) return (int)d;
    return (int)d - 1;
}

long int rm_round64(double x){
    if(x < 0) return (int) (x - 0.5f);
    return (int) (x + 0.5f);
}

int rm_sign(double x){
    if(x < 0) return -1;
    return 1;
}

//RANDOM IMPLEMENTATIONS

static void _rm_set_rng_seed(void){
    random_seed = (unsigned int)_rm_get_platform_time_for_rng();
    seeded = RMATH_TRUE;
}

static unsigned int _rm_rng_xor_shift(void){
    unsigned int temp = random_seed;
    temp ^= temp << 13;
    temp ^= temp >> 17;
    temp ^= temp << 5;
    random_seed = temp;
    return random_seed;
}

unsigned int rm_random_u32(void){
    if(seeded == RMATH_FALSE) _rm_set_rng_seed();
    return _rm_rng_xor_shift();
}

unsigned int rm_random_u32_in_interval(unsigned int min, unsigned int max){
    return (rm_random_u32() % (max - min + 1)) + min;
}

int rm_random_bool(void){
    return (rm_random_u32() & 1) == 0;
}

int rm_random_sign(void){
    return rm_random_bool() == RMATH_FALSE ? 1 : -1;
}

float rm_random_f(void){
    return (float)rm_random_u32() / (float)RMATH_U32_MAX;
}

rm_v2f rm_random_v2f(void){
    return (rm_v2f){rm_random_sign() * rm_random_f(), 
                    rm_random_sign() * rm_random_f()};
}

rm_v3f rm_random_v3f(void){
    return (rm_v3f){rm_random_sign() * rm_random_f(),
                    rm_random_sign() * rm_random_f(),
                    rm_random_sign() * rm_random_f()};
}

rm_v4f rm_random_v4f(void){
    return (rm_v4f){rm_random_sign() * rm_random_f(),
                    rm_random_sign() * rm_random_f(),
                    rm_random_sign() * rm_random_f(),
                    rm_random_sign() * rm_random_f()};
}

rm_mat2f rm_random_mat2f(void){
    return (rm_mat2f){.elem = {rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f()}};
}

rm_mat3f rm_random_mat3f(void){
    return (rm_mat3f){.elem = {rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f(),
                             rm_random_sign() * rm_random_f()}};
}

rm_mat4f rm_random_mat4f(void){
    return (rm_mat4f){.elem = {rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f(),
                               rm_random_sign() * rm_random_f()}};
}

//GRAPHICS IMPLEMENTATIONS

rm_mat3f rm_translation_2D(rm_v2f posDelta){
    rm_mat3f output = rm_identity_mat3f();

    output.elem[2] = posDelta.x;
    output.elem[5] = posDelta.y;

    return output;
}

rm_mat3f rm_scaling_2D_set_origin(rm_v2f scale, rm_v2f origin){
    rm_mat3f output = rm_identity_mat3f();

    output.elem[0] = scale.x;
    output.elem[2] = origin.x - origin.x*scale.x;
    output.elem[4] = scale.y;
    output.elem[5] = origin.y - origin.y*scale.y;

    return output;
}

rm_mat3f rm_scaling_2D(rm_v2f scale){
    return rm_scaling_2D_set_origin(scale, (rm_v2f){ 0, 0 });
}

rm_mat3f rm_rotation_2D_around_point(float rad, rm_v2f pos){
    rm_mat3f output = rm_identity_mat3f();

    float s = rm_sin32(rad);
    float c = rm_cos32(rad);

    output.elem[0] = c;
    output.elem[1] = -s;
    output.elem[2] = (pos.y * s) - (pos.x * c) + pos.x;
    output.elem[3] = s;
    output.elem[4] = c;
    output.elem[5] = -(pos.x * s) - (pos.y * c) + pos.y;

    return output;
}

rm_mat3f rm_rotation_2D(float rad){
    return rm_rotation_2D_around_point(rad, (rm_v2f){ 0, 0 });
}


rm_mat4f rm_translation_3D(rm_v3f posDelta){
    rm_mat4f output = rm_identity_mat4f();

    output.elem[3] = posDelta.x;
    output.elem[7] = posDelta.y;
    output.elem[11] = posDelta.z;

    return output;
}

rm_mat4f rm_scaling_3D(rm_v3f scale){
    rm_mat4f output = rm_identity_mat4f();

    output.elem[0] = scale.x;
    output.elem[5] = scale.y;
    output.elem[10] = scale.z;

    return output;
}

rm_mat4f rm_euler_rotationX_3D(float rad){
    rm_mat4f output = rm_identity_mat4f();

    float s = rm_sin32(rad);
    float c = rm_cos32(rad);

    output.elem[5] = c;
    output.elem[6] = -s;
    output.elem[9] = s;
    output.elem[10] = c;

    return output;
}

rm_mat4f rm_euler_rotationY_3D(float rad){
    rm_mat4f output = rm_identity_mat4f();

    float s = rm_sin32(rad);
    float c = rm_cos32(rad);

    output.elem[0] = c;
    output.elem[2] = s;
    output.elem[8] = -s;
    output.elem[10] = c;    

    return output;
}

rm_mat4f rm_euler_rotationZ_3D(float rad){
    rm_mat4f output = rm_identity_mat4f();

    float s = rm_sin32(rad);
    float c = rm_cos32(rad);

    output.elem[0] = c;
    output.elem[1] = -s;
    output.elem[4] = s;
    output.elem[5] = c;

    return output;
}

rm_mat4f rm_euler_rotationXYZ_3D(rm_v3f rads){
    rm_mat4f output = { 0 };

    rm_mat4f rotationMatrixInX = rm_euler_rotationX_3D(rads.x);
    rm_mat4f rotationMatrixInY = rm_euler_rotationY_3D(rads.y);
    rm_mat4f rotationMatrixInZ = rm_euler_rotationZ_3D(rads.z);
    
    output = rm_mult_mat4f(rotationMatrixInX, rotationMatrixInY);
    output = rm_mult_mat4f(output, rotationMatrixInZ);

    return output;
}

rm_mat4f rm_parallel_projection_3D(float l, float r, float t, float b, float f, float n){
    rm_mat4f output = rm_identity_mat4f();

    output.elem[0] = 2.0 / (r - l);
    output.elem[5] = 2.0 / (t - b);
    output.elem[10] = 2.0 / (f - n);

    output.elem[12] = -(r + l) / (r - l);
    output.elem[13] = -(t + b) / (t - b);
    output.elem[14] = -(f + n) / (f - n);

    return output;
}

rm_mat4f rm_perspective_projection_3D(float fovY, float aspectRatio, float f, float n){
    rm_mat4f output = { 0 };

    float tg = rm_tg32(0.5f*fovY);

    output.elem[0] = (1 / tg) / aspectRatio;
    output.elem[5] = (1 / tg);
    output.elem[10] = f / (f - n);
    output.elem[11] = -(f * n) / (f - n);
    output.elem[14] = 1.0;

    return output;
}

// GEOMETRY IMPLEMENTATIONS

float rm_distance_points2D(rm_point2D p, rm_point2D q){
    return rm_sqrt32((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y));
}

float rm_distance_points3D(rm_point3D p, rm_point3D q){
    return rm_sqrt32((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y) + (p.z - q.z)*(p.z - q.z));
}

float rm_distance_point_line2D(rm_point2D p, rm_line2D line){
    rm_v2f vectorAux = rm_sub_v2f(p, line.point);
    rm_v2f lineNormal = {line.direction.y, -line.direction.x};
    return rm_abs32(rm_dot_v2f(vectorAux, lineNormal) / rm_mag_v2f(lineNormal));
}

float rm_distance_lines2D(rm_line2D line1, rm_line2D line2){
    if(rm_intersecting_lines2D(line1, line2)){
        return 0;
    }
    return rm_distance_point_line2D(line1.point, line2);
}

float rm_distance_point_line3D(rm_point3D p, rm_line3D line){
    rm_v3f aux = rm_sub_v3f(p, line.point);
    return rm_mag_v3f(rm_cross_v3f(aux, line.direction)) / rm_mag_v3f(line.direction);
}

float rm_distance_lines3D(rm_line3D line1, rm_line3D line2){
    if(rm_intersecting_lines3D(line1, line2)){
        return 0;
    }
    if(rm_parallel_lines3D(line1, line2)){
        return rm_distance_point_line3D(line1.point, line2);
    }
    
    rm_v3f pointSub = rm_sub_v3f(line1.point, line2.point);
    return rm_abs32(rm_mixed_product_v3f(line1.direction, line2.direction, pointSub)) / rm_mag_v3f(rm_cross_v3f(line1.direction, line2.direction));
}

float rm_distance_point_plane(rm_point3D p, rm_plane pl){
    rm_v3f pointSub = rm_sub_v3f(p, pl.point);
    return rm_abs32(rm_dot_v3f(pointSub, pl.normal)) / rm_mag_v3f(pl.normal);
}

float rm_distance_line_plane(rm_line3D line, rm_plane pl){
    //Check if the line is parallel to the plane
    // Use Compare32() ?
    if(rm_compare32(rm_dot_v3f(line.direction, pl.normal), 0.0f, RMATH_LOW_PRECISION)){
        return rm_distance_point_plane(line.point, pl);
    }
    //Here it means that the line is intersecting the plane in one point, since it's not parallel
    return 0;
}

float rm_distance_planes(rm_plane pl1, rm_plane pl2){
    rm_line3D normalLine1 = {.direction = pl1.normal, .point = {0}};
    rm_line3D normalLine2 = {.direction = pl2.normal, .point = {0}};
    if(rm_parallel_lines3D(normalLine1, normalLine2) == RMATH_TRUE){
        return rm_distance_point_plane(pl1.point, pl2);
    }
    //Here it means that planes are intersecting, since they're not parallel
    return 0;
}

int rm_parallel_lines2D(rm_line2D l1, rm_line2D l2){
    float l1dx = l1.direction.x;
    float l1dy = l1.direction.y;
    float l2dx = l2.direction.x;
    float l2dy = l2.direction.y;
    
    if((((l1dx == 0) && (l2dx == 0)) && 
        ((l1dy != 0) && (l2dy != 0))) ||
       (((l1dy == 0) && (l2dy == 0)) && 
        ((l1dx != 0) && (l2dx != 0)))){
        return RMATH_TRUE;
    }

    if(l2dx != 0.0f && l2dy != 0.0f && rm_compare32((l1dx / l2dx), (l1dy / l2dy), RMATH_LOW_PRECISION)){
        return RMATH_TRUE;
    }
    
    return RMATH_FALSE;
}

int rm_parallel_lines3D(rm_line3D l1, rm_line3D l2){
    //The arbitrary points don't matter, since we only need the direction vectors
    if(l1.direction.x == l2.direction.x){
        rm_line2D aux1 = {.point = (rm_point2D){0}, .direction = (rm_v2f){l1.direction.y, l1.direction.z}};
        rm_line2D aux2 = {.point = (rm_point2D){0}, .direction = (rm_v2f){l2.direction.y, l2.direction.z}};
        return rm_parallel_lines2D(aux1, aux2);
    }
    if(l1.direction.y == l2.direction.y){
        rm_line2D aux1 = {.point = (rm_point2D){0}, .direction = (rm_v2f){l1.direction.x, l1.direction.z}};
        rm_line2D aux2 = {.point = (rm_point2D){0}, .direction = (rm_v2f){l2.direction.x, l2.direction.z}};
        return rm_parallel_lines2D(aux1, aux2);
    }
    if(l1.direction.z == l2.direction.z){
        rm_line2D aux1 = {.point = (rm_point2D){0}, .direction = (rm_v2f){l1.direction.x, l1.direction.y}};
        rm_line2D aux2 = {.point = (rm_point2D){0}, .direction = (rm_v2f){l2.direction.x, l2.direction.y}};
        return rm_parallel_lines2D(aux1, aux2);
    }

    // transitivity
    if(l2.direction.x != 0.0f && l2.direction.y != 0.0f && l2.direction.z != 0.0f){
        float xRatio = l1.direction.x / l2.direction.x;
        float yRatio = l1.direction.y / l2.direction.y;
        float zRatio = l1.direction.z / l2.direction.z;
        if(rm_compare32(xRatio, yRatio, RMATH_LOW_PRECISION) && 
            rm_compare32(yRatio, zRatio, RMATH_LOW_PRECISION)){
            return RMATH_TRUE;
        }
    }

    return RMATH_FALSE;
}

int rm_intersecting_lines2D(rm_line2D l1, rm_line2D l2){
    return !rm_parallel_lines2D(l1, l2);
}

int rm_intersecting_lines3D(rm_line3D l1, rm_line3D l2){
    rm_v3f directionCross = rm_cross_v3f(l1.direction, l2.direction);
    rm_v3f directionSub = rm_sub_v3f(l1.point, l2.point);
    return !rm_parallel_lines3D(l1, l2) && rm_compare32(rm_dot_v3f(directionCross, directionSub), 0.0f, RMATH_LOW_PRECISION);
}

int rm_skew_lines(rm_line3D l1, rm_line3D l2){
    return !rm_parallel_lines3D(l1, l2) && !rm_intersecting_lines3D(l1, l2);
}

int rm_collision_AABB2D(rm_AABB2D r1, rm_AABB2D r2){
    return((r1.min.x < r2.max.x) &&
           (r1.max.x > r2.min.x) &&
           (r1.min.y < r2.max.y) &&
           (r1.max.y > r2.min.y));
}

int rm_collision_point_AABB2D(rm_point2D p, rm_AABB2D r){
    return (p.x >= r.min.x && p.x <= r.max.x) &&
           (p.y >= r.min.y && p.y <= r.max.y);
}

int rm_collision_point_circle(rm_point2D p, rm_circle s){
    return rm_distance_points2D(p, s.center) < s.radius;
}

int rm_collision_circles(rm_circle s1, rm_circle s2){
    return rm_distance_points2D(s1.center, s2.center) < (s1.radius + s2.radius);
}

int rm_collision_AABB3D(rm_AABB3D r1, rm_AABB3D r2){
    return( (r1.max.x > r2.min.x) &&
            (r1.min.x < r2.max.x) &&
            (r1.max.y > r2.min.y) &&
            (r1.min.y < r2.max.y) &&
            (r1.max.z > r2.min.z) &&
            (r1.min.z < r2.max.z));
}

int rm_collision_point_AABB3D(rm_point3D p, rm_AABB3D r){
    return (p.x >= r.min.x && p.x <= r.max.x) &&
           (p.y >= r.min.y && p.y <= r.max.y) &&
           (p.z >= r.min.z && p.z <= r.max.z);
}

int rm_collision_point_sphere(rm_point3D p, rm_sphere s){
    return rm_distance_points3D(p, s.center) < s.radius;
}

int rm_collision_spheres(rm_sphere s1, rm_sphere s2){
    return rm_distance_points3D(s1.center, s2.center) < (s1.radius + s2.radius);
}

float rm_area_triangle2D(rm_triangle2D triangle){
    rm_mat2f output = { 0 };

    output.elem[0] = triangle.a.x - triangle.b.x;
    output.elem[1] = triangle.a.x - triangle.c.x;
    output.elem[2] = triangle.a.y - triangle.b.y;
    output.elem[3] = triangle.a.y - triangle.c.y;

    return rm_abs32(0.5f*rm_det_mat2f(output));
}

float rm_area_circle(rm_circle sphere){
    return rm_abs32(RMATH_PI*sphere.radius*sphere.radius);
}

float rm_area_AABB2D(rm_AABB2D aabb){
    return rm_abs32((aabb.max.x - aabb.min.x)) * rm_abs32((aabb.max.y - aabb.min.y));
}

float rm_area_Quad2D(rm_quad2D quad){
    rm_mat2f output = { 0 };

    output.elem[0] = quad.a.x - quad.c.x;
    output.elem[1] = quad.a.y - quad.c.y;
    output.elem[2] = quad.b.x - quad.d.x;
    output.elem[3] = quad.b.y - quad.d.y;

    return rm_abs32(0.5f*rm_det_mat2f(output));
}

float rm_perimeter_circle(rm_circle circle){
    return rm_abs32(circle.radius*2*RMATH_PI);
}

float rm_perimeter_AABB2D(rm_AABB2D aabb){
    return rm_abs32(2 * (aabb.max.x - aabb.min.x)) + rm_abs32(2 * (aabb.max.y - aabb.min.y));
}

float rm_perimeter_quad2D(rm_quad2D quad){
    float d1 = rm_distance_points2D(quad.a, quad.b);
    float d2 = rm_distance_points2D(quad.b, quad.c);
    float d3 = rm_distance_points2D(quad.c, quad.d);
    float d4 = rm_distance_points2D(quad.d, quad.a);
    return d1 + d2 + d3 + d4;
}

float rm_perimeter_triangle2D(rm_triangle2D triangle){
    float d1 = rm_distance_points2D(triangle.a, triangle.b);
    float d2 = rm_distance_points2D(triangle.b, triangle.c);
    float d3 = rm_distance_points2D(triangle.c, triangle.a);
    return d1 + d2 + d3;
}

float rm_area_triangle3D(rm_triangle3D triangle){
    rm_v3f u = rm_sub_v3f(triangle.b, triangle.a);
    rm_v3f v = rm_sub_v3f(triangle.c, triangle.a);
    rm_v3f c = rm_cross_v3f(u, v);
    float n = rm_mag_v3f(c);
    return 0.5f * n;
}

float rm_area_quad3D(rm_quad3D quad){
    rm_triangle3D t1 = (rm_triangle3D){quad.a, quad.b, quad.c};
    rm_triangle3D t2 = (rm_triangle3D){quad.a, quad.d, quad.c};
    float areaTriangle1 = rm_area_triangle3D(t1);
    float areaTriangle2 = rm_area_triangle3D(t2);
    return areaTriangle1 + areaTriangle2;
}

float rm_volume_sphere(rm_sphere sphere){
    float r = sphere.radius;
    return rm_abs32((4.0f/3.0f)*RMATH_PI*(r*r*r));
}

float rm_volume_AABB3D(rm_AABB3D aabb){
    return rm_abs32((aabb.max.x - aabb.min.x)) * rm_abs32((aabb.max.y - aabb.min.y)) * rm_abs32((aabb.max.z - aabb.min.z));
}

float rm_perimeter_triangle3D(rm_triangle3D triangle){
    float d1 = rm_distance_points3D(triangle.a, triangle.b);
    float d2 = rm_distance_points3D(triangle.b, triangle.c);
    float d3 = rm_distance_points3D(triangle.c, triangle.a);
    return d1 + d2 + d3;
}

float rm_surface_area_sphere(rm_sphere sphere){
    return 4.0f*RMATH_PI*sphere.radius*sphere.radius;
}

float rm_surface_area_AABB3D(rm_AABB3D aabb){
    float xDelta = aabb.max.x - aabb.min.x;
    float yDelta = aabb.max.y - aabb.min.y;
    float zDelta = aabb.max.z - aabb.min.z;
    return 2.0f * (xDelta*yDelta + xDelta*zDelta + yDelta*zDelta);
}
