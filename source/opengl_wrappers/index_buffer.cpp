#include "index_buffer.h"
#include "vertex_array.h"

IndexBuffer::IndexBuffer(VertexArray& master) : m_Master(master) {
    auto destructor = [](unsigned int* id){
        SIE_GL_CALL(glDeleteBuffers(1,id));
        delete id;
    };
    m_ID = std::shared_ptr<unsigned int>(new unsigned int(0),destructor);
    SIE_GL_CALL(glGenBuffers(1,m_ID.get()));
}

void IndexBuffer::SetIndices(std::vector<unsigned int> indices) {
    this->Bind();
    SIE_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),indices.data(),GL_STATIC_DRAW));
    this->Unbind();

    m_Count = indices.size();
}


void IndexBuffer::Bind() {
    SIE_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,*m_ID.get()));
}

void IndexBuffer::Unbind() {
    SIE_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
}
