#include "window.h"
#include "SiE.h"



yael::event_launcher<void(Window&)> Window::m_StartWindowFuncs;
Window* Window::m_MainWindow = nullptr;

Window::Window(WindowCreationProperties prop) : m_Properties(prop) {

    Window::m_MainWindow = this;
    
    if (!m_WindowGeneratorFunc) {
        SDL_Init(SDL_INIT_VIDEO);

        int contextFlags = 0;
        int windowFlags = SDL_WINDOW_OPENGL;

        if (prop.windowFlags != WindowFlag::None) {
            if (prop.windowFlags & WindowFlag::OpenGLDebugContext) {
                contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
            }
            if (prop.windowFlags & WindowFlag::InitiallyMinimized) {
                windowFlags |= SDL_WINDOW_MINIMIZED;
            }
            else {
                windowFlags |= SDL_WINDOW_MAXIMIZED;
            }

            if (!(prop.windowFlags & WindowFlag::NotResizeable)) {
                windowFlags |= SDL_WINDOW_RESIZABLE;
            }



            if ((prop.openGLVersionMajor == 3 && prop.openGLVersionMinor > 2) || prop.openGLVersionMajor > 3) {
                if (prop.windowFlags & WindowFlag::CoreProfile) {
                    contextFlags |= SDL_GL_CONTEXT_PROFILE_CORE;
                }
                else {
                    contextFlags |= SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
                }
            }
        }


        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, prop.openGLVersionMajor);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, prop.openGLVersionMinor);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, prop.multisamplingCount);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


        if (prop.windowFlags & WindowFlag::FullScreen) {
            windowFlags |= SDL_WINDOW_FULLSCREEN;
        }

        this->m_WindowPointer = SDL_CreateWindow(prop.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            prop.width, prop.height, windowFlags);

        if (!this->m_WindowPointer) {
            DEBUG_ERROR(("Could not create window:" + std::string(SDL_GetError())).c_str());
            return;
        }


        this->m_Context = SDL_GL_CreateContext(m_WindowPointer);

        if (!m_Context) {
            DEBUG_ERROR(("Could not create context:" + std::string(SDL_GetError())).c_str());
            return;
        }
        SDL_GL_MakeCurrent(m_WindowPointer, m_Context);

        SDL_GL_SetSwapInterval(1);

        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            DEBUG_ERROR("Glew wasn't initiated!");
            return;
        }
    }

    

    GameObject mainCamera = GameObject::CreateNew("Main Camera");
    mainCamera.AddComponent<Camera>();
    mainCamera.Transform().SetPosition(0, 2, 10);

    mainCamera.GetComponent<Camera>().SetLookAt(0, 0, 0);

    m_MainCamera = mainCamera.ID();

    this->Events().ClosingEvent().Connect([](Window&) {
        
        
        ecspp::DeleteAllObjects();

        ecspp::Registry().clear();

        });

    this->Events().ResizedEvent().Connect([](Window& win,WindowResizedEventProperties prop){
        win.m_Properties.width = prop.width;
        win.m_Properties.height = prop.height;
        
        if (win.GetCurrentCamera()) {
            win.GetCurrentCamera().GetAsObject().GetComponent<Camera>().SetRenderTarget(std::make_shared<Framebuffer>(prop.width,prop.height));
        }

    });
    
    //GL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
    GL_CALL(glEnable(GL_STENCIL_TEST));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    //GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
    GL_CALL(glStencilMask(0x00));
    

    


    //handle creation events

    Window::m_StartWindowFuncs.EmitEvent(*this);

    

}

Window::~Window() {
    
    
    m_CreatedShaders.clear();
    m_CreatedVertexArrays.clear();

    SDL_DestroyWindow(m_WindowPointer);
}

bool Window::IsOpen() {
    return m_IsOpen;
}

SDL_GLContext& Window::GetContext() {
    return m_Context;
}

void Window::EndDrawState() {
    m_PostDrawingLoopFuncs.EmitEvent(*this);
    
    SDL_GL_SwapWindow(m_WindowPointer);


}

