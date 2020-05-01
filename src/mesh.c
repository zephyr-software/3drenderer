#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "mesh.h"


vec3_t cube_vertices[N_CUBE_VERTICES] = {
        {.x = -1, .y = -1, .z = -1}, // 1
        {.x = -1, .y =  1, .z = -1}, // 2
        {.x =  1, .y =  1, .z = -1}, // 3
        {.x =  1, .y = -1, .z = -1}, // 4
        {.x =  1, .y =  1, .z =  1}, // 5
        {.x =  1, .y = -1, .z =  1}, // 6
        {.x = -1, .y =  1, .z =  1}, // 7
        {.x = -1, .y = -1, .z =  1}  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
        // front
        {.a = 1, .b = 2, .c = 3},
        {.a = 1, .b = 3, .c = 4},
        // right
        {.a = 4, .b = 3, .c = 5},
        {.a = 4, .b = 5, .c = 6},
        // back
        {.a = 6, .b = 5, .c = 7},
        {.a = 6, .b = 7, .c = 8},
        // left
        {.a = 8, .b = 7, .c = 2},
        {.a = 8, .b = 2, .c = 1},
        // top
        {.a = 2, .b = 7, .c = 5},
        {.a = 2, .b = 5, .c = 3},
        // bottom
        {.a = 6, .b = 8, .c = 1},
        {.a = 6, .b = 1, .c = 4}
};

mesh_t mesh = {
        .vertices = NULL,
        .faces = NULL,
        .rotation = {0, 0, 0}
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }

    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void load_obj_file_mesh_data(char *filename) {
    FILE *file;
    file = fopen(filename, "r");

    int bufferLength = 255;
    char buffer[bufferLength];
    while (fgets(buffer, bufferLength, file)) {
        printf("--> new line: %s", buffer);

        if (buffer[0] == 'v' && buffer[1] == ' ') { // found string with vertex signature
            printf("found v - processing: ");

            vec3_t obj_vertex;
            char *p;
            p = strtok(buffer, " ");
            if (p) {
                for (int i = 0; i < 3; i++) {
                    p = strtok(NULL, " ");
                    float tmpFloat = atof(p);

                    if (i == 0) {
                        obj_vertex.x = tmpFloat;
                    }

                    if (i == 1) {
                        obj_vertex.y = tmpFloat;
                    }

                    if (i == 2) {
                        obj_vertex.z = tmpFloat;
                    }

                    printf("float: %.6f ", tmpFloat);
                }

                array_push(mesh.vertices, obj_vertex);
                printf("\n");
            }
        }

        if (buffer[0] == 'f' && buffer[1] == ' ') { // found string with vertex signature
            printf("found f - processing: ");


            char *string_array[3];

            char *p;
            p = strtok(buffer, " ");
            if (p) {
                for (int i = 0; i < 3; i++) {
                    p = strtok(NULL, " ");
                    printf("string: %s ", p);

                    string_array[i] = p;
                }

                face_t obj_file_face;

                printf("\n");
                for (int i = 0; i < 3; i++) {
                    char *p2 = strtok(string_array[i], "/");
                    int num = atoi(p2);
                    printf("num: %d ", num);

                    if (i == 0) {
                        obj_file_face.a = num;
                    }

                    if (i == 1) {
                        obj_file_face.b = num;
                    }

                    if (i == 2) {
                        obj_file_face.c = num;
                    }
                }

                array_push(mesh.faces, obj_file_face);
                printf("\n");
            }
        }
    }

    fclose(file);
}
