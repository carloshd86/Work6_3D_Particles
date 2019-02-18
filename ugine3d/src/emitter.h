#pragma once

#include "common.h"
#include "entity.h"
#include "material.h"
#include "particle.h"
#include <list>

class Emitter;
typedef std::shared_ptr<Emitter> EmitterPtr;

class Emitter : public Entity {
public:
	static EmitterPtr create(const Material& mat, bool autofade = false);

	void setRateRange(float min, float max);
	void setVelocityRange(const glm::vec3& min, const glm::vec3& max);
	void setSpinVelocityRange(float min, float max);
	void setScaleRange(float min, float max);
	void setLifetimeRange(float min, float max);
	void setColorRange(const glm::vec4& min, const glm::vec4& max);
	void emit(bool enable);
	bool isEmitting();
	virtual void update(float deltaTime) override;
	virtual void draw() override;

protected:
	Emitter(const Material& mat, bool autofade = false);

private:
	const Material& mMaterial;
	bool            mAutofade;
	float           mRateMin;
	float           mRateMax;
	glm::vec3       mVelocityMin;
	glm::vec3       mVelocityMax;
	float           mSpinVelocityMin;
	float           mSpinVelocityMax;
	float           mScaleMin;
	float           mScaleMax;
	float           mLifetimeMin;
	float           mLifetimeMax;
	glm::vec4       mColorMin;
	glm::vec4       mColorMax;
	bool            mEmit;

	int                    mNumParticles;
	std::list<ParticlePtr> mParticles;
};