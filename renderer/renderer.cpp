b32 
InitializeRenderer(renderer* Renderer,
                   int32 Width, int32 Height,
                   const char* WindowTitle)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    Renderer->Window =glfwCreateWindow(Width,
                                       Height,
                                       WindowTitle,
                                       NULL,
                                       NULL);
    if(Renderer->Window == NULL) return false;
    glfwMakeContextCurrent(Renderer->Window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    
    // NOTE(Banni): GL Flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return true;
}

b32
RendererRunning(renderer* Renderer)
{
    return !glfwWindowShouldClose(Renderer->Window);
}

void 
DrawScene(renderer* Renderer, scene* Scene)
{
    glClearColor(.0, .0, .0, .0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
}
