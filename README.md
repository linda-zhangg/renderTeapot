# CGRA 251 Assignment 1 readme

Linda Zhang 300570498 zhanglind1

## Core - done
- made objfile.hpp and objfile.cpp
- made new ObjFile field in application.hpp
- private fields include: vectors to store the temporary parse data 
                          the reordered parse data and indices (used index buffers)
                          GLuint variables to reference OpenGL objects
- functions include: loadObj - loads all the data from the obj file, sorts positions and normals to the correct order specified by faces, and added data to the indices vector.
                     build - binds and prepares the data to be displayed
                     draw - displays the 3d teapot on the screen
                     destroy - unloads the reference to all objects and clear data
                     printMeshData - prints the mesh data that was loaded
- call the relevant functions from objfile.cpp in application.cpp when buttons are pressed: "Load" button calls loadObj, "Print" button calls printMeshData, "Unload" button calls destroy.
- I also ensured that data is automatically destroyed first before data is loaded again, so that a different object, such as cube.obj can be loaded quickly.

## Completion - done
- edited the fragment shader program to include floats to represent RGB values
- made new array of 3 floats field in application.hpp for the RGB values
- uploaded uniform color vaiables from the shader program in application.cpp
- link this field with ImGui::ColorEdit3 to change the colour, and render the teapot in a different colour.

## Challenge
- edited the fragment shader program to include floats to represent x,y,z values for light position and made new vec3 to store these values
- made new array of 3 floats field in application.hpp for the xyz values
- uploaded uniform lighting position vaiables from the shader program in application.cpp
- link this field with ImGui::SliderFloat3 to change the lighting position with sliders on the gui, and render the teapot from the perspective of a different light source.
