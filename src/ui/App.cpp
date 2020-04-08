#include <memory>
#include <thread>

#include <ui/Window.h>
#include <ui/menus/MainAppMenu.h>
#include <ui/Renderer.h>
#include <ui/elements/Frame.h>
#include <ui/elements/DraggableRectangle.h>
#include <ui/elements/Label.h>
#include <ui/UITypes.h>
#include <ui/Events.h>

#include <ui/App.h>

namespace EngFlow {
	namespace UI {
        class App {
            std::shared_ptr<Window> win;
            std::shared_ptr<Frame> frame;
            std::shared_ptr<Renderer> ren;
            bool running;
        public:
            App(): win(new Window), frame(new Frame(win)), ren(new Renderer(frame)), running(false){}
            void launch() {
                win->open();
                auto appMenu = UIPTR(new MainAppMenu());
                frame->addChild(appMenu);
                ren->init();
            }
            void run() {
                using namespace std::chrono_literals;
                running = true;
                bool eventHappened = true;
                Event event;
                while (running) {
                    while (!std::holds_alternative<Events::NO_EVENT>(event = win->pollEvents())) {
                        eventHappened = true;
                        if (std::holds_alternative<Events::QUIT>(event)) {
                            running = false;
                            break;
                        }
                        frame->handleEvent(event);
                    }
                    if (eventHappened)
                        ren->render();
                    eventHappened = false;
                    std::this_thread::sleep_for(5ms);
                }
            }
            void close(){
                win->close();
            }
        };
        void LaunchApp() {
            App ap;
            ap.launch();
            ap.run();
            ap.close();
        }
	};
};