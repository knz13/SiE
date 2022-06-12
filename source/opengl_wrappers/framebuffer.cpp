#include "framebuffer.h"


void Framebuffer::Bind() {
    if(m_ID){
        SIE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER,*m_ID.get()));
        if(m_AttachedTexture){
            m_AttachedTexture.get()->Bind();
        }
    }
}

void Framebuffer::Unbind() {
    SIE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER,0));
    if(m_AttachedTexture){
            m_AttachedTexture.get()->Unbind();
    }
}

Framebuffer::Framebuffer(float sizeX,float sizeY) : m_Size(sizeX,sizeY){
    if(!m_ID){
        auto deleter = [](unsigned int* id){
            SIE_GL_CALL(glDeleteFramebuffers(1,id));
        };
        m_ID = std::shared_ptr<unsigned int>(new unsigned int,deleter);
        SIE_GL_CALL(glGenFramebuffers(1,m_ID.get()));
        
        this->Bind();
        
        m_AttachedTexture = std::shared_ptr<Texture<Type2D>>(new Texture<Type2D>([=](Texture<Type2D>& tex){
            tex.Bind();

            SIE_GL_CALL(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, sizeX, sizeY));

            SIE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            SIE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));  


            tex.Unbind();
        }));


        SIE_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, m_AttachedTexture.get()->GetID(), 0));
        
        
        if(!m_RenderBufferID){
            auto renderBufferDeleter = [](unsigned int* id){
                SIE_GL_CALL(glDeleteRenderbuffers(1,id));
            };

            m_RenderBufferID = std::shared_ptr<unsigned int>(new unsigned int,renderBufferDeleter);
            SIE_GL_CALL(glGenRenderbuffers(1,m_RenderBufferID.get()));
            SIE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER,*m_RenderBufferID.get()));
            SIE_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY));

            SIE_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *m_RenderBufferID.get()));  
            SIE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER,0));
        }
        
        
        
        
        assert(this->Status());
        
        this->Unbind();

    }    
}

bool Framebuffer::Status() {
    this->Bind();
    bool state;
    SIE_GL_CALL_WITH_RESULT(state,glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    this->Unbind();
    return state;

}

void Framebuffer::Clear(Color color)
{
    this->Bind();
    glm::vec3 normColor = color.Normalized();
    SIE_GL_CALL(glClearColor(normColor.x,normColor.y,normColor.z,1.0f));
    SIE_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    this->Unbind();
}

const glm::vec2& Framebuffer::GetSize() {
    return m_Size;    
}

Texture<Type2D>& Framebuffer::GetAttachedTexture() {
    return *m_AttachedTexture.get();
}
