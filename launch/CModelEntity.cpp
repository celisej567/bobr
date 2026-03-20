#include "shared.h"
#include "CShader.h"
#include "CModelEntity.h"

LINK_CLASSNAME_TO_ENTITY(model_entity, CModelEntity);

CModelEntity::CModelEntity()
{
    
}

CModelEntity::~CModelEntity()
{

}

void CModelEntity::Think()
{
    BaseClass::Think();
}

void CModelEntity::SetModelName( std::string filename )
{
    m_Model.ProcessFile(filename);
}

void CModelEntity::SetTextureName( std::string filename, GLenum textureType )
{
    m_Texture.ProcessFile(filename.data(), textureType);
}

void CModelEntity::Frame()
{

    g_ShaderIDMap["default_new"]->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glBindVertexArray(m_Model.VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, GetAbsPos() );

    model = glm::rotate(model, glm::radians(GetAbsRot()[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(GetAbsRot()[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(GetAbsRot()[2]), glm::vec3(0.0f, 0.0f, 1.0f));

    GLfloat* model_p = glm::value_ptr(model);
    GLfloat* view_p = glm::value_ptr(view);
    GLfloat* projection_p = glm::value_ptr(projection);

    g_ShaderIDMap["default_new"]->SetUniformMat4("model", model_p);
    g_ShaderIDMap["default_new"]->SetUniformMat4("view", view_p);
    g_ShaderIDMap["default_new"]->SetUniformMat4("proj", projection_p);

    glDrawElements(GL_TRIANGLES, m_Model.GetIndexesCount(), GL_UNSIGNED_INT, 0);
}
