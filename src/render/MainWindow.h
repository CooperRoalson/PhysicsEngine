#ifndef PHYSICSENGINE_MAINWINDOW_H
#define PHYSICSENGINE_MAINWINDOW_H

#define GL_GLEXT_PROTOTYPES

#include <SDL.h>
#include <SDL_opengl.h>

#include "../core/Vector3.h"
#include "../physics/PhysicsWorld.h"

/*
 * Include basic shader source code
 */
#include "shaders.h"

/*
 * Define maximum OpenGL buffer sizes
 */
#define VERTEX_BUFFER_LENGTH 2000
#define INDEX_BUFFER_LENGTH 10000

class MainWindow {

private:
    /*
     * Window variables
     */
    SDL_Window* window;
    int windowWidth, windowHeight;
    SDL_GLContext glContext;
    GLuint smoothShadingProgram,flatShadingProgram;
    GLuint vao, vbo[2], ebo;

    /*
     * Compiles a shader program from the GLSL source code
     */
    GLuint createProgram(const GLchar* vertexSource, const GLchar* fragmentSource);

    /*
     * Sets the uniform values for the shaders (eg. the view matrix)
     */
    void setUniforms(GLuint program);

    /*
     * View variables
     */
    struct {
        /*
         * Whether the mouse is visible and can drag to rotate the camera
         */
        bool dragMode = false;
        /*
         * Camera position
         */
        Vector3 pos{0, 1, 0};
        /*
         * See Vector3::fromAngles for description of azimuth and elevation
         */
        real azimuth=0, elevation=0;
        /*
         * Which direction the light is shining in
         */
        Vector3 lightDirection = Vector3::fromAngles(1, 0.5 - (real) M_PI_2, 1);

        real movementSpeed, mouseSensitivity;

    } view;

    struct {
        real nearClippingPlane, farClippingPlane, fieldOfView;
    } camera;

public:
    MainWindow(int width, int height, real nearClippingPlane, real farClippingPlane, real fieldOfView, real movementSpeed, real mouseSensitivity);

    void close();

    bool initSDL();
    bool initGL();

    void render(PhysicsWorld &world, bool initialWrite);

    void updateView(real deltaTime);

    void handleEvent(SDL_Event& e);
};


#endif //PHYSICSENGINE_MAINWINDOW_H
