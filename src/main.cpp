#define GL_GLEXT_PROTOTYPES

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "render/shaders.h"
#include "render/Shape.h"
#include "core/Vector3.h"
#include "core/precision.h"
#include "core/Matrix4.h"
#include "physics/PhysicsObject.h"

#define DEFAULT_WINDOW_WIDTH 1000
#define DEFAULT_WINDOW_HEIGHT 1000
#define UPDATES_PER_SECOND 60

#define NEAR_CLIPPING 0.5
#define FAR_CLIPPING 100
#define FOV 100*M_PI/180

#define MOVEMENT_SPEED 3.5f
#define MOUSE_SENSITIVITY 0.004f

#define VERTEX_BUFFER_LENGTH 2000
#define INDEX_BUFFER_LENGTH 10000

struct {
    SDL_Window* window;
    int windowWidth, windowHeight;
    SDL_GLContext glContext;
    GLuint smoothShadingProgram,flatShadingProgram;
    GLuint vao, vbo[2], ebo;

    std::vector<PhysicsObject> objects;
} mainWindow;

struct {
    bool dragMode = false;

    Vector3 pos {0,1,0};
    /*
     * See Vector3::fromAngles for description of azimuth and elevation
     */
    real azimuth, elevation;
    Vector3 lightDirection = Vector3::fromAngles(1,0.5-(real)M_PI_2,1);
} view;

GLuint createProgram(const GLchar* vertexSource, const GLchar* fragmentSource) {

    int infoLogLength;
    GLint status = GL_FALSE;

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader,1,&vertexSource,NULL);
    glCompileShader(vShader);

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        std::cout << "Unable to compile vertex shader!" << std::endl;
        glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(vShader, infoLogLength, NULL, infoLog);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        return -1;
    }

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader,1,&fragmentSource,NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        std::cout << "Unable to compile fragment shader!" << std::endl;
        glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(fShader, infoLogLength, NULL, infoLog);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        return -1;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program,vShader);
    glAttachShader(program,fShader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
        std::cout << "Error linking program!" << std::endl;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength];
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        return -1;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}

bool initSDL() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    mainWindow.window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, flags);

    if (!mainWindow.window){
        std::cout << "Failed to create window! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GetWindowSize(mainWindow.window, &mainWindow.windowWidth, &mainWindow.windowHeight);

    SDL_SetRelativeMouseMode((SDL_bool) !view.dragMode);

    return true;
}

void close() {
    SDL_DestroyWindow(mainWindow.window);
    SDL_GL_DeleteContext(mainWindow.glContext);
    SDL_Quit();
}

bool initGL() {
    // Use OpenGL 4.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    mainWindow.glContext = SDL_GL_CreateContext(mainWindow.window);

    if(!mainWindow.glContext) {
        std::cout << "Failed to create OpenGL context! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    glViewport(0, 0, mainWindow.windowWidth, mainWindow.windowHeight);

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cout << "Failed to set VSync! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if ((mainWindow.smoothShadingProgram = createProgram(SMOOTH_VERT_SHADER_SRC,SMOOTH_FRAG_SHADER_SRC)) == -1) {
        return false;
    }
    if ((mainWindow.flatShadingProgram = createProgram(FLAT_VERT_SHADER_SRC,FLAT_FRAG_SHADER_SRC)) == -1) {
        return false;
    }

    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    glGenVertexArrays(1,&mainWindow.vao);
    glBindVertexArray(mainWindow.vao);

    glGenBuffers(2, mainWindow.vbo);

    glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[0]);
    // Positions
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normals
    glVertexAttribPointer(2,3,GL_FLOAT,GL_TRUE,6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[1]);
    // Colors
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLvoid*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mainWindow.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainWindow.ebo);

    glBindVertexArray(0);

    return true;
}

void initGeometry() {
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    mainWindow.objects.emplace_back(Vector3(),Vector3(),0,Shape::tiledFloor(Vector3(),10,1,C_BLACK,C_PURPLE));

    mainWindow.objects.push_back(Particle(Vector3(0,2,-2),Vector3(0,3,0),1,C_RED));
}

