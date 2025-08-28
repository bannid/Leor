framebuffer CreateFrameBuffer(u32 Width, u32 Height)
{
    u32 FrameBuffer;
    glGenFramebuffers(1, &FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
    
    u32 ColourBufferID;
    glGenTextures(1, &ColourBufferID);
    glBindTexture(GL_TEXTURE_2D, ColourBufferID);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 Width,
                 Height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Note(Banni): Attach it to currently bound FrameBuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           ColourBufferID, 0);
    
    u32 RenderBufferID;
    glGenRenderbuffers(1, &RenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferID); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferID);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ASSERT_DEBUG(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    framebuffer Result;
    Result.ID = FrameBuffer;
    Result.TextureID = ColourBufferID;
    Result.Width = Width;
    Result.Height = Height;
    return Result;
}
