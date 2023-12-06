module Graphics.Resources:Material;

import :Utilities;

Material::Material(ShaderProgram& p_shader):
	_shader(p_shader)
{}

void Material::SetMaterialVariables()
{
	using namespace UniformVariables::Material;

	_shader.SetVariable(materialHasTexture, false);
	if (!_diffuseMap.expired()) {
		
		_shader.SetVariable(materialHasTexture, true);
		
		_diffuseMap.lock()->Bind(TextureUnits::Diffuse);
		
		//TODO: optimize
		if (_ambientMap.expired()) {
			_diffuseMap.lock()->Bind(TextureUnits::Ambient);
		}
		else {
			_ambientMap.lock()->Bind(TextureUnits::Ambient);
		}
		
		if (_specularMap.expired()) {
			_diffuseMap.lock()->Bind(TextureUnits::Specular);
		}
		else {
			_specularMap.lock()->Bind(TextureUnits::Specular);
		}

	}

	_shader.SetVariable(materialAmbientColor, _lightProperties.ambient);
	_shader.SetVariable(materialDiffuseColor, _lightProperties.diffuse);
	_shader.SetVariable(materialSpecularColor, _lightProperties.specular);
	_shader.SetVariable(materialEmissiveColor, _lightProperties.emissive);
	_shader.SetVariable(materialShininess, _lightProperties.shininess);
}