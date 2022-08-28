
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"
#include "triangle.hpp"
#include "objfile.hpp"


// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// basic shader
	GLuint m_shader;

	// test triangle model
	TestTriangle m_model;
    //obj file model
    ObjFile file_model;
    
    //completion - colour RGB values
    float color[3] = {1,1,1};
    
    //challenge - lighting position
    float lightingPos[3] = {1,1,1};

public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};
