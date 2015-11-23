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
    
    void createBuffer(Vertex *pVerts, int numVerts, int *pIndices, int numOfIndices);
    void loadShader(const string& vsFilename, const string& fsFilename);
    
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
};


#endif /* GameObject_h */
