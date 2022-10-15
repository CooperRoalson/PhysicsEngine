set (SOURCES render/Shape.cpp core/Vector3.cpp core/Matrix4.cpp core/Vector4.cpp physics/PhysicsObject.cpp physics/PhysicsObject.h physics/ForceGenerator.cpp physics/ForceGenerator.h physics/ForceRegistry.cpp physics/ForceRegistry.h)
add_executable(PhysicsEngine main.cpp ${SOURCES})

target_link_libraries(PhysicsEngine ${SDL2_LIBRARIES} "-framework OpenGL")