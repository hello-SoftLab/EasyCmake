
#setting cmake version

cmake_minimum_required(VERSION 3.18)

#adding useful functions

function(DIR_EXISTS variable dir_path)

file(GLOB ${variable}_check ${dir_path}/*)

list(LENGTH ${variable}_check ${variable}_len)

if(${${variable}_len} EQUAL 0)

set(${variable} FALSE PARENT_SCOPE)

else()

set(${variable} TRUE PARENT_SCOPE)

endif()

endfunction()

#adding extra cmake libs
include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)


#project name
project("EasyCmake")

#creating variables for ease of adding libraries
set(${PROJECT_NAME}_DEPS_TO_BUILD )
set(${PROJECT_NAME}_SOURCE_FILES )
set(${PROJECT_NAME}_INCLUDES )
set(${PROJECT_NAME}_LIBRARIES )

        
        

# repository download and settings for entt...

    dir_exists(entt_exists ${PROJECT_SOURCE_DIR}/vendor/entt)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/entt/single_include)


    if(NOT ${entt_exists})
        FetchContent_Declare(ENTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG origin/master
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/entt
        )

        FetchContent_MakeAvailable(ENTT)

    endif()
                    


# repository download and settings for imgui...

    dir_exists(imgui_exists ${PROJECT_SOURCE_DIR}/vendor/imgui)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/imgui)

    
    # adding sources


        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp)

        list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp)


    if(NOT ${imgui_exists})
        FetchContent_Declare(IMGUI
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG origin/docking
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui
        )

        FetchContent_MakeAvailable(IMGUI)

    endif()
                    


# repository download and settings for yaml...

    dir_exists(yaml_exists ${PROJECT_SOURCE_DIR}/vendor/yaml)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/yaml/include/)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/yaml/lib/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${yaml_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(YAML
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
        GIT_TAG yaml-cpp-0.7.0
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/yaml
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -DYAML_CPP_BUILD_TESTS=OFF
                        
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/yaml/lib/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD YAML)

    endif()




# repository download and settings for dialog...

    dir_exists(dialog_exists ${PROJECT_SOURCE_DIR}/vendor/dialog)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/dialog/include)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/dialog/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nfd${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${dialog_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(DIALOG
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended
        GIT_TAG origin/master
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/dialog
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/dialog/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nfd${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD DIALOG)

    endif()




# repository download and settings for glfw...

    dir_exists(glfw_exists ${PROJECT_SOURCE_DIR}/vendor/glfw)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/glfw/include)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${glfw_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.6
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/glfw
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/glfw/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD GLFW)

    endif()




# repository download and settings for glew...

    dir_exists(glew_exists ${PROJECT_SOURCE_DIR}/vendor/glew)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/glew/include)


    # adding libraries


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${PROJECT_SOURCE_DIR}/vendor/glew/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX})


    if(NOT ${glew_exists} OR NOT ${${PROJECT_NAME}_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        ExternalProject_Add(GLEW
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
        GIT_TAG glew-cmake-2.2.0
        CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/glew
                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                    -Dglew-cmake_BUILD_SHARED=OFF
                        
                    -DONLY_LIBS=ON
                        
        BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/glew/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glew$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}
        )

        list(APPEND ${PROJECT_NAME}_DEPS_TO_BUILD GLEW)

    endif()




# repository download and settings for glm...

    dir_exists(glm_exists ${PROJECT_SOURCE_DIR}/vendor/glm)


    # adding includes:


        list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor/glm)


    if(NOT ${glm_exists})
        FetchContent_Declare(GLM
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm
        )

        FetchContent_MakeAvailable(GLM)

    endif()
                    


# finding package and adding settings for alias OpenGL...


    find_package(OpenGL REQUIRED)


    # adding libraries:


        list(APPEND ${PROJECT_NAME}_LIBRARIES ${OpenGL_LIBRARIES})

        list(APPEND ${PROJECT_NAME}_LIBRARIES OpenGL::GLU)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/window/window.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/cmake_generator/cmake_generator.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/cmake_generator/repository.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/general/color.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/general/helpers.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/component.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/object.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/object_properties.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/object_property_register.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/object/registry.cpp)


list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/cmake_generator/external_repo.cpp)


#creating executable
add_executable(${PROJECT_NAME}

        ${${PROJECT_NAME}_SOURCE_FILES}
)
        
#setting c/cpp standard

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

        
#adding dependencies

foreach(X ${${PROJECT_NAME}_DEPS_TO_BUILD})

    add_dependencies(${PROJECT_NAME} ${X})

endforeach()


            
# ------------- linking libraries -------------

    target_link_libraries(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_LIBRARIES})



#------------ include directories -------------


    list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/vendor)


    list(APPEND ${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR}/source)


    target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDES})



# cacheing the build type

set(${PROJECT_NAME}_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE INTERNAL "")
        
