#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

float vec2_length(vec2_t vector);

float vec3_length(vec3_t vector);

vec3_t vec3_rotate_x(vec3_t vector, float angle);

vec3_t vec3_rotate_y(vec3_t vector, float angle);

vec3_t vec3_rotate_z(vec3_t vector, float angle);

#endif
