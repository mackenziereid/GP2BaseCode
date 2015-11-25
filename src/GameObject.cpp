#include "GameObject.h"
#include "Shader.h"

GameObject::GameObject()
{
    m_VAO = 0;
    m_VBO = 0;
    m_EBO = 0;
    m_ShaderProgram = 0;
    
    m_NoOfIndices = 0;
    m_noOfVertex = 0;
    
    m_ModelMatrix = mat4(1.0f);
    
    m_Position = vec3(0.0f, 0.0f, 0.0f);
    m_Rotation = vec3(0.0f, 0.0f, 0.0f);
    m_Scale = vec3(1.0f, 1.0f, 1.0f);
    
    m_ParentGameObjects = NULL;
    m_ChildGameObjects.clear();
}

GameObject::~GameObject()
{
    glDeleteProgram(m_ShaderProgram);
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void GameObject::update()
{
    mat4 parentModel = mat4(1.0f);
    if (m_ParentGameObjects) {
        parentModel = m_ParentGameObjects->getModelMatrix();
    }
    
    mat4 translationMatrix = translate(mat4(1.0f), m_Position);
    mat4 scaleMatrix = scale(mat4(1.0f), m_Scale);
    mat4 rotationMatrix = rotate(mat4(1.0f), m_Rotation.x, vec3(1.0f, 0.0f, 0.0f))*
                          rotate(mat4(1.0f), m_Rotation.y, vec3(0.0f, 1.0f, 0.0f))*
                          rotate(mat4(1.0f), m_Rotation.z, vec3(0.0f, 0.0f, 1.0f));
    
    m_ModelMatrix = scaleMatrix*rotationMatrix*translationMatrix;
    m_ModelMatrix = m_ModelMatrix*parentModel;
    
    for (auto iter = m_ChildGameObjects.begin(); iter != m_ChildGameObjects.end(); iter++) {
        (*iter)->update();
    }
}

void GameObject::createBuffer(Vertex *pVerts, int numVerts, int *pIndices, int numOfIndices)
{
    m_NoOfIndices = numOfIndices;
    m_noOfVertex = numVerts;
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    glBufferData(GL_ARRAY_BUFFER, numVerts*sizeof(Vertex), pVerts, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices*sizeof(int), pIndices, GL_STATIC_DRAW);
    
    //Tell the shader that 0 is the position element
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) + sizeof(vec4)));
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) + sizeof(vec4) + sizeof(vec2)));
    
    
}

void GameObject::loadShader(const string& vsFilename, const string& fsFilename)
{
    GLuint vertexShaderProgram = loadShaderFromFile(vsFilename, VERTEX_SHADER);
    checkForCompilerErrors(vertexShaderProgram);
    
    GLuint fragmentShaderProgram = loadShaderFromFile(fsFilename, FRAGMENT_SHADER);
    checkForCompilerErrors(fragmentShaderProgram);
    
    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShaderProgram);
    glAttachShader(m_ShaderProgram, fragmentShaderProgram);
    
    //Link attributes
    glBindAttribLocation(m_ShaderProgram, 0, "vertexPosition");
    glBindAttribLocation(m_ShaderProgram, 1, "vertexColour");
    glBindAttribLocation(m_ShaderProgram, 2, "vertexTexCoords");
    glBindAttribLocation(m_ShaderProgram, 3, "vertexNormal");
    
    glLinkProgram(m_ShaderProgram);
    checkForLinkErrors(m_ShaderProgram);
    //now we can delete the VS & FS Programs
    glDeleteShader(vertexShaderProgram);
    glDeleteShader(fragmentShaderProgram);

}