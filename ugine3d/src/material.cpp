#include "material.h"
#include "texture.h"
#include "shader.h"
#include "state.h"


Material::Material(const std::shared_ptr<Texture>& tex, const std::shared_ptr<Shader>& shader, const glm::vec4& color, uint8_t shininess, 
					BlendMode blendMode, bool lighting, bool culling, bool depthWrite) :
	mTexture    (tex),
	mShader     (shader),
	mColor      (color),
	mShininess  (shininess),
	mBlendMode  (blendMode),
	mLighting   (lighting),
	mCulling    (culling),
	mDepthWrite (depthWrite),
	mOpacityMultiplier(1.f) {}

/******************/

const std::shared_ptr<Shader>& Material::getShader() const {
	if (!mShader) return const_cast<const std::shared_ptr<Shader>&>(State::defaultShader);
	else          return const_cast<const std::shared_ptr<Shader>&>(mShader);
}

/******************/

std::shared_ptr<Shader>& Material::getShader() {
	if (!mShader) return State::defaultShader;
	else          return mShader;
}

/******************/

void Material::setShader(const std::shared_ptr<Shader>& shader) {
	mShader = shader;
}

/******************/

const std::shared_ptr<Texture>& Material::getTexture() const {
	return const_cast<const std::shared_ptr<Texture>&>(mTexture);
}

/******************/

void Material::setTexture(const std::shared_ptr<Texture>& tex) {
	mTexture = tex;
}

/******************/

const glm::vec4& Material::getColor() const {
	return mColor;
}

/******************/

void Material::setColor(const glm::vec4& color) {
	mColor = color;
}

/******************/

uint8_t Material::getShininess() const {
	return mShininess;
}

/******************/

void Material::setShininess(uint8_t shininess) {
	mShininess = shininess;
}

/******************/

void Material::prepare() {
	ShaderPtr shaderUsed = getShader();
	shaderUsed->use();

	glm::mat4 mvpMatrix = State::projectionMatrix * State::viewMatrix * State::modelMatrix;
	shaderUsed->setMatrix(shaderUsed->getLocation("mvp"), mvpMatrix);

	glm::mat4 modelviewMatrix = State::viewMatrix * State::modelMatrix;
	shaderUsed->setMatrix(shaderUsed->getLocation("modelView"), modelviewMatrix);

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelviewMatrix));
	shaderUsed->setMatrix(shaderUsed->getLocation("mNormal"), normalMatrix);

	int isTexture = mTexture ? 1 : 0;
	shaderUsed->setInt(shaderUsed->getLocation("isTexture"), isTexture);
	shaderUsed->setInt(shaderUsed->getLocation("texSampler"), 0);
	if (isTexture) mTexture->bind();

	int numLights = static_cast<int>(State::lights.size());
	shaderUsed->setInt(shaderUsed->getLocation("numLights"), numLights);
	shaderUsed->setVec4(shaderUsed->getLocation("diffuseColor"), mColor);
	shaderUsed->setInt(shaderUsed->getLocation("shininess"), mShininess);
	shaderUsed->setVec3(shaderUsed->getLocation("ambientLight"), State::ambient);
	shaderUsed->setFloat(shaderUsed->getLocation("opacityMultiplier"), mOpacityMultiplier);
	for (int i = 0; i < numLights; ++i) {
		State::lights[i]->prepare(i, shaderUsed);
	}

	switch (mBlendMode) {
		case ALPHA: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
		case ADD:   glBlendFunc(GL_SRC_ALPHA, GL_ONE);                 break;
		case MUL:   glBlendFunc(GL_DST_COLOR, GL_ZERO);                break;
		case SOLID: glBlendFunc(GL_ONE      , GL_ZERO);                break;
		default:    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
	}

	if (mCulling) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (mDepthWrite) {
		glDepthMask(GL_TRUE);
	}
	else {
		glDepthMask(GL_FALSE);
	}
}

/******************/

Material::BlendMode Material::getBlendMode() const {
	return mBlendMode;
}

/******************/

void Material::setBlendMode(BlendMode blendMode) {
	mBlendMode = blendMode;
}

/******************/

bool Material::getLighting() const {
	return mLighting;
}

/******************/

void Material::setLighting(bool enable) {
	mLighting = enable;
}

/******************/

bool Material::getCulling() const {
	return mCulling;
}

/******************/

void Material::setCulling(bool enable) {
	mCulling = enable;
}

/******************/

bool Material::getDepthWrite() const {
	return mDepthWrite;
}

/******************/

void Material::setDepthWrite(bool enable) {
	mDepthWrite = enable;
}

/******************/

float Material::getOpacityMultiplier() const {
	return mOpacityMultiplier;
}

/******************/

void Material::setOpacityMultiplier(float multiplier) {
	mOpacityMultiplier = multiplier;
}