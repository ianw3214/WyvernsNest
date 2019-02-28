#pragma once

#include <GL/glew.h>
#include "imgui.h"

#include "glwrappers.hpp"
#include "shader.hpp"
#include "imgui_impl_opengl3.h"

#include <stdexcept>

namespace GL {
    
    void init() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw new std::runtime_error("GLEW failed to initialize");
        }

        glClearColor(0.f, 0.f, 0.f, 1.f);
    }

};