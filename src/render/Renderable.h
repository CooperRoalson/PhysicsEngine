#ifndef PHYSICSENGINE_RENDERABLE_H
#define PHYSICSENGINE_RENDERABLE_H

#include "Shape.h"

class Renderable {
public:
    virtual Shape getShape() const = 0;
};

#endif //PHYSICSENGINE_RENDERABLE_H
