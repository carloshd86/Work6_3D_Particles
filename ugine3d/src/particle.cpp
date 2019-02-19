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
}

/******************/

float Particle::getRemainingLifetime() const {
	return std::max(mLifetime - mCurrentLifetime, 0.f);
}

/******************/

void Particle::update(float deltaTime) {
	mCurrentLifetime += deltaTime;
	setPosition(getPosition() + deltaTime * mVel);
	mAngleSpin += glm::radians(deltaTime * mSpin);
	setQuaternion(glm::angleAxis(mAngleSpin, glm::vec3(0.f, 0.f, 1.f)));
	if (mAutofade) {
		mMaterial.setOpacityMultiplier(getRemainingLifetime() / mLifetime);
	}
	else {
		mMaterial.setOpacityMultiplier(1.f);
	}
}