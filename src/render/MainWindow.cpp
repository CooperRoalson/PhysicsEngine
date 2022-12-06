#include "MainWindow.h"
#include "Shape.h"

#include <iostream>

bool MainWindow::initSDL() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, flags);

    if (!window){
        std::cout << "Failed to create window! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_SetRelativeMouseMode((SDL_bool) !view.dragMode);

    return true;
}

MainWindow::MainWindow(int width, int height, real nearClippingPlane, real farClippingPlane, real fieldOfView, real movementSpeed, real mouseSensitivity)
: windowWidth(width),windowHeight(height),camera{nearClippingPlane, farClippingPlane, fieldOfView} {
    view.movementSpeed = movementSpeed;
    view.mouseSensitivity = mouseSensitivity;
}

void MainWindow::close() {
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
}

GLuint MainWindow::createProgram(const GLchar* vertexSource, const GLchar* fragmentSource) {

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

bool MainWindow::initGL() {
    // Use OpenGL 4.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(window);

    if(!glContext) {
        std::cout << "Failed to create OpenGL context! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cout << "Failed to set VSync! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if ((smoothShadingProgram = createProgram(SMOOTH_VERT_SHADER_SRC,SMOOTH_FRAG_SHADER_SRC)) == -1) {
        return false;
    }
    if ((flatShadingProgram = createProgram(FLAT_VERT_SHADER_SRC,FLAT_FRAG_SHADER_SRC)) == -1) {
        return false;
    }

    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Positions
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normals
    glVertexAttribPointer(2,3,GL_FLOAT,GL_TRUE,6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // Colors
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLvoid*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);

    return true;
}

void MainWindow::setUniforms(GLuint program) {
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
    Matrix4 projMatrix = Matrix4::perspectiveProjectionMatrix(camera.fieldOfView,camera.nearClippingPlane,camera.farClippingPlane,(real)windowWidth/windowHeight);
    GLfloat* projMatrixArr = projMatrix.toGLFloatArray();
    glUniformMatrix4fv(projMatrixLoc,1,GL_TRUE,projMatrixArr);
    delete[] projMatrixArr;

    //std::cout << "pos of (0,0,3): " << /*projMatrix.multiply(*/viewMatrix.multiply(Vector4(0,0,3,1)) << std::endl;

    // Light direction
    GLuint lightDirID = glGetUniformLocation(program,"lightDir");
    glUniform3f(lightDirID, view.lightDirection.x, view.lightDirection.y, view.lightDirection.z);

}

void MainWindow::render(PhysicsWorld &world, bool initialWrite) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    unsigned int numVertices, numIndices;
    world.writeVertexAndIndexCounts(numVertices, numIndices);

    Vector3* positions = new Vector3[numVertices*2];
    VertexColor* colors;
    GLuint* indices;
    if (initialWrite) {
        colors = new VertexColor[numVertices];
        indices = new GLuint[numIndices];
    }
    int vertexIdx = 0, indexIdx = 0;
    world.writeObjectData(false, initialWrite, positions, colors, indices, vertexIdx, indexIdx);
    unsigned int flatShadingStart = indexIdx;
    world.writeObjectData(true, initialWrite, positions, colors, indices, vertexIdx, indexIdx);

    glBindVertexArray(vao);

    if (!initialWrite) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(Vector3)*numVertices, positions);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH * 2*sizeof(Vector3), positions, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_LENGTH * sizeof(VertexColor), colors, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_LENGTH * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

    delete[] positions;
    if (initialWrite) {
        delete[] colors;
        delete[] indices;
    }

    glUseProgram(smoothShadingProgram);
    setUniforms(smoothShadingProgram);
    glDrawElements(GL_TRIANGLES,flatShadingStart,GL_UNSIGNED_INT,(GLvoid*)0);

    glUseProgram(flatShadingProgram);
    setUniforms(flatShadingProgram);
    glDrawElements(GL_TRIANGLES,numIndices-flatShadingStart,GL_UNSIGNED_INT,(GLvoid*)(flatShadingStart*sizeof(GLuint)));

    glUseProgram(0);
    glBindVertexArray(0);


    SDL_GL_SwapWindow(window);

}

void MainWindow::updateView(real deltaTime) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    Vector3 deltaPos;
    if (keystate[SDL_SCANCODE_W]) {deltaPos += Vector3::fromAngles(view.azimuth,view.elevation,1);}
    if (keystate[SDL_SCANCODE_S]) {deltaPos += -Vector3::fromAngles(view.azimuth,view.elevation,1);}
    if (keystate[SDL_SCANCODE_A]) {deltaPos += Vector3::fromAngles(view.azimuth+(real)M_PI_2,0,1);}
    if (keystate[SDL_SCANCODE_D]) {deltaPos += -Vector3::fromAngles(view.azimuth+(real)M_PI_2,0,1);}
    view.pos += deltaPos.normalized()*view.movementSpeed*deltaTime;
    if (keystate[SDL_SCANCODE_SPACE]) {view.pos += Vector3::UP*view.movementSpeed*deltaTime;}
    if (keystate[SDL_SCANCODE_LSHIFT]) {view.pos += Vector3::DOWN*view.movementSpeed*deltaTime;}
}

void MainWindow::handleEvent(SDL_Event &e) {
    static bool mouseDragged = false;
    switch(e.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button & SDL_BUTTON_LMASK) {
                mouseDragged = false;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (!mouseDragged) {
                view.dragMode = !view.dragMode;
                SDL_SetRelativeMouseMode((SDL_bool) !view.dragMode);
                SDL_WarpMouseInWindow(window,windowWidth/2,windowHeight/2);
            }
            break;
        case SDL_MOUSEMOTION:
            mouseDragged = true;
            if (!view.dragMode || (e.motion.state & SDL_BUTTON_LMASK)) {
                view.elevation = std::clamp(view.elevation - (real) e.motion.yrel * view.mouseSensitivity * (view.dragMode ? -1 : 1), (real) -M_PI_2,(real) M_PI_2);
                view.azimuth = real_mod((view.azimuth - (real) e.motion.xrel * view.mouseSensitivity * (view.dragMode ? -1 : 1)), (real) (2 * M_PI));
            }
            break;
        case SDL_WINDOWEVENT:
            SDL_WindowEvent we = e.window;
            if (we.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                windowWidth = we.data1;
                windowHeight = we.data2;
                glViewport(0, 0, windowWidth, windowHeight);
            }
            break;
    }
}
