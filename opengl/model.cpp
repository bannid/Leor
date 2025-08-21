gl_model QuadModelBottomLeftAligned()
{
    
    f32 z = 0.0f;
    f32 e = 1.0f;
    f32 Vertices[] = {
        0, e, z, 0.0f, 1.0f, // top left
        e, 0, z, 1.0f, 0.0f, // bottom right
        e, e, z, 1.0f, 1.0f, //top right
        
        0, e, z, 0.0f, 1.0f, // top left
        0, 0, z, 0.0f, 0.0f, // bottom left
        e, 0, z, 1.0f, 0.0f  //bottom right
    };
    
    u32 Stride = 5;
    u32 VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    gl_model Model = {};
    Model.VaoID = VAO;
    Model.VboID = VBO;
    Model.VertCount = 6;
    return Model;
}

gl_model QuadModelCenterAligned()
{
    f32 z = 0.0f;
    f32 e = .5f;
    
    f32 Vertices[] = {
        -e, e, z, 0.0f, 1.0f, // top left
        e, -e, z, 1.0f, 0.0f, // bottom right
        e, e, z, 1.0f, 1.0f, //top right
        
        -e, e, z, 0.0f, 1.0f, // top left
        -e, -e, z, 0.0f, 0.0f, // bottom left
        e, -e, z, 1.0f, 0.0f  //bottom right
    };
    
    u32 Stride = 5;
    u32 VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Stride * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    gl_model Model = {};
    Model.VaoID = VAO;
    Model.VboID = VBO;
    Model.VertCount = 6;
    return Model;
}

gl_model LoadQuadModel(b32 CenterAligned)
{
    if(CenterAligned) return QuadModelCenterAligned();
    return QuadModelBottomLeftAligned();
}