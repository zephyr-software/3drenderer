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

vec2_t vec2_add(vec2_t vector_a, vec2_t vector_b);

vec2_t vec2_sub(vec2_t vector_a, vec2_t vector_b);

vec2_t vec2_mul(vec2_t vector, float factor);

vec2_t vec2_div(vec2_t vector, float factor);


float vec3_length(vec3_t vector);

vec3_t vec3_add(vec3_t vector_a, vec3_t vector_b);

vec3_t vec3_sub(vec3_t vector_a, vec3_t vector_b);

vec3_t vec3_mul(vec3_t vector, float factor);

vec3_t vec3_div(vec3_t vector, float factor);


vec3_t vec3_rotate_x(vec3_t vector, float angle);

vec3_t vec3_rotate_y(vec3_t vector, float angle);

vec3_t vec3_rotate_z(vec3_t vector, float angle);


vec3_t vec3_cross(vec3_t vector_a, vec3_t vector_b);


#endif
