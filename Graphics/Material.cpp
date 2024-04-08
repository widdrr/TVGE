module Graphics.Resources:Material;

import :Utilities;

using namespace TVGE::Graphics;

LightProperties::LightProperties():
	ambient(0),
	diffuse(0),
	specular(0),
	emissive(0),
	shininess(1)
{}

Material::Material(ShaderProgram& p_shader):
	_shader(p_shader)
{}

void Material::SetMaterialVariables()
{
	using namespace UniformVariables::Materials;

	_shader.SetVariable(materialHasTexture, false);
	if (!diffuseMap.expired()) {
		
		_shader.SetVariable(materialHasTexture, true);
		
		diffuseMap.lock()->Bind(TextureUnits::Diffuse);
		
		//TODO: optimize
		if (ambientMap.expired()) {
			diffuseMap.lock()->Bind(TextureUnits::Ambient);
		}
		else {
			ambientMap.lock()->Bind(TextureUnits::Ambient);
		}
		
		if (specularMap.expired()) {
			diffuseMap.lock()->Bind(TextureUnits::Specular);
		}
		else {
			specularMap.lock()->Bind(TextureUnits::Specular);
		}

	}

	_shader.SetVariable(materialAmbientColor, lightProperties.ambient);
	_shader.SetVariable(materialDiffuseColor, lightProperties.diffuse);
	_shader.SetVariable(materialSpecularColor, lightProperties.specular);
	_shader.SetVariable(materialEmissiveColor, lightProperties.emissive);
	_shader.SetVariable(materialShininess, lightProperties.shininess);
}