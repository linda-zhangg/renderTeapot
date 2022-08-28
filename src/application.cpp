
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_shader.hpp"


using namespace std;
using namespace cgra;
using namespace glm;

Application::Application(GLFWwindow *window) : m_window(window) {
	
	// build the shader
	shader_builder color_sb;
	color_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_vert.glsl"));
	color_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_frag.glsl"));
	m_shader = color_sb.build();

	// build the mesh for the triangle
	m_model.setup();
}


void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	// calculate the projection and view matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);
	mat4 view = translate(mat4(1), vec3(0, -5, -20));

	// set shader and upload variables
	glUseProgram(m_shader);
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(view));
    //colour coords
    glUniform1f(glGetUniformLocation(m_shader, "R"), color[0]);
    glUniform1f(glGetUniformLocation(m_shader, "G"), color[1]);
    glUniform1f(glGetUniformLocation(m_shader, "B"), color[2]);
    //lighting control
    glUniform1f(glGetUniformLocation(m_shader, "x"), lightingPos[0]);
    glUniform1f(glGetUniformLocation(m_shader, "y"), lightingPos[1]);
    glUniform1f(glGetUniformLocation(m_shader, "z"), lightingPos[2]);

	// draw the model
    file_model.draw();
	m_model.draw();
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500, 130), ImGuiSetCond_Once);
	ImGui::Begin("Mesh loader", 0);

	// Loading buttons
	static char filename[512] = "";
	ImGui::InputText("", filename, 512);
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
        m_model.destroy();
        
		// TODO load mesh from 'filename'
        file_model.loadObj(filename);
        file_model.build();
	}

	ImGui::SameLine();
	if (ImGui::Button("Print")) {
		// TODO print mesh data
        file_model.printMeshData();
	}

	ImGui::SameLine();
	if (ImGui::Button("Unload")) {
		// TODO unload mesh
        file_model.destroy();
	}
    
    //completion - colour controls
    ImGui::NewLine();
    ImGui::ColorEdit3("Colour", color);
    
    //challenge - lighting controls
    ImGui::NewLine();
    ImGui::SliderFloat3("Lighting position", lightingPos, -1.0, 1.0);

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	(void)xpos, ypos; // currently un-used
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)button, action, mods; // currently un-used
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset, yoffset; // currently un-used
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
