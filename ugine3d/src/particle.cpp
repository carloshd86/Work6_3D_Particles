#include "particle.h"
#include <algorithm>

ParticlePtr Particle::create(const Material& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade) {
	std::shared_ptr<Particle> p(new Particle(mat, vel, spinVel, lifetime, autofade), [](Particle* p) {
		delete p;
	});
	return p;
}

/******************/

Particle::Particle(const Material& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade) :
	Billboard        (mat, spinVel),
	mVel             (vel),
	mLifetime        (lifetime),
	mAutofade        (autofade),
	mCurrentLifetime (0.f) {
	mInitialColorAlpha = mMaterial.getColor().a;
}

/******************/

float Particle::getRemainingLifetime() const {
	return std::max(mLifetime - mCurrentLifetime, 0.f);
}

/******************/

void Particle::update(float deltaTime) {
	mCurrentLifetime += deltaTime;
	setPosition(getPosition() + deltaTime * mVel);
	setEuler(getEuler() + mSpin * deltaTime);
	if (mAutofade) {
		glm::vec4 updatedColor = mMaterial.getColor();
		updatedColor.a = getRemainingLifetime() * mInitialColorAlpha / mLifetime;
		mMaterial.setColor(updatedColor);
	}
}