#include "emitter.h"


EmitterPtr Emitter::create(const Material& mat, bool autofade) {
	std::shared_ptr<Emitter> p(new Emitter(mat, autofade), [](Emitter* p) {
		delete p;
	});
	return p;
}

/******************/

Emitter::Emitter(const Material& mat, bool autofade) :
	Entity              (),
	mMaterial           (mat),
	mAutofade           (autofade),
    mRateMin            (0.f),
    mRateMax            (0.f),
    mVelocityMin        { 0.f, 0.f, 0.f },
    mVelocityMax        { 0.f, 0.f, 0.f },
    mSpinVelocityMin    (0.f),
    mSpinVelocityMax    (0.f),
    mScaleMin           (1.f),
    mScaleMax           (1.f),
    mLifetimeMin        (0.f),
    mLifetimeMax        (0.f),
    mColorMin           { 0.f, 0.f, 0.f, 0.f },
    mColorMax           { 0.f, 0.f, 0.f, 0.f },
    mEmit               (false),
	mNumParticles       (0.f),
	mNumParticlesToEmit (0.f) {}

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
		mNumParticlesToEmit += glm::linearRand(mRateMin, mRateMax) * deltaTime;

		int particlesEmitted = 0;
		for (int i = mNumParticles; i  < static_cast<int>(mNumParticlesToEmit); ++i) {
			glm::vec3 particleVel(glm::linearRand(mVelocityMin.x, mVelocityMax.x), glm::linearRand(mVelocityMin.y, mVelocityMax.y), glm::linearRand(mVelocityMin.z, mVelocityMax.z));
			float particleSpinVel = glm::linearRand(mSpinVelocityMin, mSpinVelocityMax);
			float particleLifetime = glm::linearRand(mLifetimeMin, mLifetimeMax);
			float particleScale = glm::linearRand(mScaleMin, mScaleMax);
			ParticlePtr particle = Particle::create(mMaterial, particleVel, particleSpinVel, particleLifetime, mAutofade);
			particle->setScale(glm::vec3(particleScale, particleScale, 1.f));
			particle->setPosition(getPosition());
			//glm::vec4 particleColor(glm::linearRand(mColorMin.r, mColorMax.r), glm::linearRand(mColorMin.g, mColorMax.g), glm::linearRand(mColorMin.b, mColorMax.b), glm::linearRand(mColorMin.a, mColorMax.a));
			// TODO treat color
			mParticles.push_front(particle);
			++mNumParticles;
			++particlesEmitted;
		}
		mNumParticlesToEmit -= particlesEmitted;
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