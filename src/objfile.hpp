//
//  objfile.hpp
//  CGRA_PROJECT_base
//
//  Created by Linda Zhang on 14/07/22.
//

//#ifndef objfile_hpp
//#define objfile_hpp

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "opengl.hpp"

using namespace std;
using namespace glm;

class ObjFile{
private:
    std::vector<glm::vec3> temp_positions; //temp vectors
    std::vector<glm::vec2> temp_uv;
    std::vector<glm::vec3> temp_normals;
    
    std::vector<glm::vec3> positions;    //3d position coordinates (p)
    std::vector<glm::vec2> uv;           //texture coords (t)
    std::vector<glm::vec3> normals;      //normal, direction of face (n)
    
    std::vector<unsigned int> positionIndices, uvIndices, normalIndices;
    std::vector<unsigned int> indices;   //faces in p/t/n format (stores vertex in groups of 3)
    
    // GPU-side data
    GLuint m_vao = 0;
    GLuint m_vbo_pos = 0;
    GLuint m_vbo_norm = 0;
    GLuint m_ibo = 0;

public:
    /*
     loads an obj file and populates the fields
     @param filename the name of the file specified in the ImGui input controls
     */
    void loadObj(string filename);
    
    //set up, draw, and clear mesh geometry data buffers in OpenGL
    void build();
    void draw();
    void destroy();
    
    /*
     prints the mesh data in the console for debugging purposes
     */
    void printMeshData();
};

//#endif /* objfile_hpp */
