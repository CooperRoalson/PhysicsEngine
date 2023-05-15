#include <iostream>
#include <vector>
#include <cmath>

#include "physics/PhysicsWorld.h"
#include "physics/ObjectLink.h"
#include "render/MainWindow.h"
#include "physics/RigidBody.h"

#define UPDATES_PER_SECOND 60

#define DEFAULT_WINDOW_WIDTH 1000
#define DEFAULT_WINDOW_HEIGHT 1000

#define NEAR_CLIPPING 0.5
#define FAR_CLIPPING 100
#define FOV 100*M_PI/180

#define MOVEMENT_SPEED 3.5f
#define MOUSE_SENSITIVITY 0.004f

#define MAX_CONTACTS 2000

PhysicsWorld world(MAX_CONTACTS);
MainWindow mainWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, NEAR_CLIPPING, FAR_CLIPPING, FOV, MOVEMENT_SPEED, MOUSE_SENSITIVITY);

void initGeometry() {
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    UniformGravityForce *gravity;
    world.addForceGenerator(gravity = new UniformGravityForce(Vector3(0,-9.8,0)));

    world.addObject(new PhysicsObject(Vector3(),Vector3(),0,false,Shape::tiledFloor(Vector3(),10,1,{0.15,0.15,0.15},C_PURPLE)));

    RigidBodyModel *cube = new RectangularPrismModel(0.5,0.5,0.5);
    RigidBody *r1, *r2, *r3, *r4;
    world.addObject(r1 = new RigidBody(Vector3(0,2,0), Vector3(), Quaternion(), Vector3(), 0.1, true, cube,C_BLUE));
    world.addObject(r2 = new RigidBody(Vector3(0,2.86,0), Vector3(), Quaternion(), Vector3(), 0.1, true, cube,C_RED));
    world.addObject(r3 = new RigidBody(Vector3(0.5,1.5,0), Vector3(), Quaternion(), Vector3(), 0.1, true, cube,C_YELLOW));
    world.addObject(r4 = new RigidBody(Vector3(0,2.5,0.65), Vector3(), Quaternion(), Vector3(), 0.1, true, cube,C_YELLOW));


    BVHTree tree;
    tree.insert(r1);
    tree.insert(r2);
    tree.insert(r3);
    tree.insert(r4);

    PotentialContact contacts[100];
    unsigned int num;
    std::cout << (num = tree.getPotentialContacts(contacts, 100)) << std::endl;
    for (int i = 0; i < num; i++) {
        std::cout << *contacts[i].bodies[0] << ", " << *contacts[i].bodies[1] << std::endl;
    }
    tree.print();

    // world.applyForceToObject(r1, gravity);
    // world.applyForceToObject(r2, gravity);
    // world.addContactGenerator(new FloorContactGenerator(r1, 0, 1));
    // world.addContactGenerator(new FloorContactGenerator(r2, 0, 1));

//    SpringForce *spring_1_2;
//    world.addForceGenerator(spring_1_2 = new SpringForce(r1, Vector3(0.25,0.25,0.25), r2, Vector3(-0.25,-0.25,-0.25),10.0f,1.0f,true));
//    world.applyForceToObject(r1,spring_1_2);
//    world.applyForceToObject(r2,spring_1_2);

    /*Particle *p1, *p2;
    world.addObject(p1 = new Particle(Vector3(-1,2,-2),Vector3(0,3,0),1,true,C_RED));
    world.addObject(p2 = new Particle(Vector3(-1,1,2),Vector3(0,0,0),1,true,C_BLUE));
    world.applyForceToObject(p1,gravity);
    world.applyForceToObject(p2,gravity);
    world.addContactGenerator(new FloorContactGenerator(p1, 0, 1));
    world.addContactGenerator(new FloorContactGenerator(p2, 0, 1));

    SpringForce *spring_1_2;
    world.addForceGenerator(spring_1_2 = new SpringForce(p1, Vector3(), p2, Vector3(),5.0f,1.0f,true));
    world.applyForceToObject(p1,spring_1_2);
    world.applyForceToObject(p2,spring_1_2);

    Particle *p3, *p4;
    world.addObject(p3 = new Particle(Vector3(3,2,0),Vector3(0,0,0),1,true,C_GREEN));
    world.addObject(p4 = new Particle(Vector3(0,2,0),Vector3(0,0,0),0,true,C_WHITE));
    SpringForce *spring_3_4;
    world.addForceGenerator(spring_3_4 = new SpringForce(p3, Vector3(), p4, Vector3(),8.0f,2.0f,true));
    world.applyForceToObject(p3,spring_3_4);

    Particle *p5;
    world.addObject(p5 = new Particle(Vector3(0,3,1.25),Vector3(0,5,0),1,true,C_YELLOW));
    world.applyForceToObject(p5,gravity);
    world.addContactGenerator(new FloorContactGenerator(p5, 0, 1));
    world.addContactGenerator(new ParticleCable(p4, p5, 1.5, 0.5));*/

    /*Particle *earth, *moon;
    world.addObject(earth = new Particle(Vector3(0,2,0),Vector3(0,0,0),0.0791,true,C_GREEN));
    world.addObject(moon = new Particle(Vector3(0,2,2),Vector3(2.51,0,0),0.0791*4,false,C_WHITE));

    GravitationalAttractionForce* earthGravity;
    world.addForceGenerator(earthGravity = new GravitationalAttractionForce(earth, 1));

    world.applyForceToObject(moon, earthGravity);*/

    /*Particle *p0,*p1,*p2,*p3,*p4;
    world.addObject(p0 = new Particle(Vector3(0,10,0),Vector3(0,0,0),0,true,C_BLACK));
    world.addObject(p1 = new Particle(Vector3(0,7,0),Vector3(0,5,2),1,true,C_WHITE));
    world.addObject(p2 = new Particle(Vector3(1,8,0),Vector3(0,5,0),1,true,C_WHITE));
    world.addObject(p3 = new Particle(Vector3(1,8,1),Vector3(0,5,0),1,true,C_WHITE));
    world.addObject(p4 = new Particle(Vector3(0,8,1),Vector3(0,5,0),1,true,C_WHITE));
    world.applyForceToObject(p1,gravity);
    world.applyForceToObject(p2,gravity);
    world.applyForceToObject(p3,gravity);
    world.applyForceToObject(p4,gravity);
    world.addContactGenerator(new ParticleCable(p0, p1, 3, 0.5));
    world.addContactGenerator(new FloorContactGenerator(p1, 0, 1));
    world.addContactGenerator(new FloorContactGenerator(p2, 0, 1));
    world.addContactGenerator(new FloorContactGenerator(p3, 0, 1));
    world.addContactGenerator(new FloorContactGenerator(p4, 0, 1));
    world.addContactGenerator(new ParticleRod(p1, p2, 1.41));
    world.addContactGenerator(new ParticleRod(p1, p3, 1.73));
    world.addContactGenerator(new ParticleRod(p1, p4, 1.41));
    world.addContactGenerator(new ParticleRod(p2, p3, 1));
    world.addContactGenerator(new ParticleRod(p2, p4, 1.41));
    world.addContactGenerator(new ParticleRod(p3, p4, 1));*/

}

void update(real deltaTime) {
    mainWindow.updateView(deltaTime);
    world.update(deltaTime);
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
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            case SDL_WINDOWEVENT:
               mainWindow.handleEvent(e);
               break;
        }
    }
}

int main() {
    if (!mainWindow.initSDL()) {
        mainWindow.close();
        return 1;
    }
    std::cout << "Successfully initiated SDL" << std::endl;

    if (!mainWindow.initGL()) {
        mainWindow.close();
        return 1;
    }
    std::cout << "Successfully initiated OpenGL" << std::endl;

    initGeometry();
    std::cout << "Successfully initiated geometry" << std::endl;

    mainWindow.render(world,true);

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

        mainWindow.render(world,false);
    }

    mainWindow.close();

    return 0;
}