void setUniforms(GLuint program) {
    // View matrix
    GLuint viewMatrixLoc = glGetUniformLocation(program,"viewMatrix");
    Matrix4 viewMatrix = Matrix4::viewMatrix(view.pos,view.azimuth,view.elevation,0);
    GLfloat* viewMatrixArr = viewMatrix.toGLFloatArray();
    glUniformMatrix4fv(viewMatrixLoc,1,GL_TRUE,viewMatrixArr);
    delete[] viewMatrixArr;

    //std::cout << "pos: " << view.pos << std::endl;
    //std::cout << "View: " << Vector3::fromAngles(view.azimuth,view.elevation,1) << std::endl;

    // Projection matrix
    GLuint projMatrixLoc = glGetUniformLocation(program,"projectionMatrix");
    Matrix4 projMatrix = Matrix4::perspectiveProjectionMatrix(FOV,NEAR_CLIPPING,FAR_CLIPPING,(real)mainWindow.windowWidth/mainWindow.windowHeight);
    GLfloat* projMatrixArr = projMatrix.toGLFloatArray();
    glUniformMatrix4fv(projMatrixLoc,1,GL_TRUE,projMatrixArr);
    delete[] projMatrixArr;

    //std::cout << "pos of (0,0,3): " << /*projMatrix.multiply(*/viewMatrix.multiply(Vector4(0,0,3,1)) << std::endl;

    // Light direction
    GLuint lightDirID = glGetUniformLocation(program,"lightDir");
    glUniform3f(lightDirID, view.lightDirection.x, view.lightDirection.y, view.lightDirection.z);

}

void render(bool initialWrite) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    unsigned int numVertices = 0, numIndices = 0;
    for (PhysicsObject &obj : mainWindow.objects) {
        numVertices += obj.getModel().numVertices();
        numIndices += obj.getModel().numIndices();
    }

    Vector3* positions = new Vector3[numVertices*2];
    VertexColor* colors;
    GLuint* indices;
    if (initialWrite) {
        colors = new VertexColor[numVertices];
        indices = new GLuint[numIndices];
    }
    int vertexIdx = 0, indexIdx = 0;
    for (PhysicsObject &obj : mainWindow.objects) {
        if (obj.getModel().isFlatShaded()) {continue;}
        obj.getModel().writeVertexPositionsAndNormals(positions + vertexIdx*2,obj.getModelMatrix());
        if (initialWrite) {
            obj.getModel().writeVertexColors(colors + vertexIdx);
            obj.getModel().writeIndices(indices + indexIdx, vertexIdx);
        }
        vertexIdx += obj.getModel().numVertices();
        indexIdx += obj.getModel().numIndices();
    }
    unsigned int flatShadingStart = indexIdx;
    for (PhysicsObject &obj : mainWindow.objects) {
        if (!obj.getModel().isFlatShaded()) {continue;}
        obj.getModel().writeVertexPositionsAndNormals(positions + vertexIdx*2,obj.getModelMatrix());
        if (initialWrite) {
            obj.getModel().writeVertexColors(colors + vertexIdx);
            obj.getModel().writeIndices(indices + indexIdx, vertexIdx);
        }
        vertexIdx += obj.getModel().numVertices();
        indexIdx += obj.getModel().numIndices();
    }

    glBindVertexArray(mainWindow.vao);

    if (!initialWrite) {
        glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(Vector3)*numVertices, positions);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH * 2*sizeof(Vector3), positions, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH * sizeof(VertexColor), colors, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainWindow.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_LENGTH * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

    glUseProgram(mainWindow.smoothShadingProgram);
    setUniforms(mainWindow.smoothShadingProgram);
    glDrawElements(GL_TRIANGLES,flatShadingStart,GL_UNSIGNED_INT,(GLvoid*)0);

    glUseProgram(mainWindow.flatShadingProgram);
    setUniforms(mainWindow.flatShadingProgram);
    glDrawElements(GL_TRIANGLES,numIndices-flatShadingStart,GL_UNSIGNED_INT,(GLvoid*)(flatShadingStart*sizeof(GLuint)));

    glUseProgram(0);
    glBindVertexArray(0);


    SDL_GL_SwapWindow(mainWindow.window);

}

