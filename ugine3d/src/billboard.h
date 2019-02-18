#pragma once

#include "common.h"
#include "entity.h"
#include "material.h"
#include "buffer.h"

class Billboard;
typedef std::shared_ptr<Billboard> BillboardPtr;

class Billboard : public Entity {
public:
	static BillboardPtr create(const Material& mat, float spin = 0.f);

	const Material& getMaterial() const;
	Material& getMaterial();
	const glm::vec2& getSize() const;
	void setSize(const glm::vec2& size);
	float getSpin() const;
	void setSpin(float spin);
	virtual void draw() override;

protected:
	Billboard(const Material& mat, float spin = 0.f);

	Material        mMaterial;
	glm::vec2       mSize;
	float           mSpin;

private:
	BufferPtr       mBuffer;
};