void Window::BeginDrawState() {
    m_PreDrawingLoopFuncs.EmitEvent(*this);

    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
        m_EventCallback.EmitEvent(*this, ev);
        switch (ev.type) {
        case SDL_WINDOWEVENT:
            switch (ev.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                m_ClosingCallbackFuncs.EmitEvent(*this);
                m_IsOpen = false;
                break;
            case SDL_WINDOWEVENT_ENTER:
                m_MouseEnteredWindowFuncs.EmitEvent(*this, {GetMousePos()});
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                m_FocusEventFuncs.EmitEvent(*this, false);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                m_FocusEventFuncs.EmitEvent(*this, true);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                m_MouseLeftWindowFuncs.EmitEvent(*this, { GetMousePos() });
                break;
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                m_FocusEventFuncs.EmitEvent(*this, true);
                break;

            }
            break;
        case SDL_KEYDOWN:
            m_KeyEventFuncs.EmitEvent(*this,SDL_KeyboardEvent(ev.key));
            break;
        case SDL_KEYUP:
            m_KeyEventFuncs.EmitEvent(*this, SDL_KeyboardEvent(ev.key));
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_MouseButtonFuncs.EmitEvent(*this, SDL_MouseButtonEvent(ev.button));
            break;
        case SDL_MOUSEBUTTONUP:
            m_MouseButtonFuncs.EmitEvent(*this, SDL_MouseButtonEvent(ev.button));
            break;
        case SDL_MOUSEMOTION:
            m_MouseMovedFuncs.EmitEvent(*this, SDL_MouseMotionEvent(ev.motion));
            break;
        case SDL_MOUSEWHEEL:
            m_MouseScrollFuncs.EmitEvent(*this, SDL_MouseWheelEvent(ev.wheel));
            break;
        case SDL_DROPFILE:
            break;
        case SDL_DROPTEXT:
            break;
        }
    }

    glm::vec3 color = m_ClearColor.Normalized();
    GL_CALL(glClearColor(color.x,color.y,color.z,1.0f));
    GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
    GL_CALL(glStencilMask(0x00));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


}



void Window::SetClearColor(Color color) {
    m_ClearColor = color;
}

const WindowCreationProperties& Window::Properties() const {
    return m_Properties;
}


void Window::SetCamera(GameObject obj) {
    if(obj.HasComponent<Camera>()){
        m_MainCamera = obj.ID();
    }
}



void Window::SetViewPort(int x, int y, int width, int height)
{
    if (GetCurrentCamera()) {
        m_Properties.width = width - x;
        m_Properties.height = height - y;
        //glm::vec4 viewport = GetCurrentCamera().GetAsObject().GetComponent<Camera>().GetViewPort();
        //GL_CALL(glViewport(x,y,width,height));
    }
}



ecspp::NamedObjectHandle<GameObject> Window::GetCurrentCamera() {
    return ecspp::NamedObjectHandle<GameObject>(m_MainCamera);
}



WindowEvents Window::Events() {
    return WindowEvents(*this);
}

void Window::DrawFrame() {
    static float currentTime=0,oldTime=0;

    SDL_GL_MakeCurrent(m_WindowPointer,m_Context);

      
    BeginDrawState();
        
    currentTime = SDL_GetTicks();
    m_DeltaTime = static_cast<float>(currentTime - oldTime)/1000;

    oldTime = currentTime;

    GameObject::ForEach([&](GameObject obj) {
        for (auto& name : obj.GetComponentsNames()) {
            if (auto comp = obj.GetComponentByName(name); comp) {
                //comp.GetAs<GameComponent>()->Update(m_DeltaTime);
            }
        }
    });

    Camera::ForEach([](Camera& camera) {
        if (!camera.GetMasterObject().GetAsObject().IsActive()) {
            return;
        }
        if (!camera.IsEnabled()) {
            return;
        }
        if (!camera.HasRenderTarget() && camera.GetMasterObject() != Window::GetCurrentWindow().GetCurrentCamera()) {
            return;
        }

        camera.Render();
    });

    EndDrawState();

    PostDrawOperations();
        
    
    


    
}





VertexArray& WindowCreators::NewVertexArray() {
    VertexArray& vertex = *m_Master.m_CreatedVertexArrays.emplace_back(std::make_unique<VertexArray>()).get();
    return vertex;
}

