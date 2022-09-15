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

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define FPS 60

#define NEAR_CLIPPING -1
#define FAR_CLIPPING 1
#define FOV 60*M_PI/180

#define MOVEMENT_SPEED 0.01f
#define MOUSE_SENSITIVITY 7

#define VERTEX_BUFFER_LENGTH 1000
#define INDEX_BUFFER_LENGTH 2000

struct {
    SDL_Window* window;
    SDL_GLContext glContext;
    GLuint program;
    GLuint vao, vbo[2], ebo;
    std::vector<Shape> shapes;
} mainWindow;

struct {
    Vector3 pos;
    real azimuth, elevation;
    Vector3 lightDirection = Vector3::fromAngles(1,0.2-(real)M_PI_2,1);
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

    glBindAttribLocation( program, 0, "LVertexPos2D" );

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
    mainWindow.window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, flags);

    if (!mainWindow.window){
        std::cout << "Failed to create window! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
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

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cout << "Failed to set VSync! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if ((mainWindow.program = createProgram(BASIC_VERT_SHADER_SRC,BASIC_FRAG_SHADER_SRC)) == -1) {
        return false;
    }

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
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,(GLvoid*)0);
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

    mainWindow.shapes.push_back(Shape::tiledFloor({0,0,-0.9},1,0.2,C_BLACK,C_PURPLE));
    mainWindow.shapes.push_back(Shape::cube({0,0,0},0.6,C_RED));

}

void close() {
    SDL_DestroyWindow(mainWindow.window);
    SDL_GL_DeleteContext(mainWindow.glContext);
    SDL_Quit();
}

void handleEvents(SDL_Event& e, bool& quit) {
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
            case SDL_MOUSEMOTION:
                if (e.motion.state & SDL_BUTTON_LMASK) {
                    float dx = e.motion.xrel;
                    float dy = e.motion.yrel;
                    view.elevation = fmin(fmax(view.elevation + dy/WINDOW_HEIGHT*MOUSE_SENSITIVITY,(real)-M_PI_2),(real)M_PI_2);
                    view.azimuth = fmod((view.azimuth + dx/WINDOW_WIDTH*MOUSE_SENSITIVITY),(real) (2*M_PI));
                }
        }
    }
}

void setUniforms() {
    // View matrix
    GLuint viewMatrixLoc = glGetUniformLocation(mainWindow.program,"viewMatrix");
    Matrix4 viewMatrix;
    viewMatrix.rotateZ((real) -M_PI_2);
    viewMatrix.rotateZYX(-view.azimuth,-(view.elevation+(real)M_PI_2),0);
    viewMatrix.translate(-view.pos);
    GLfloat* viewMatrixArr = viewMatrix.toGLFloatArray();
    glUniformMatrix4fv(viewMatrixLoc,1,GL_TRUE,viewMatrixArr);
    delete[] viewMatrixArr;

    // Projection matrix
    GLuint projMatrixLoc = glGetUniformLocation(mainWindow.program,"projectionMatrix");
    Matrix4 projMatrix; //= Matrix4::perspectiveProjectionMatrix(FOV,NEAR_CLIPPING,FAR_CLIPPING);
    //std::cout << projMatrix.multiply(Vector4{0.5,0.5,0.5,1}) << std::endl;
    GLfloat* projMatrixArr = projMatrix.toGLFloatArray();
    glUniformMatrix4fv(projMatrixLoc,1,GL_TRUE,projMatrixArr);
    delete[] projMatrixArr;

    // Light direction
    GLuint lightDirID = glGetUniformLocation(mainWindow.program,"lightDir");
    glUniform3f(lightDirID, view.lightDirection.x, view.lightDirection.y, view.lightDirection.z);


}

void render(bool initialWrite) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    unsigned int numVertices = 0, numIndices = 0;
    for (Shape &s : mainWindow.shapes) {
        numVertices += s.numVertices();
        numIndices += s.numIndices();
    }

    Vector3* positions = new Vector3[numVertices*2];
    VertexColor* colors;
    GLuint* indices;
    if (initialWrite) {
        colors = new VertexColor[numVertices];
        indices = new GLuint[numIndices];
    }
    int vertexIdx = 0, indexIdx = 0;
    for (Shape &s : mainWindow.shapes) {
        s.writeVertexPositionsAndNormals(positions + vertexIdx*2);
        if (initialWrite) {
            s.writeVertexColors(colors + vertexIdx);
            s.writeIndices(indices + indexIdx, vertexIdx);
        }
        vertexIdx += s.numVertices();
        indexIdx += s.numIndices();
    }

    glUseProgram(mainWindow.program);
    glBindVertexArray(mainWindow.vao);

    setUniforms();

    if (!initialWrite) {
        glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH * 2*sizeof(Vector3), positions, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, mainWindow.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH * sizeof(VertexColor), colors, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainWindow.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_LENGTH * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

    glDrawElements(GL_TRIANGLES,numIndices,GL_UNSIGNED_INT,(GLvoid*)0);

    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(mainWindow.window);

}

void update(real timeDelta) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W]) {view.pos += Vector3::fromAngles(view.azimuth,view.elevation,MOVEMENT_SPEED*timeDelta);}
    if (keystate[SDL_SCANCODE_S]) {view.pos += -Vector3::fromAngles(view.azimuth,view.elevation,MOVEMENT_SPEED*timeDelta);}
    if (keystate[SDL_SCANCODE_A]) {view.pos += Vector3::fromAngles(view.azimuth+(real)M_PI_2,0,MOVEMENT_SPEED*timeDelta);}
    if (keystate[SDL_SCANCODE_D]) {view.pos += -Vector3::fromAngles(view.azimuth+(real)M_PI_2,0,MOVEMENT_SPEED*timeDelta);}
    if (keystate[SDL_SCANCODE_SPACE]) {view.pos += Vector3{0,0,MOVEMENT_SPEED*timeDelta};}
    if (keystate[SDL_SCANCODE_LSHIFT]) {view.pos += Vector3{0,0,-MOVEMENT_SPEED*timeDelta};}

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
    real timeStep = 1000.0f/FPS;
    real currentTime = SDL_GetTicks();
    real lastTime,timeDelta,accumulatedTime=0;

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        timeDelta = currentTime - timeStep;
        accumulatedTime += timeDelta;

        while (accumulatedTime >= timeStep)
        {
            handleEvents(e, quit);
            update(timeStep/1000);
            accumulatedTime -= timeStep;
        }

        render(false);
    }

    close();

    return 0;
}