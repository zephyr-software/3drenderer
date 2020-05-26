#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#include <stdio.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"


bool is_running = false;

triangle_t *triangles_to_render = NULL;

float fov_factor = 512;

vec3_t camera_position = {0, 0, 0};
//vec3_t light_position = {0, 0, 0};

vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};
mat4_t proj_matrix;

int previous_frame_time = 0;

void setup(void) {
    // Initialize render mode and triangle culling method
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    // allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * window_width * window_height);

    // creating a sdl texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height
    );

    // Initialize the perspective projection matrix
    float fov = M_PI / 3.0; // the same as 180/3, or 60deg
    float aspect = (float) window_height / (float) window_width;
    float znear = 0.1;
    float zfar = 100.0;
    proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

    // Loads the cube values in the mesh data structure
//    load_cube_mesh_data();
    load_obj_file_data("./assets/f22r.obj");
}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;

            if (event.key.keysym.sym == SDLK_1)
                render_method = RENDER_WIRE_VERTEX;
            if (event.key.keysym.sym == SDLK_2)
                render_method = RENDER_WIRE;
            if (event.key.keysym.sym == SDLK_3)
                render_method = RENDER_FILL_TRIANGLE;
            if (event.key.keysym.sym == SDLK_4)
                render_method = RENDER_FILL_TRIANGLE_WIRE;

            if (event.key.keysym.sym == SDLK_c)
                cull_method = CULL_BACKFACE;
            if (event.key.keysym.sym == SDLK_d)
                cull_method = CULL_NONE;
            break;
    }
}

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor) {
    if (percentage_factor < 0) percentage_factor = 0;
    if (percentage_factor > 1) percentage_factor = 1;

    uint32_t a = (original_color & 0xFF000000);
    uint32_t r = (original_color & 0x00FF0000) * percentage_factor;
    uint32_t g = (original_color & 0x0000FF00) * percentage_factor;
    uint32_t b = (original_color & 0x000000FF) * percentage_factor;

    uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

    return new_color;
}

void update(void) {
    // Wait some time until the reach the target frame time in milliseconds
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    // Only delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    mesh.rotation.x = 0.5;
    mesh.rotation.y += 0.005;
    mesh.rotation.z = 0;

//    mesh.scale.x += 0.001;
//    mesh.scale.y += 0.001;
//    mesh.scale.z += 0.001;

//    mesh.translation.x += 0.01;
//    mesh.translation.y += 0.01;
    mesh.translation.z = 4.0;

    // Create scale, rotation, and translation matrices that will be used to multiply the mesh vertices
    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

    triangles_to_render = NULL; // Initialize the array of triangles to render
    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec4_t transformed_vertices[3];
        // Loop all three vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Create a World Matrix combining scale, rotation, and translation matrices
            mat4_t world_matrix = mat4_identity();

            // Order matters: First scale, then rotate, then translate. [T]*[R]*[S]*v
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            // Multiply the world matrix by the original vector
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            // Save transformed vertex in the array of transformed vertices
            transformed_vertices[j] = transformed_vertex;
        }

        uint32_t flat_color = 0xFF777777;
        // Backface culling test to see if the current face should be projected
        if (cull_method == CULL_BACKFACE) {
            // Check backface culling
            vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*   A   */
            vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
            vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C---B */

            // Get the vector subtraction of B-A and C-A
            vec3_t vector_ab = vec3_sub(vector_b, vector_a);
            vec3_t vector_ac = vec3_sub(vector_c, vector_a);

            vec3_normalize(&vector_ab); // normalize the vector
            vec3_normalize(&vector_ac); // normalize the vector

            // Compute the face normal (using cross product to find perpendicular)
            vec3_t normal = vec3_cross(vector_ab, vector_ac);
            vec3_normalize(&normal); // normalize the face normal vector

            // Find the vector between a point in the triangle and the camera origin
            vec3_t camera_ray = vec3_sub(camera_position, vector_a);

            // Calculate how aligned the camera ray is with the face normal (using dot product)
            float dot_normal_camera = vec3_dot(normal, camera_ray);

            // Bypass the triangles that are looking away from the camera
            if (dot_normal_camera < 0) {
                continue;
            }

            flat_color = light_apply_intensity(0xFFFFFFFF, dot_normal_camera / mesh.translation.z);
        }

        vec4_t projected_points[3];

        // Loop all three vertices to perform projection
        for (int j = 0; j < 3; j++) {
            // Project the current vertex
            projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

            // Scale into the view
            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);

            // Translate the projected points to the middle of the screen
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);
        }

        // Calculate the average depth for each face based on the vertices after transformation
        float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

        triangle_t projected_triangle = {
                .points = {
                        {projected_points[0].x, projected_points[0].y},
                        {projected_points[1].x, projected_points[1].y},
                        {projected_points[2].x, projected_points[2].y},
                },
//                .color = mesh_face.color,
                .color = flat_color,
                .avg_depth = avg_depth
        };

//        fprintf(stdout, "flat_color %.6f\n", flat_color);

        // Save the projected triangle in the array of triangles to render
        array_push(triangles_to_render, projected_triangle);
    }

    // Sort the triangles to render by their avg_depth
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++) {
        for (int j = i; j < num_triangles; j++) {
            if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
                // Swap the triangles positions in the array
                triangle_t temp = triangles_to_render[i];
                triangles_to_render[i] = triangles_to_render[j];
                triangles_to_render[j] = temp;
            }
        }
    }
}

void render(void) {
    clear_color_buffer(0xFF000000);
    draw_grid(16, 0xFF003300);
    draw_center(0xFF770000);

    // Loop all projected triangles and render them
    int num_triangles = array_length(triangles_to_render);
    // Loop all projected triangles and render them
    for (int i = 0; i < num_triangles; i++) {
        triangle_t triangle = triangles_to_render[i];

        // Draw filled triangle
        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
                                 triangle.points[1].x, triangle.points[1].y,
                                 triangle.points[2].x, triangle.points[2].y,
                                 triangle.color);
        }

        // Draw triangle wireframe
        if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX ||
            render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_triangle(triangle.points[0].x, triangle.points[0].y,
                          triangle.points[1].x, triangle.points[1].y,
                          triangle.points[2].x, triangle.points[2].y,
                          0xFFFFFF00);
        }

        // Draw triangle vertex points
        if (render_method == RENDER_WIRE_VERTEX) {
            draw_rect(triangle.points[0].x - 2, triangle.points[0].y - 2, 4, 4, 0xFFFF00FF);
            draw_rect(triangle.points[1].x - 2, triangle.points[1].y - 2, 4, 4, 0xFFFF00FF);
            draw_rect(triangle.points[2].x - 2, triangle.points[2].y - 2, 4, 4, 0xFFFF00FF);
        }
    }
    // Clear the array of triangles to render every frame loop
    array_free(triangles_to_render);

    render_color_buffer();
    SDL_RenderPresent(renderer);
}

void free_resources(void) {
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}

int main(void) {
    is_running = intialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();

    return 0;
}
