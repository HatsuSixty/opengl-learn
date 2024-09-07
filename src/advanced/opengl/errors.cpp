#include "errors.hpp"

#include <GL/glew.h>

#include <iostream>

void gl_clear_errors()
{
    while (glGetError() != GL_NO_ERROR);
}

void gl_check_errors()
{
    while (GLenum error = glGetError()) {
        std::cerr << "ERROR: OpenGL error: error code 0x"
                  << std::hex << error << std::dec << std::endl;
    }
}