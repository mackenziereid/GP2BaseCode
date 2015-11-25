#ifndef GameObject_h
#define GameObject_h

#include "Common.h"
#include "Vertices.h"

class GameObject
{
public:
    GameObject();
    ~GameObject();
    
    void update();
    
    void addChild(shared_ptr<GameObject> child);
    
    void createBuffer(Vertex *pVerts, int numVerts, int *pIndices, int numOfIndices);
    void loadShader(const string& vsFilename, const string& fsFilename);
    
    GLuint getShaderProgram()
    {
        return m_ShaderProgram;
    };
    
    GLuint getVertexArrayObject()
    {
        return m_VAO;
    };
    
    GLuint getNumberOfIndices()
    {
        return m_NoOfIndices;
    };
    
    mat4& getModelMatrix()
    {
         return m_ModelMatrix;
    };
    
    void setPosition(const vec3& position)
    {
        m_Position = position;
    };
    
    void setRotation(const vec3& rotation)
    {
        m_Rotation = rotation;
    };
    
    void setScale(const vec3& scale)
    {
        m_Scale = scale;
    };
    
    GameObject* getParent()
    {
        return m_ParentGameObjects;
    };
    
    int getNumberOfChildren()
    {
        return m_ChildGameObjects.size();
    };
    
    shared_ptr<GameObject> getChild(int i)
    {
        return m_ChildGameObjects.at(i);
    };
    
private:
    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_EBO;
    GLuint m_ShaderProgram;
    int m_NoOfIndices;
    int m_noOfVertex;
    mat4 m_ModelMatrix;
    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;
    
    vector<shared_ptr<GameObject>> m_ChildGameObjects;
    GameObject * m_ParentGameObjects;
    
};


#endif /* GameObject_h */
