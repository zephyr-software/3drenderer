#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);

mat4_t mat4_make_scale(float scale_x, float scaly_y, float scale_z);

mat4_t mat4_make_translation(float translate_x, float translate_y, float translate_z);

mat4_t mat4_make_rotation_x(float angle);

mat4_t mat4_make_rotation_y(float angle);

mat4_t mat4_make_rotation_z(float angle);

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);

mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up);

#endif
