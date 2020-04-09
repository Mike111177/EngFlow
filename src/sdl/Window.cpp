#include <iostream>

#include <SDL.h>

#include <ui/Window.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480



namespace EngFlow {
    namespace UI {
        class WindowIMPL {
            SDL_Window* window = NULL;
            SDL_Surface* screenSurface = NULL;
            SDL_GLContext glContext = NULL;       
            bool is_Open = false;

            static unsigned int wCount;
            static bool sdlInitialized;
            static bool initSDL() {
                if (!sdlInitialized) {
                    std::cout << "SDL Starting" << std::endl;
                    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
                        return false;
                    }
                }
                sdlInitialized = true;
                return true;
            }
            bool spawnWindow() {
                 std::cout << "SDL Spawning Window" << std::endl;
                 window = SDL_CreateWindow(
                    "EngFlow",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                );
                if (window == NULL) {
                    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
                    return false;
                }
                return true;
            }
            bool createGLContext() {
                glContext = SDL_GL_CreateContext(window);
                if (glContext == nullptr) {
                    // Display error message
                    std::string msg = "OpenGL context could not be created: ";
                    msg.append(SDL_GetError());
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Failed", msg.c_str(), nullptr);
                    return false;
                }
                return true;
            }
        public:
            bool open() {
                is_Open = true;
                wCount++;
                if (initSDL()) {
                    if (spawnWindow()) {
                        if (createGLContext()) {
                            return true;
                        }
                    }
                }
                close();    
                return false;
            }
            void close() {
                is_Open = false;
                wCount--;
                if (sdlInitialized) {
                    std::cout << "SDL Destroying Window" << std::endl;
                    SDL_DestroyWindow(window);
                    if (wCount == 0) {
                        std::cout << "SDL Shutting Down" << std::endl;
                        SDL_Quit();
                        sdlInitialized = false;
                    }
                }
            }
            Event SDL_EventToUiEvent(SDL_Event event) {
                switch (event.type) {
                case SDL_QUIT:
                    return Events::QUIT();
                case SDL_MOUSEMOTION:
                    UISize s;
                    SDL_GetWindowSize(window, &s.w, &s.h);
                    return Events::MOUSE_MOVE{ event.motion.x, s.h - event.motion.y };
                case SDL_MOUSEBUTTONDOWN:
                    return Events::MOUSE_PRESS();
                case SDL_MOUSEBUTTONUP:
                    return Events::MOUSE_RELEASE();
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        return Events::WINDOW_RESIZE{ event.window.data1, event.window.data2 };
                    case SDL_WINDOWEVENT_SHOWN:
                        return Events::WINDOW_SHOWN();
                    case SDL_WINDOWEVENT_EXPOSED:
                        return Events::WINDOW_EXPOSED();
                    case SDL_WINDOWEVENT_HIDDEN:
                        return Events::WINDOW_HIDDEN();
#ifdef _DEBUG
                    default:
                        std::cout << "Unhandled SDL Window Event (" << +event.window.event << ")" << std::endl;
#endif
                    }
                    break;
#ifdef _DEBUG
                default:
                    std::cout << "Unhandled SDL Event (" << +event.type << ")" << std::endl;
#endif
                }
                return Events::NO_EVENT();
            }
            ~WindowIMPL() {
                close();
            }
            void glswap() {
                SDL_GL_SwapWindow(window);
            }
            UISize getSize() {
                UISize ret;
                SDL_GetWindowSize(window, &ret.w, &ret.h);
                return ret;
            }
            Event pollEvents() {
                SDL_Event event;
                if (!SDL_PollEvent(&event)) return Events::NO_EVENT();
                return SDL_EventToUiEvent(event);
            }
        };
        bool WindowIMPL::sdlInitialized = false;
        unsigned int WindowIMPL::wCount = 0;


        Window::Window(): impl(new WindowIMPL) {}
        Window::~Window() { delete impl; }
        bool Window::open() { return impl->open(); }
        void Window::close() { return impl->close(); }
        Event Window::pollEvents() { return impl->pollEvents(); }
        void Window::glswap() {return impl->glswap();}
        UISize Window::getSize(){return impl->getSize();}
        
    };
};