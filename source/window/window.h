#pragma once 
#include "../global.h"
#include <functional>

class Camera;
class Framebuffer;
class Mesh;
class Window;
class VertexArray;
class Shader;
struct WindowEvents {

    WindowEvents(Window& win) : m_Master(win) {};

    yael::event_sink<void(Window&, bool)> FocusEvent();
    yael::event_sink<void(Window&,SDL_MouseWheelEvent)> MouseScrollEvent();
    yael::event_sink<void(Window&,SDL_MouseButtonEvent)> MouseButtonEvent();
    yael::event_sink<void(Window&,MouseEventProperties)> MouseEnteredWindowEvent();
    yael::event_sink<void(Window&,MouseEventProperties)> MouseLeftWindowEvent();
    yael::event_sink<void(Window&,SDL_KeyboardEvent)> KeyEvent();
    yael::event_sink<void(Window&,SDL_MouseMotionEvent)> MouseMovedEvent();
    yael::event_sink<void(Window&)> PreDrawingLoopEvent();
    yael::event_sink<void(Window&)> PostDrawingLoopEvent();
    yael::event_sink<void(Window&)> ClosingEvent();
    yael::event_sink<void(Window&,WindowResizedEventProperties)> ResizedEvent();
    yael::event_sink<void(Window&, SDL_Event&)> AllEvents();

private:
    Window& m_Master;

};



class WindowCreators {
public:
    WindowCreators(Window& wid) : m_Master(wid){}

    VertexArray& NewVertexArray();
    Shader& CachedShader(std::string shaderRelativePath,bool* loadResult=nullptr);
    

private:
    Window& m_Master;


};

class WindowSDLInitializerProperties {
    SDL_Window* window;
    SDL_GLContext context;
};

class Window {

public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    
    WindowEvents Events();

    WindowCreators Create();
    
    
    ecspp::NamedObjectHandle<GameObject> GetCurrentCamera();
    void SetClearColor(Color color);
    
    void DisableCamera();
    void SetCamera(GameObject obj);
    
    void SetViewPort(int x,int y,int width,int height);

    const WindowCreationProperties& Properties() const;
    SDL_GLContext& GetContext();
    float GetDeltaTime();
    

    void DrawFrame();
    
    
    static Window& GetCurrentWindow();
   
    static yael::event_sink<void(Window&)> WindowCreationEvent();

    static void SetSDLCreationFunc(std::function<WindowSDLInitializerProperties()> func);
    
    RayCastHit RayCast(glm::vec2 screenPos);
    

    SDL_Window* GetWindowPointer();

    glm::vec2 GetMousePos();

protected:

    friend class Mesh;

private:

    void BeginDrawState();
    void EndDrawState();
    void PostDrawOperations();
    
    
    bool m_IsOpen = true;
    WindowCreationProperties m_Properties;
    //GLFWwindow* m_ContextPointer=nullptr;
    SDL_Window* m_WindowPointer;
    SDL_GLContext m_Context;

    
    entt::entity m_MainCamera = entt::null;
    std::shared_ptr<Framebuffer> m_MainFrameBuffer;
    

    float m_DeltaTime = 0;


    std::vector<std::unique_ptr<VertexArray>> m_CreatedVertexArrays;
    std::map<std::string,std::unique_ptr<Shader>> m_CreatedShaders;

    yael::event_launcher<void(Window&, bool)> m_FocusEventFuncs;
    yael::event_launcher<void(Window&,WindowResizedEventProperties)> m_WindowResizedEventFuncs;
    yael::event_launcher<void(Window&,SDL_MouseMotionEvent)> m_MouseMovedFuncs;
    yael::event_launcher<void(Window&,MouseEventProperties)> m_MouseEnteredWindowFuncs;
    yael::event_launcher<void(Window&,MouseEventProperties)> m_MouseLeftWindowFuncs;
    yael::event_launcher<void(Window&,SDL_MouseButtonEvent)> m_MouseButtonFuncs;
    yael::event_launcher<void(Window&,SDL_MouseWheelEvent)> m_MouseScrollFuncs;
    yael::event_launcher<void(Window&,SDL_KeyboardEvent)> m_KeyEventFuncs;
    yael::event_launcher<void(Window&, SDL_Event&)> m_EventCallback;
    yael::event_launcher<void(Window&)> m_PreDrawingLoopFuncs;
    yael::event_launcher<void(Window&)> m_PostDrawingLoopFuncs;
    yael::event_launcher<void(Window&)> m_ClosingCallbackFuncs;
    Color m_ClearColor = Color::Black;



    //static members

    static Window* m_MainWindow;
    static yael::event_launcher<void(Window&)> m_StartWindowFuncs;
    static inline std::function<WindowSDLInitializerProperties()> m_WindowGeneratorFunc;
   

    friend struct WindowCreators;
    friend struct WindowEvents;


};