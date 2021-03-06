#include "vertex_array.h"

VertexArray::VertexArray(){
    auto deleter = [](unsigned int* id){
        SIE_GL_CALL(glDeleteVertexArrays(1,id));
    };
    m_ID = std::shared_ptr<unsigned int>(new unsigned int,deleter);
    SIE_GL_CALL(glGenVertexArrays(1,m_ID.get()));
}
VertexBuffer& VertexArray::CreateVertexBuffer(unsigned int numberOfVertices) {
    m_VBO = std::make_unique<VertexBuffer>(*this);
    m_VBO.get()->CreateNew(numberOfVertices);
    return *m_VBO.get();
}

VertexBuffer& VertexArray::CreateVertexBuffer() {
    m_VBO = std::make_unique<VertexBuffer>(*this);
    return *m_VBO.get();
}

void VertexArray::Bind() {
    SIE_GL_CALL(glBindVertexArray(*m_ID.get()));
    if(m_VBO){
        m_VBO.get()->Bind();
    }
    if(m_IB){
        m_IB.get()->Bind();
    }
}

void VertexArray::Unbind() {
    SIE_GL_CALL(glBindVertexArray(0));
    if(m_VBO){
        m_VBO.get()->Unbind();
    }
    if(m_IB){
        m_IB.get()->Unbind();
    }
}

IndexBuffer& VertexArray::CreateIndexBuffer() {
    m_IB = std::make_unique<IndexBuffer>(*this);
    return *m_IB.get();
}
bool VertexArray::HasIndexBuffer() {
    return m_IB.operator bool();
}

unsigned int VertexArray::GetDrawCount() {
    if(this->HasIndexBuffer()){
        return m_IB.get()->m_Count;
    }
    else {
        return m_VBO.get()->numberOfElements;
    }
}

void VertexArray::Reset() {
    m_IB.reset();
    m_VBO.reset();
}

int VertexArray::GetVBOID() {
    if(m_VBO){
        return *m_VBO.get()->m_ID.get();
    }
    else{
        return -1;
    }
}
