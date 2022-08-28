//
//  objfile.cpp
//  CGRA_PROJECT_base
//
//  Created by Linda Zhang on 14/07/22.
//

#include "objfile.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "opengl.hpp"
#include <fstream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


using namespace std;
using namespace glm;

/*
 loads an obj file and populates the fields
 @param filename the name of the file specified in the ImGui input controls
 */
void ObjFile::loadObj(string filename){
    destroy(); //unload everything first
    
    ifstream file;
    file.open(filename, ios::in); //if teapot.obj fails to open, please set custom work directory to the location of the file
    
    if(!file.is_open()){
        cout<<"file "<<filename<<" failed to open."<<endl;
        file.close();
    }
    
    //open successful, parse the file
    string token;
    int count = 0;
    
    //read tokens separated by space
    while(getline(file,token,' ')){
        count++;
        
        //parse position
        if(token == "v"){
            string token1, token2, token3;
            getline(file,token1,' ');
            getline(file,token2,' ');
            getline(file,token3,'\n');
            
            temp_positions.push_back(glm::vec3((float)stod(token1), (float)stod(token2), (float)stod(token3)));
        }
        //parse texture (ommitted in rendering)
        else if(token == "vt"){
            string token1, token2;
            getline(file,token1,' ');
            getline(file,token2,'\n');
            
            temp_uv.push_back(glm::vec2((float)stod(token1), (float)stod(token2)));
        }
        //parse normal
        else if(token == "vn"){
            string token1, token2, token3;
            getline(file,token1,' ');
            getline(file,token2,' ');
            getline(file,token3,'\n');
            
            temp_normals.push_back(glm::vec3((float)stod(token1), (float)stod(token2), (float)stod(token3)));
        }
        //parse face
        else if(token == "f"){
            for(int i=0; i<3; i++){
                string token1, token2, token3;
                getline(file,token1,'/');
                getline(file,token2,'/');
                if(i == 2){ //last vertex in the triangle
                    getline(file,token3,'\n');
                } else{
                    getline(file,token3,' ');
                }
                
                positionIndices.push_back(stoi(token1));
                uvIndices.push_back(stoi(token2));
                normalIndices.push_back(stoi(token3));
            }
        }
        //other - skip to next line
        else{
            cout<<"ignore line: "<<token<<" at line "<<count<<endl;
            file.ignore(numeric_limits<streamsize>::max(),'\n');
        }
    }
    
    //re-order the vertex attributes to suit OpenGL
    int countIndex = 0;
    
    for(unsigned long i=0; i<positionIndices.size(); i++){
        int posIndex = positionIndices.at(i);
        vec3 pos = temp_positions.at(posIndex-1);
        indices.push_back(countIndex);
        positions.push_back(pos);
        countIndex++;
    }
    
    //potentially no need to render
    for(unsigned long i=0; i<uvIndices.size(); i++){
        int uvIndex = uvIndices.at(i);
        glm::vec2 text = temp_uv.at(uvIndex-1);
        //indices.push_back(countIndex);
        uv.push_back(text);
        //countIndex++;
    }
    for(unsigned long i=0; i<normalIndices.size();i++ ){
        int normalIndex = normalIndices.at(i);
        glm::vec3 norm = temp_normals.at(normalIndex-1);
        indices.push_back(countIndex);
        normals.push_back(norm);
        countIndex++;
    }
    
    file.close();
}

//set up, draw, and clear mesh geometry data buffers in OpenGL
void ObjFile::build(){
    if(m_vao != 0){return;}
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    glGenBuffers(1, &m_vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glGenBuffers(1, &m_vbo_norm);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
}

void ObjFile::draw(){
    if (m_vao == 0) return;
    // bind our VAO which sets up all our buffers and data for us
    glBindVertexArray(m_vao);
    
    glEnableClientState(GL_COLOR_ARRAY);
    float colors[3] = {0.5, 0.0, 1.0};
    glColorPointer(3, GL_FLOAT, 0, &colors);
    
    // tell opengl to draw our VAO using the draw mode and how many verticies to render
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
}

void ObjFile::destroy(){
    // delete the vectors and data buffers
    temp_positions.clear();
    temp_uv.clear();
    temp_normals.clear();
    
    positionIndices.clear();
    uvIndices.clear();
    normalIndices.clear();
    
    positions.clear();
    uv.clear();
    normals.clear();
    indices.clear();
    
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_pos);
    //glDeleteBuffers(1, &m_vbo_text);
    glDeleteBuffers(1, &m_vbo_norm);
    glDeleteBuffers(1, &m_ibo);
    m_vao = 0;
}

/*
 prints the mesh data in the console for debugging purposes
 */
void ObjFile::printMeshData(){
    if (m_vao == 0){
        cout<<"No data to display."<<endl;
    } else {
        
        //positions
        for(unsigned long i=0;i<positions.size();i++){
            cout<<"v "<<to_string(positions.at(i))<<endl;
        }
    
        //textures
        for(unsigned long i=0;i<uv.size();i++){
            cout<<"vt "<<to_string(uv.at(i))<<endl;
        }
        
        //normals
        for(unsigned long i=0;i<normals.size();i++){
            cout<<"vn "<<to_string(normals.at(i))<<endl;
        }
        
        //faces
        for (unsigned long i = 0; i < positionIndices.size(); i = i+3) {
            printf("f %d/%d/%d\t", positionIndices.at(i), uvIndices.at(i), normalIndices.at(i));
            printf("%d/%d/%d\t", positionIndices.at(i+1), uvIndices.at(i+1), normalIndices.at(i+1));
            printf("%d/%d/%d\n", positionIndices.at(i+2), uvIndices.at(i+2), normalIndices.at(i+2));
        }
    }
}
