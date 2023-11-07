module;

#include <GL/glew.h>

module Graphics.Resources:Material;



import :Utilities;


Material::Material(ShaderProgram& p_shader):
	_shader(p_shader)
{}

void Material::SetMaterialVariables()
{
	using namespace UniformVariables::Material;

	_shader.SetVariable(materialHasTexture, false);
	if (!_texture.expired()) {
		_shader.SetVariable(materialHasTexture, true);
	}

	_shader.SetVariable(materialAmbientColor, _lightProperties.ambient);
	_shader.SetVariable(materialDiffuseColor, _lightProperties.diffuse);
	_shader.SetVariable(materialSpecularColor, _lightProperties.specular);
	_shader.SetVariable(materialEmissiveColor, _lightProperties.emissive);
	_shader.SetVariable(materialShininess, _lightProperties.shininess);
}