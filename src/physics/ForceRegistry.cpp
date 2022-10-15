#include "ForceRegistry.h"

bool ForceRegistry::ForceRegistration::operator==(const ForceRegistration& other) {
    return object == other.object && forceGenerator == other.forceGenerator;
}

void ForceRegistry::add(PhysicsObject *object, ForceGenerator *fg) {
    registrations.push_back(ForceRegistration{object,fg});
}

void ForceRegistry::remove(PhysicsObject *object, ForceGenerator *fg) {
    std::remove(registrations.begin(), registrations.end(), ForceRegistration{object,fg});
}

void ForceRegistry::clear() {
    registrations.clear();
}

void ForceRegistry::updateForces(real deltaTime) {
    for (ForceRegistration& reg : registrations) {
        reg.forceGenerator->updateForce(reg.object,deltaTime);
    }
}
