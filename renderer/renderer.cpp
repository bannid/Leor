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
DrawScene(renderer* Renderer,
          scene* Scene,
          shader_program* Shader,
          glm::mat4* Projection,
          leor_model_list* Models)
{
    glClearColor(.0, .0, .0, .0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 ViewMat = GetViewMatrix(&Scene->ThirdPersonCamera);
    glUseProgram(Shader->ID);
    glUniformMatrix4fv(glGetUniformLocation(Shader->ID, "uProjection"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(*Projection));
    glUniformMatrix4fv(glGetUniformLocation(Shader->ID, "uView"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(ViewMat));     
    for(int32 j = 0; j < Scene->Entites.Length; j++)
    {
        
        entity* Entity = GetItemPointer(&Scene->Entites, j);
        glm::mat4 ModelMat = TransformToMat4(&Entity->Transform);
        glUniformMatrix4fv(glGetUniformLocation(Shader->ID, "uModel"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(ModelMat));
        leor_model* Model = GetItemPointer(Models, Entity->ModelIndex);
        for(int32 i = 0; i < Model->Meshes.Length; i++)
        {
            leor_mesh* Mesh = GetItemPointer(&Model->Meshes, i);

            // NOTE(Banni): Attach the texture if any
            if(*Mesh->DiffuseTexture != '\0')
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, Mesh->DiffuseTextureID);
                SetUniformBool(Shader->ID, true, "uUseTexture");
            }
            else
            {
                SetUniformBool(Shader->ID, false, "uUseTexture");
            }
            glBindVertexArray(Mesh->GPUId);
            glDrawArrays(GL_TRIANGLES, 0, Mesh->Vertices.Length);
        }
    }
    
}
