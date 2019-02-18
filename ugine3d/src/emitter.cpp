#include "emitter.h"


EmitterPtr Emitter::create(const Material& mat, bool autofade) {
	std::shared_ptr<Emitter> p(new Emitter(mat, autofade), [](Emitter* p) {
		delete p;
	});
	return p;
}

/******************/

Emitter::Emitter(const Material& mat, bool autofade) :
	Entity           (),
	mMaterial        (mat),
	mAutofade        (autofade),
    mRateMin         (0.f),
    mRateMax         (0.f),
    mVelocityMin     { 0.f, 0.f, 0.f },
    mVelocityMax     { 0.f, 0.f, 0.f },
    mSpinVelocityMin (0.f),
    mSpinVelocityMax (0.f),
    mScaleMin        (0.f),
    mScaleMax        (0.f),
    mLifetimeMin     (0.f),
    mLifetimeMax     (0.f),
    mColorMin        { 0.f, 0.f, 0.f, 0.f },
    mColorMax        { 0.f, 0.f, 0.f, 0.f },
    mEmit            (false) {}

/******************/

void Emitter::setRateRange(float min, float max) {
	mRateMin = min;
	mRateMax = max;
}

/******************/

void Emitter::setVelocityRange(const glm::vec3& min, const glm::vec3& max) {
	mVelocityMin = min;
	mVelocityMax = max;
}

/******************/

void Emitter::setSpinVelocityRange(float min, float max) {
	mSpinVelocityMin = min;
	mSpinVelocityMax = max;
}

/******************/

void Emitter::setScaleRange(float min, float max) {
	mScaleMin = min;
	mScaleMax = max;
}

/******************/

void Emitter::setLifetimeRange(float min, float max) {
	mLifetimeMin = min;
	mLifetimeMax = max;
}

/******************/

void Emitter::setColorRange(const glm::vec4& min, const glm::vec4& max) {
	mColorMin = min;
	mColorMax = max;
}

/******************/

void Emitter::emit(bool enable) {
	mEmit = enable;
}

/******************/

bool Emitter::isEmitting() {
	return mEmit;
}

/******************/

void Emitter::update(float deltaTime) {
	if (mEmit) {
		float numParticles = glm::linearRand(mRateMin, mRateMax);

		while (mNumParticles < numParticles) {
			glm::vec3 particleVel(glm::linearRand(mVelocityMin.x, mVelocityMax.x), glm::linearRand(mVelocityMin.y, mVelocityMax.y), glm::linearRand(mVelocityMin.z, mVelocityMax.z));
			float particleSpinVel = glm::linearRand(mSpinVelocityMin, mSpinVelocityMax);
			float particleLifetime = glm::linearRand(mLifetimeMin, mLifetimeMax);
			mParticles.push_front(Particle::create(mMaterial, particleVel, particleSpinVel, particleLifetime, mAutofade));
			++mNumParticles;
		}
	}

	for (auto it = mParticles.begin(); it != mParticles.end(); ) {
		if (0 >= (*it)->getRemainingLifetime()) {
			it = mParticles.erase(it);
			--mNumParticles;
		}
		else {
			(*it)->update(deltaTime);
			++it;
		}
	}
}

/******************/

void Emitter::draw() {
	for (auto& particle : mParticles) {
		particle->draw();
	}
}