Shader& WindowCreators::CachedShader(std::string shaderRelativePath, bool* loadResult) {
    if(m_Master.m_CreatedShaders.find(shaderRelativePath) != m_Master.m_CreatedShaders.end()){
        Shader& shader = *m_Master.m_CreatedShaders[shaderRelativePath].get();
        if(loadResult){
            *loadResult=true;
        }
        return shader;
    }
    
    m_Master.m_CreatedShaders[shaderRelativePath] = std::make_unique<Shader>();
    Shader& shader = *m_Master.m_CreatedShaders[shaderRelativePath].get();

    if(!std::filesystem::exists(shaderRelativePath)){
        LOG("Could not load shader at path " + std::filesystem::absolute(shaderRelativePath).string() + " passing empty shader");
        if(loadResult){
            *loadResult=false;
        }
        return shader;
    }

    std::vector<std::pair<ShaderType,std::string>> sources;
    for(auto file : std::filesystem::directory_iterator(shaderRelativePath)){
        std::string fileName = file.path().filename().string();

        if(file.path().extension().string() == ".vert"){
            std::string source = LoadFileContents(std::filesystem::absolute(shaderRelativePath + "/" + fileName).string());
            sources.push_back(std::make_pair(ShaderType::Vertex,source));
        }
        if(file.path().extension().string() == ".frag"){
            std::string source = LoadFileContents(std::filesystem::absolute(shaderRelativePath + "/" + fileName).string());
            sources.push_back(std::make_pair(ShaderType::Fragment,source));
        }
    }
    

    ShaderCreationProperties prop = m_Master.m_CreatedShaders[shaderRelativePath].get()->CreateNew();
    for(auto& [type,source] : sources){
        prop.AddShader(type,source);
    }

    if(!prop.Generate()){
        if(loadResult){
            *loadResult=false;
        }
        return shader;
    }

    if(loadResult){
        *loadResult=true;
    }
    return shader;

}

WindowCreators Window::Create() {
    return WindowCreators(*this);
}





yael::event_sink<void(Window&,MouseEventProperties)> WindowEvents::MouseLeftWindowEvent() {
    return { m_Master.m_MouseLeftWindowFuncs };
}



yael::event_sink<void(Window&,MouseEventProperties)> WindowEvents::MouseEnteredWindowEvent() {
    return { m_Master.m_MouseEnteredWindowFuncs };
}

yael::event_sink<void(Window&,SDL_MouseButtonEvent)> WindowEvents::MouseButtonEvent() {
    return { m_Master.m_MouseButtonFuncs };
}

yael::event_sink<void(Window&, bool)> WindowEvents::FocusEvent()
{
    return { m_Master.m_FocusEventFuncs };
}

yael::event_sink<void(Window&,SDL_MouseWheelEvent)> WindowEvents::MouseScrollEvent() {
    return { m_Master.m_MouseScrollFuncs };
}

yael::event_sink<void(Window&,WindowResizedEventProperties)> WindowEvents::ResizedEvent() {
    return { m_Master.m_WindowResizedEventFuncs };
}

yael::event_sink<void(Window&, SDL_Event&)> WindowEvents::AllEvents()
{
    return {m_Master.m_EventCallback};
}

yael::event_sink<void(Window&,SDL_KeyboardEvent)> WindowEvents::KeyEvent() {
    return { m_Master.m_KeyEventFuncs };
}

yael::event_sink<void(Window&, SDL_MouseMotionEvent)> WindowEvents::MouseMovedEvent()
{
    return { m_Master.m_MouseMovedFuncs };
}

yael::event_sink<void(Window&)> Window::WindowCreationEvent() {
    return yael::event_sink<void(Window&)>(Window::m_StartWindowFuncs);
}

yael::event_sink<void(Window&)> WindowEvents::PreDrawingLoopEvent() {
    return yael::event_sink<void(Window&)>(m_Master.m_PreDrawingLoopFuncs);
}

yael::event_sink<void(Window&)> WindowEvents::PostDrawingLoopEvent() {
    return yael::event_sink<void(Window&)>(m_Master.m_PostDrawingLoopFuncs);
}
yael::event_sink<void(Window&)> WindowEvents::ClosingEvent() {
    return yael::event_sink<void(Window&)>(m_Master.m_ClosingCallbackFuncs);
}


SDL_Window* Window::GetWindowPointer()
{
    return m_WindowPointer;
}

glm::vec2 Window::GetMousePos()
{
    glm::ivec2 vec;
    SDL_GetMouseState(&vec.x, &vec.y);
    return glm::vec2(vec);
}

float Window::GetDeltaTime() {
    return m_DeltaTime;
}
Window& Window::GetCurrentWindow() {
    return *Window::m_MainWindow;
}

void Window::DisableCamera() {
    m_MainCamera = entt::null;
}

void Window::PostDrawOperations() {
    

    if(!m_IsOpen){
        m_ClosingCallbackFuncs.EmitEvent(*this);
    }

    ecspp::ClearDeletingQueue();
}

void Window::SetSDLCreationFunc(std::function<WindowSDLInitializerProperties()> func) {
    Window::m_WindowGeneratorFunc = func;
}

RayCastHit Window::RayCast(glm::vec2 screenPos)
{
    return RayCastHit();
}
