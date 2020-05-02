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

void load_obj_file_data(char *filename) {
    FILE *file;
    file = fopen(filename, "r");

    int buffer_length = 255;
    char buffer[buffer_length];
    while (fgets(buffer, buffer_length, file)) {
        printf("-> read new line: %s", buffer);

        if (buffer[0] == 'v' && buffer[1] == ' ') { // found string - vertex
            printf("found 'v ' - processing [vertex]: ");

            char *token;
            token = strtok(buffer, " ");

            vec3_t vertex;
            for (int i = 0; i < 3; i++) {
                token = strtok(NULL, " ");
                float float_value = atof(token);

                if (i == 0) {
                    vertex.x = float_value;
                }

                if (i == 1) {
                    vertex.y = float_value;
                }

                if (i == 2) {
                    vertex.z = float_value;
                }

                printf("float: %.6f ", float_value);
            }

            array_push(mesh.vertices, vertex);
            printf("\n");
        }

        if (buffer[0] == 'f' && buffer[1] == ' ') { // found string - face vertex indices
            printf("found 'f ' - processing [face vertex indices]: ");

            char *indices_token;
            indices_token = strtok(buffer, " ");

            char *indices_array[3];
            for (int i = 0; i < 3; i++) {
                indices_token = strtok(NULL, " ");
                printf("indices token: %s ", indices_token);

                indices_array[i] = indices_token;
            }

            face_t face;
            for (int i = 0; i < 3; i++) {
                char *token = strtok(indices_array[i], "/");
                int vertex_index = atoi(token);
                printf("face vertex index: %d ", vertex_index);

                if (i == 0) {
                    face.a = vertex_index;
                }

                if (i == 1) {
                    face.b = vertex_index;
                }

                if (i == 2) {
                    face.c = vertex_index;
                }
            }

            array_push(mesh.faces, face);
            printf("\n");
        }
    }

    fclose(file);
}
