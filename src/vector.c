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

vec2_t vec2_mul(vec2_t vector, float factor) {
    vec2_t result = {
            .x = vector.x * factor,
            .y = vector.y * factor
    };

    return result;
}

vec2_t vec2_div(vec2_t vector, float factor) {
    vec2_t result = {
            .x = vector.x / factor,
            .y = vector.y / factor
    };

    return result;
}

float vec2_dot(vec2_t vector_a, vec2_t vector_b) {
    return vector_a.x * vector_b.x + vector_a.y * vector_b.y;
}

void vec2_normalize(vec2_t *vector) {
    float length = sqrt(vector->x * vector->x + vector->y * vector->y);

    vector->x /= length;
    vector->y /= length;
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

vec3_t vec3_mul(vec3_t vector, float factor) {
    vec3_t result = {
            .x = vector.x * factor,
            .y = vector.y * factor,
            .z = vector.z * factor
    };

    return result;
}

vec3_t vec3_div(vec3_t vector, float factor) {
    vec3_t result = {
            .x = vector.x / factor,
            .y = vector.y / factor,
            .z = vector.z / factor
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
            .x = vector.x * cos(angle) + vector.z * sin(angle),
            .y = vector.y,
            .z = -vector.x * sin(angle) + vector.z * cos(angle)
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

vec3_t vec3_cross(vec3_t vector_a, vec3_t vector_b) {
    vec3_t result = {
            .x = vector_a.y * vector_b.z - vector_a.z * vector_b.y,
            .y = vector_a.z * vector_b.x - vector_a.x * vector_b.z,
            .z = vector_a.x * vector_b.y - vector_a.y * vector_b.x
    };

    return result;
}

float vec3_dot(vec3_t vector_a, vec3_t vector_b) {
    return vector_a.x * vector_b.x + vector_a.y * vector_b.y + vector_a.z * vector_b.z;
}

void vec3_normalize(vec3_t *vector) {
    float length = sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);

    vector->x /= length;
    vector->y /= length;
    vector->z /= length;
}


vec4_t vec4_from_vec3(vec3_t v) {
    vec4_t result = {v.x, v.y, v.z, 1.0};

    return result;
}

vec3_t vec3_from_vec4(vec4_t v) {
    vec3_t result = {v.x, v.y, v.z};

    return result;
}

vec2_t vec2_from_vec4(vec4_t v) {
    vec2_t result = {v.x, v.y};

    return result;
}
