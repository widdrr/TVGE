#include "Renderer.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <fstream>
#include <memory>

std::shared_ptr<Renderer> Renderer::_instance = nullptr;

std::shared_ptr<Renderer> Renderer::GetInstance(){
	
	if (_instance == nullptr){
		_instance = std::shared_ptr<Renderer>(new Renderer());
	}

	return _instance;
}

void Renderer::AddObject(Object& p_object){

	//TODO: error handling
	glGenVertexArrays(1, &p_object.vao);
	
	glGenBuffers(1, &p_object.vbo);
	glGenBuffers(1, &p_object.ebo);

	glBindVertexArray(p_object.vao);

	glBindBuffer(GL_ARRAY_BUFFER, p_object.vbo);
	auto& vertices = p_object.mesh.GetVertices();
	//TODO, replace c-style cast
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), (void*)vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_object.ebo);
	auto& elements = p_object.mesh.GetDrawOrder();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), (void*)elements.data(), GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	_objects.push_back(std::make_shared<const Object>(p_object));
}

void Renderer::SetShaderProgram(const ShaderProgram& p_program) {

	_activeShaderProgram = std::make_shared<const ShaderProgram>(p_program);
	glUseProgram(_activeShaderProgram->Id());
}

void Renderer::RenderFunction(){

	for (const auto& object : _objects) {

		glBindVertexArray(object->vao);
		//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
		//to permit customizing this
		glPointSize(10.f);
		glDrawElements(GL_TRIANGLES, object->mesh.GetElementCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glutPostRedisplay();
	glutSwapBuffers();
}

void Renderer::CleanupFunction(){
	
	for (const auto& object : _objects) {

		glBindVertexArray(object->vao);
		//TODO: make an enum for vertex attributes
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &object->vbo);
		glDeleteBuffers(1, &object->ebo);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &object->vao);
	}
	glUseProgram(0);
	//glDeleteProgram(_activeShaderProgram->Id());
}

void Renderer::RenderCallback(){
	
	_instance->RenderFunction();
}

void Renderer::CleanupCallback(){

	_instance->CleanupFunction();
}

Renderer::Renderer() {
	
	//TODO: look into command line arguments
	int argc = 0;
	char* argv[1] = {};
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(_windowWidth, _windowHeight);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glutCreateWindow("Engine Project V0.1a");

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << "\n";
		exit(1);
	}

	//TODO: figure out this mess and initializing openGL nicely
	ShaderProgram shaderProgram("shader.vert", "shader.frag");
	SetShaderProgram(shaderProgram);

	glutDisplayFunc(RenderCallback);
	glutCloseFunc(CleanupCallback);

}

void Renderer::Run() {

	glutMainLoop();
}