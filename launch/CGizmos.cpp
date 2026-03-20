#include "shared.h"
#include "CGizmos.h"
#include "CModel.h"
#include "CShader.h"
#include "glm.hpp"

vertex_t LineVertexPos[] = 
{
    {0,0,0,1,1,1,0,0},
    {1,0,0,1,1,1,0,0},
};

LINK_CLASSNAME_TO_ENTITY(gizmos,CGizmos);

CGizmos::CGizmos()
{
    uint VBO; 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * 2, LineVertexPos, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

CGizmos::~CGizmos()
{

}

void CGizmos::Frame()
{
    
    g_ShaderIDMap["color"]->Use();

    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0,0,0) );

    GLfloat* model_p = glm::value_ptr(model);
    GLfloat* view_p = glm::value_ptr(view);
    GLfloat* projection_p = glm::value_ptr(projection);

    g_ShaderIDMap["color"]->SetUniformMat4("model", model_p);
    g_ShaderIDMap["color"]->SetUniformMat4("view", view_p);
    g_ShaderIDMap["color"]->SetUniformMat4("proj", projection_p);
    g_ShaderIDMap["color"]->SetUniformVec3("vecColor", 1,0,0);

    glDrawArrays(GL_LINES, 0, 2);


    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    g_ShaderIDMap["color"]->SetUniformMat4("model", model_p);
    g_ShaderIDMap["color"]->SetUniformVec3("vecColor", 0,1,0);
    glDrawArrays(GL_LINES, 0, 2);


    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    g_ShaderIDMap["color"]->SetUniformMat4("model", model_p);
    g_ShaderIDMap["color"]->SetUniformVec3("vecColor", 0,0,1);
    glDrawArrays(GL_LINES, 0, 2);
    
}