void update(real timeDelta) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    Vector3 deltaPos;
    if (keystate[SDL_SCANCODE_W]) {deltaPos += Vector3::fromAngles(view.azimuth,view.elevation,1);}
    if (keystate[SDL_SCANCODE_S]) {deltaPos += -Vector3::fromAngles(view.azimuth,view.elevation,1);}
    if (keystate[SDL_SCANCODE_A]) {deltaPos += Vector3::fromAngles(view.azimuth+(real)M_PI_2,0,1);}
    if (keystate[SDL_SCANCODE_D]) {deltaPos += -Vector3::fromAngles(view.azimuth+(real)M_PI_2,0,1);}
    view.pos += deltaPos.normalized()*MOVEMENT_SPEED*timeDelta;
    if (keystate[SDL_SCANCODE_SPACE]) {view.pos += Vector3::UP*MOVEMENT_SPEED*timeDelta;}//Vector3{0,MOVEMENT_SPEED*timeDelta,0};}
    if (keystate[SDL_SCANCODE_LSHIFT]) {view.pos += Vector3::DOWN*MOVEMENT_SPEED*timeDelta;}//Vector3{0,-MOVEMENT_SPEED*timeDelta,0};}


    for (PhysicsObject& obj : mainWindow.objects) {
        if (obj.hasFiniteMass()) {obj.addAcceleration(PhysicsObject::GRAVITY);}

        obj.update(timeDelta);

    }

}

void handleEvents(SDL_Event& e, bool& quit) {
    static bool mouseDragged = false;
    while (SDL_PollEvent(&e)){
        switch(e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button & SDL_BUTTON_LMASK) {
                    mouseDragged = false;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (!mouseDragged) {
                    view.dragMode = !view.dragMode;
                    SDL_SetRelativeMouseMode((SDL_bool) !view.dragMode);
                    SDL_WarpMouseInWindow(mainWindow.window,mainWindow.windowWidth/2,mainWindow.windowHeight/2);
                }
                break;
            case SDL_MOUSEMOTION:
                mouseDragged = true;
                if (!view.dragMode || (e.motion.state & SDL_BUTTON_LMASK)) {
                    view.elevation = std::clamp(view.elevation - (real) e.motion.yrel * MOUSE_SENSITIVITY * (view.dragMode ? -1 : 1), (real) -M_PI_2,(real) M_PI_2);
                    view.azimuth = real_mod((view.azimuth - (real) e.motion.xrel * MOUSE_SENSITIVITY * (view.dragMode ? -1 : 1)), (real) (2 * M_PI));
                }
                break;
            case SDL_WINDOWEVENT:
                SDL_WindowEvent we = e.window;
                if (we.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    mainWindow.windowWidth = we.data1;
                    mainWindow.windowHeight = we.data2;
                    glViewport(0, 0, mainWindow.windowWidth, mainWindow.windowHeight);
                }
                break;
        }
    }

}

int main(){
    if (!initSDL()) {
        close();
        return 1;
    }
    std::cout << "Successfully initiated SDL" << std::endl;

    if (!initGL()) {
        close();
        return 1;
    }
    std::cout << "Successfully initiated OpenGL" << std::endl;

    initGeometry();
    std::cout << "Successfully initiated geometry" << std::endl;

    render(true);

    // Loop timing, in milliseconds
    Uint32 timeStep = 1000.0f/UPDATES_PER_SECOND;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime,accumulatedTime=0;

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        accumulatedTime += currentTime - lastTime;

        while (accumulatedTime >= timeStep)
        {
            handleEvents(e, quit);
            update(timeStep/1000.0f);
            accumulatedTime -= timeStep;
        }

        render(false);
    }

    close();

    return 0;
}