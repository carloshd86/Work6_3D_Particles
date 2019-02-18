#pragma once

#include "common.h"

class Texture;
class Shader;

class Material {

public:
	enum BlendMode {
		ALPHA = 0,
		ADD   = 1,
		MUL   = 2,
		SOLID = 3
	};

	Material(const std::shared_ptr<Texture>& tex = nullptr, const std::shared_ptr<Shader>& shader = nullptr, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f }, uint8_t shininess = 255, BlendMode blendMode = ALPHA, bool lighting = true, bool culling = true, bool depthWrite = true);

	const std::shared_ptr<Shader>&  getShader() const;
	std::shared_ptr<Shader>&        getShader();
	void                            setShader(const std::shared_ptr<Shader>& shader);
	const std::shared_ptr<Texture>& getTexture() const;
	void                            setTexture(const std::shared_ptr<Texture>& tex);
	const glm::vec4&                getColor() const;
	void                            setColor(const glm::vec4& color);
	uint8_t                         getShininess() const;
	void                            setShininess(uint8_t shininess);
	void                            prepare();

	BlendMode getBlendMode() const;
	void setBlendMode(BlendMode blendMode);
	bool getLighting() const;
	void setLighting(bool enable);
	bool getCulling() const;
	void setCulling(bool enable);
	bool getDepthWrite() const;
	void setDepthWrite(bool enable);

private:
	std::shared_ptr<Texture> mTexture;
	std::shared_ptr<Shader>  mShader;
	glm::vec4                mColor;
	uint8_t                  mShininess;
	BlendMode                mBlendMode;
	bool                     mLighting;
	bool                     mCulling;
	bool                     mDepthWrite;
};
