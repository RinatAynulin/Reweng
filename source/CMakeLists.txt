cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT game)
project (${PROJECT})

set(COMMON_SOURCE_LIST common/error.cpp
                       common/error.h
                       common/utils.h)

set(ENTITY_COMPONENTS_SOURCE_LIST EntityEngine/components/inputcomponents.h)                       

set(EVENTS_SOURCE_LIST events/eventmanager.h
                       events/eventmanager.cpp
                       events/eventmanagerutils.h
                       events/eventmanagerutils.cpp
                       events/event.h)

set(ENTITY_SYSTEMS_SOURCE_LIST EntityEngine/systems/inputsystem.h
                               EntityEngine/systems/inputsystem.cpp)


set(ENTITY_ENGINE_SOURCE_LIST  EntityEngine/entity.h
                               EntityEngine/entity.cpp
                               EntityEngine/component.h
                               EntityEngine/componentpool.h
                               EntityEngine/componentpool.cpp
                               EntityEngine/componentutils.h
                               EntityEngine/componentutils.cpp
                               EntityEngine/system.h
                               EntityEngine/context.h
                               EntityEngine/context.cpp
                               EntityEngine/group.h
                               EntityEngine/group.cpp
                               EntityEngine/entityevents.h)

set(RENDERER_SOURCE_LIST       renderer/renderer.cpp 
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
source_group(game\\events FILES ${EVENTS_SOURCE_LIST})
source_group(game\\EntityEngine FILES ${ENTITY_ENGINE_SOURCE_LIST})
source_group(game\\EntityEngine\\components FILES ${ENTITY_COMPONENTS_SOURCE_LIST})
source_group(game\\EntityEngine\\systems FILES ${ENTITY_SYSTEMS_SOURCE_LIST})
source_group(game\\common FILES ${COMMON_SOURCE_LIST})
source_group(game\\renderer FILES ${RENDERER_SOURCE_LIST})

add_library(${PROJECT} ${COMMON_SOURCE_LIST}
                       ${EVENTS_SOURCE_LIST}
                       ${ENTITY_COMPONENTS_SOURCE_LIST}
                       ${ENTITY_SYSTEMS_SOURCE_LIST}
                       ${ENTITY_ENGINE_SOURCE_LIST}
                       ${RENDERER_SOURCE_LIST})

target_link_libraries(${PROJECT} glfw3 glad opengl32.lib )