#include "Renderer.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

std::shared_ptr<Renderer> Renderer::_instance = nullptr;

std::shared_ptr<Renderer> Renderer::GetInstance(){
	
	if (_instance == nullptr){
		_instance = std::shared_ptr<Renderer>(new Renderer());
	}

	return _instance;
}

std::shared_ptr<ShaderProgram> Renderer::ShaderFactory(std::string p_vertexShaderPath, std::string p_fragmentShaderPath) {

	ShaderProgram program(p_vertexShaderPath, p_fragmentShaderPath);
	_shaders.push_back(std::make_shared<ShaderProgram>(program));
	return _shaders.back();
}

void Renderer::AddObject(Entity& p_object) {

	//obtaining the GraphicsComponent of the Entity
	auto graphicsComponent = p_object.GetComponentOfType<GraphicsComponent>();
	if (graphicsComponent == nullptr) {
		std::cerr << "Object does not have a Graphics Component";
		return;
	}

	if (graphicsComponent->mesh == nullptr) {
		std::cerr << "Object does not have a Mesh";
		return;
	}

	//generating VAO to store buffer data
	glGenVertexArrays(1, &graphicsComponent->_vao);
	
	//generating the VBO and EBO
	glGenBuffers(1, &graphicsComponent->_vbo);
	glGenBuffers(1, &graphicsComponent->_ebo);

	//activating VAO
	glBindVertexArray(graphicsComponent->_vao);

	//copying data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, graphicsComponent->_vbo);
	auto& vertices = graphicsComponent->mesh->GetVertices();
	//TODO, replace c-style cast
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), (void*)vertices.data(), GL_STATIC_DRAW);

	//copying data to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsComponent->_ebo);
	auto& elements = graphicsComponent->mesh->GetElements();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), (void*)elements.data(), GL_STATIC_DRAW);

	//setting Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);
	//setting Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//deactivating VAO
	glBindVertexArray(0);

	_entities.push_back(graphicsComponent);
}

void Renderer::SetPerspective(float p_fov, float p_nearPlane, float p_farPlane) {
	
	float aspectRatio = static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);
	_projectionMatrix = glm::perspective(p_fov / 2.f, aspectRatio, p_nearPlane, p_farPlane);

	for (auto& shader : _shaders) {
		shader->SetVariable("glProjectionMatrix", _projectionMatrix);
	}
}

void Renderer::RenderFunction(){

	glClear(GL_COLOR_BUFFER_BIT);

	for (const auto& object : _entities) {
		glBindVertexArray(object->_vao);
		//TODO: IMPORTANT!! design a mechanism to store drawing logic in the object
		//to permit customizing this

		//If there is a shader associated we use it
		if (object->shaderProgram != nullptr) {
			object->shaderProgram->SetVariable("glModelMatrix", object->GetModelTransformation());
			glUseProgram(object->shaderProgram->_id);
		}
		//else fallback to the default shader
		else {
			_shaders[0]->SetVariable("glModelMatrix", object->GetModelTransformation());
			glUseProgram(_shaders[0]->_id);
		}
		
		glPointSize(10.f);
		glDrawElements(GL_TRIANGLES, object->mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	glutPostRedisplay();
	glutSwapBuffers();
}

void Renderer::CleanupFunction(){
	
	for (const auto& entity : _entities) {
		glBindVertexArray(entity->_vao);
		//TODO: make an enum for vertex attributes
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &entity->_vbo);
		glDeleteBuffers(1, &entity->_ebo);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &entity->_vao);
	}

	glUseProgram(0);
	for (const auto& shader : _shaders) {
		glDeleteProgram(shader->_id);
	}
}

void Renderer::RenderCallback() {
	
	_instance->RenderFunction();
}

void Renderer::CleanupCallback() {

	_instance->CleanupFunction();
}

Renderer::Renderer() :
	_projectionMatrix(glm::identity<glm::mat4>())
{
	
	//TODO: look into command line arguments
	int argc = 0;
	char* argv[1] = {};
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(_windowWidth, _windowHeight);
	glutCreateWindow("TavaGL V0.2a");

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << "\n";
		exit(1);
	}

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//TODO: further improve shader management
	ShaderFactory("shader.vert", "shader.frag");
	_shaders.front()->SetVariable("glProjectionMatrix", _projectionMatrix);

	glutDisplayFunc(RenderCallback);
	glutCloseFunc(CleanupCallback);

}

//TODO: multithread the main loop
//IMPOSSIBLE, GLUT is not thread safe
//Either thread the physics engine or move to GLFW or something
void Renderer::Run() {

	glutMainLoop();
}