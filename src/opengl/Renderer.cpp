#include <iostream>
#include <GL/glew.h>

#include <ui/Renderer.h>

#ifdef _MSC_VER
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")
#endif

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

namespace EngFlow {
    namespace UI {
        struct RendererData {
            GLuint shaderProgram;
        };
        Renderer::Renderer(std::shared_ptr<Frame> frame) : data(new RendererData), frame(frame) {}
        Renderer::~Renderer(){delete data;}
        void Renderer::init() {
#ifdef _DEBUG
            std::cout << "Renderer initializing" << std::endl;
#endif
            glewInit();
#ifdef _DEBUG
            // During init, enable debug output
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(MessageCallback, 0);
#endif
        }
        void Renderer::render() {
            glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            frame->draw();

            frame->getWindow()->glswap();
        }
    }
}