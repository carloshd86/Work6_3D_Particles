#pragma once

#include "billboard.h"

class Particle;
typedef std::shared_ptr<Particle> ParticlePtr;

class Particle : public Billboard {
public:
	static ParticlePtr create(const Material& mat, const glm::vec3& vel = { 0.f, 0.f, 0.f }, float spinVel = 0.f, float lifetime = 1.f, bool autofade = false);

	float getRemainingLifetime() const;
	virtual void update(float deltaTime) override;
	
protected:
	Particle(const Material& mat, const glm::vec3& vel = { 0.f, 0.f, 0.f }, float spinVel = 0.f, float lifetime = 1.f, bool autofade = false);

private:
	float           mInitialColorAlpha;
	glm::vec3       mVel;
	float           mLifetime;
	float           mCurrentLifetime;
	bool            mAutofade;
};