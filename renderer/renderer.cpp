UsesScratchArena b32 InitializeRenderer(renderer *Renderer,
                                        int32 Width, int32 Height,
                                        const char *WindowTitle,
                                        memory_arena ScratchArena)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    Renderer->Window = glfwCreateWindow(Width,
                                        Height,
                                        WindowTitle,
                                        NULL,
                                        NULL);
    if (Renderer->Window == NULL)
        return false;
    glfwMakeContextCurrent(Renderer->Window);
    
    Renderer->Height = Height;
    Renderer->Width = Width;
    
    // glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    
    // NOTE(Banni): GL Flags
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // NOTE(Banni): Load the fonts
    Renderer->Fonts = FreeTypeLoadFontsFromFile("../assets/arial.ttf", 16.0f, ScratchArena);
    
    // NOTE(Banni): Generated from https://tools.irvantaufik.me/newline-to-escape/
    const char* VsSource = "#version 330\n\nlayout (location = 0) in vec3 aPos;\nlayout (location = 1) in vec2 aTex;\n\nuniform mat4 uProjection;\n\nout vec2 fTex;\n\nvoid main()\n{\n    gl_Position = uProjection * vec4(aPos, 1.0f);\n    fTex = aTex;\n}";
    const char* FsSource = "#version 330\n\nin vec2 fTex;\n\nout vec4 finalColor;\n\nuniform vec4 uColour;\nuniform sampler2D uTexture;\n\nvoid main()\n{\n    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture, fTex).r);\n    finalColor = uColour * sampled;\n}";
    
    
    Renderer->FontShader = LoadShader((char*)VsSource, (char*)FsSource);
    Renderer->ScreenProjection = glm::ortho(0.0f, (f32)Width,
                                            0.0f, (f32)Height,
                                            0.0f, 10.0f);
    Renderer->FontModel = DynamicVaoForFontRendering();
    return true;
}

void DrawText(renderer *Renderer, glm::vec2 Position, const char* Text, v4 Colour)
{
    ASSERT_DEBUG(Renderer->FontShader.Valid);
    
    f32 X = Position.x;
    f32 Y = Position.y;
    
    glUseProgram(Renderer->FontShader.ID);
    glBindVertexArray(Renderer->FontModel.VaoID);
    glUniformMatrix4fv(Renderer->FontShader.ProjectionLocation,
                       1,
                       GL_FALSE,
                       glm::value_ptr(Renderer->ScreenProjection));
    
    glUniform1i(glGetUniformLocation(Renderer->FontShader.ID, "uTexture"), 0);
    glUniform4fv(Renderer->FontShader.ColourLocation,
                 1,
                 (f32*)&Colour);
    glActiveTexture(GL_TEXTURE0);
    
    f32 Size = 16.0f;
    
    f32 ScaleX = ((f32)Size / Renderer->Fonts.PixelHeight);
    f32 ScaleY = ((f32)Size / Renderer->Fonts.PixelHeight);
    
    while(*Text != 0)
    {
        char C = *Text;
        u32 Index = (u32)C;
        font_character Ch = Renderer->Fonts.Characters[Index];
        f32 XPos = X + Ch.Bearing.x * ScaleX;
        f32 YPos = Y - (Ch.Size.y - Ch.Bearing.y) * ScaleY;
        
        f32 W = Ch.Size.x * ScaleX;
        f32 H = Ch.Size.y * ScaleY;
        // update VBO for each character
        f32 Vertices[6][5] = {
            { XPos,     YPos + H,0.0f,   0.0f, 0.0f },            
            { XPos,     YPos,    0.0f,   0.0f, 1.0f },
            { XPos + W, YPos,    0.0f,   1.0f, 1.0f },
            
            { XPos,     YPos + H,0.0f,   0.0f, 0.0f },
            { XPos + W, YPos,    0.0f,   1.0f, 1.0f },
            { XPos + W, YPos + H,0.0f,   1.0f, 0.0f }           
        };
        
        // NOTE(Banni): Bind the texture
        glBindTexture(GL_TEXTURE_2D, Ch.TextureID);
        
        // NOTE(Banni): Update the buffer data
        glBindBuffer(GL_ARRAY_BUFFER, Renderer->FontModel.VboID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices); // be sure to use 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        X += (Ch.Advance >> 6) * ScaleX; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))s
        
        //Move to next character
        Text++;
    }
    
    // NOTE(Banni): Unbind gl state
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

b32 RendererRunning(renderer *Renderer)
{
    return !glfwWindowShouldClose(Renderer->Window);
}

void DrawScene(renderer *Renderer,
               scene *Scene,
               shader_program *Shader,
               glm::mat4 *Projection,
               leor_model_list *Models)
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
    for (int32 j = 0; j < Scene->Entites.Length; j++)
    {
        
        entity *Entity = GetItemPointer(&Scene->Entites, j);
        glm::mat4 ModelMat = TransformToMat4(&Entity->Transform);
        glUniformMatrix4fv(glGetUniformLocation(Shader->ID, "uModel"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(ModelMat));
        leor_model *Model = GetItemPointer(Models, Entity->ModelIndex);
        for (int32 i = 0; i < Model->Meshes.Length; i++)
        {
            leor_mesh *Mesh = GetItemPointer(&Model->Meshes, i);
            
            // NOTE(Banni): Attach the texture if any
            if (*Mesh->DiffuseTexture != '\0')
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

void DrawCollisionMesh(renderer *Renderer,
                       shader_program *Shader,
                       leor_physics_world *World,
                       glm::mat4 *ViewMat,
                       glm::mat4 *Projection)
{
    glUseProgram(Shader->ID);
    glUniformMatrix4fv(glGetUniformLocation(Shader->ID, "uProjection"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(*Projection));
    glUniformMatrix4fv(glGetUniformLocation(Shader->ID, "uView"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(*ViewMat));
    glBindVertexArray(World->GPUHandle);
    glDrawArrays(GL_TRIANGLES, 0, World->CollisionMesh.Length * 3);
}
