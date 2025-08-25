// NOTE(Banni): Compiles, links and loads the shader to GPU
shader_program LoadShader(char* VsSource, char* FsSource)
{
    shader_program Shader = {};
    for(;;)
    {
        int32 Success = 0;
        char ErrorLog[512];
        int32 VertexShader, FragmentShader, Program;
        // Compile the vertex shader
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, (const char**)&VsSource, NULL);
        glCompileShader(VertexShader);
        glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
        
        if(!Success)
        {
            glGetShaderInfoLog(VertexShader,
                               512,
                               NULL,
                               ErrorLog);
            DEBUG_PRINT(ErrorLog);
            break;
        }
        
        // Compile the fragment shader
        FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShader, 1, (const char**)&FsSource, NULL);
        glCompileShader(FragmentShader);
        glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
        
        if(!Success)
        {
            glGetShaderInfoLog(FragmentShader,
                               512,
                               NULL,
                               ErrorLog);
            DEBUG_PRINT(ErrorLog);
            break;
        }
        Program = glCreateProgram();
        glAttachShader(Program, VertexShader);
        glAttachShader(Program, FragmentShader);
        glLinkProgram(Program);
        glGetProgramiv(Program, GL_LINK_STATUS, &Success);
        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        if(!Success)
        {
            glGetProgramInfoLog(Program, 512, NULL, ErrorLog);
            break;
        }
        Shader.Valid = true;
        Shader.ID = Program;
        glUseProgram(Shader.ID);
        Shader.ProjectionLocation = glGetUniformLocation(Shader.ID, "uProjection");
        Shader.ModelLocation = glGetUniformLocation(Shader.ID, "uModel");
        Shader.ColourLocation = glGetUniformLocation(Shader.ID, "uColour");
        break;
    }
    return Shader;
}

UsesScratchArena shader_program
LoadShaderFromFile(const char* VsFilePath,
                   const char* FsFilePath,
                   memory_arena Arena)
{
    ZeroMemory(Arena.BasePointer, Arena.Size);
    u8* BufferA = (u8*)GetMemory(&Arena, KILOBYTE(512));
    u8* BufferB = (u8*)GetMemory(&Arena, KILOBYTE(512));
    b32 VsFileValid = Win32ReadEntireFileV2(VsFilePath, BufferA, KILOBYTE(512));
    b32 FsFileValid = Win32ReadEntireFileV2(FsFilePath, BufferB, KILOBYTE(512));
    if(VsFileValid && FsFileValid)
    {
        return LoadShader((char*)BufferA, (char*)BufferB);
    }
    
    return {};
}

inline void
SetUniformBool(u32 ShaderProgramID, b32 Value, const char* UniformName)
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, UniformName), (int32)Value);
}