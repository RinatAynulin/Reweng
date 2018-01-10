cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT game)
project (${PROJECT})

set(COMMON_SOURCE_LIST          common/error.cpp
                                common/error.h
                                common/utils.h)

set(EVENTS_SOURCE_LIST          events/eventmanager.h
                                events/eventmanager.cpp
                                events/eventmanagerutils.h
                                events/eventmanagerutils.cpp
                                events/event.h)

set(GAME_COMPONENTS_SOURCE_LIST game/components/inputcomponents.h
                                game/components/camera.h
                                game/components/mesh.h
                                game/components/material.h
                                game/components/transform.h)

set(GAME_SYSTEMS_SOURCE_LIST    game/systems/inputsystem.h
                                game/systems/inputsystem.cpp
                                game/systems/inputcleanupsystem.h
                                game/systems/inputcleanupsystem.cpp)


set(ENTITY_ENGINE_SOURCE_LIST   ecs/entity.h
                                ecs/entity.cpp
                                ecs/component.h
                                ecs/componentpool.h
                                ecs/componentpool.cpp
                                ecs/componentutils.h
                                ecs/componentutils.cpp
                                ecs/system.h
                                ecs/context.h
                                ecs/context.cpp
                                ecs/group.h
                                ecs/group.cpp
                                ecs/entityevents.h)

set(RENDERER_SOURCE_LIST        renderer/renderer.cpp 
                                renderer/renderer.h
                                renderer/glshader.cpp
                                renderer/glshader.h
                                renderer/glshadermanager.cpp
                                renderer/glshadermanager.h
                                renderer/meshcomponent.h
                                renderer/glmeshmanager.cpp
                                renderer/glmeshmanager.h
                                renderer/vertex.h
                                renderer/gltexturemanager.cpp
                                renderer/gltexturemanager.h
                                renderer/gltexture.h
                                renderer/modelcomponent.h
                                renderer/glmaterialcomponent.h
                                renderer/gltransformcomponent.h)

source_group(game)
source_group(game\\components   FILES ${GAME_COMPONENTS_SOURCE_LIST})
source_group(game\\systems      FILES ${GAME_SYSTEMS_SOURCE_LIST})
source_group(events             FILES ${EVENTS_SOURCE_LIST})
source_group(ecs                FILES ${ENTITY_ENGINE_SOURCE_LIST})
source_group(ecs\\components    FILES ${ENTITY_COMPONENTS_SOURCE_LIST})
source_group(ecs\\systems       FILES ${ENTITY_SYSTEMS_SOURCE_LIST})
source_group(common             FILES ${COMMON_SOURCE_LIST})
source_group(renderer           FILES ${RENDERER_SOURCE_LIST})

add_library(${PROJECT}          ${COMMON_SOURCE_LIST}
                                ${GAME_COMPONENTS_SOURCE_LIST}
                                ${GAME_SYSTEMS_SOURCE_LIST}
                                ${EVENTS_SOURCE_LIST}
                                ${ENTITY_COMPONENTS_SOURCE_LIST}
                                ${ENTITY_SYSTEMS_SOURCE_LIST}
                                ${ENTITY_ENGINE_SOURCE_LIST}
                                ${RENDERER_SOURCE_LIST})

target_link_libraries(${PROJECT} glfw3 glad opengl32.lib )