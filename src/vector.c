#include <math.h>
#include "vector.h"


float vec2_length(vec2_t vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

vec2_t vec2_add(vec2_t vector_a, vec2_t vector_b) {
    vec2_t result = {
            .x = vector_a.x + vector_b.x,
            .y = vector_a.y + vector_b.y
    };

    return result;
}

vec2_t vec2_sub(vec2_t vector_a, vec2_t vector_b) {
    vec2_t result = {
            .x = vector_a.x - vector_b.x,
            .y = vector_a.y - vector_b.y
    };

    return result;
}


float vec3_length(vec3_t vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

vec3_t vec3_add(vec3_t vector_a, vec3_t vector_b) {
    vec3_t result = {
            .x = vector_a.x + vector_b.x,
            .y = vector_a.y + vector_b.y,
            .z = vector_a.z + vector_b.z
    };

    return result;
}

vec3_t vec3_sub(vec3_t vector_a, vec3_t vector_b) {
    vec3_t result = {
            .x = vector_a.x - vector_b.x,
            .y = vector_a.y - vector_b.y,
            .z = vector_a.z - vector_b.z
    };

    return result;
}


vec3_t vec3_rotate_x(vec3_t vector, float angle) {
    vec3_t rotated_vector = {
            .x = vector.x,
            .y = vector.y * cos(angle) - vector.z * sin(angle),
            .z = vector.y * sin(angle) + vector.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t vector, float angle) {
    vec3_t rotated_vector = {
            .x = vector.x * cos(angle) - vector.z * sin(angle),
            .y = vector.y,
            .z = vector.x * sin(angle) + vector.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t vector, float angle) {
    vec3_t rotated_vector = {
            .x = vector.x * cos(angle) - vector.y * sin(angle),
            .y = vector.x * sin(angle) + vector.y * cos(angle),
            .z = vector.z
    };

    return rotated_vector;
}
