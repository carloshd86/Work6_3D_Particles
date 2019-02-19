#include "billboard.h"
#include "texture.h"
#include "state.h"
#include "vertex.h"

BillboardPtr Billboard::create(const Material& mat, float spin) {
	std::shared_ptr<Billboard> p(new Billboard(mat, spin), [](Billboard* p) {
		delete p;
	});
	return p;
}

/******************/

Billboard::Billboard(const Material& mat, float spin) :
	Entity     (),
	mMaterial  (const_cast<Material&>(mat)),
	mSpin      (spin),
	mAngleSpin (0.f) {
	const std::shared_ptr<Texture>& matTexture = mat.getTexture();
	if (matTexture) {
		mSize = matTexture->getSize();
	}
	else {
		mSize.x = 1.f;
		mSize.y = 1.f;
	}

	// define triangle
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3( 0.5f,  0.5f, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3( 0.5f, -0.5f, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-0.5f,  0.5f, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1))
	};

	std::vector<uint16_t> indices = {0, 1, 2, 0, 3, 1};

	mBuffer = Buffer::create(vertices, indices);
}

/******************/

const Material& Billboard::getMaterial() const {
	return const_cast<const Material&>(mMaterial) ;
}

/******************/

Material& Billboard::getMaterial() {
	return mMaterial;
}

/******************/

const glm::vec2& Billboard::getSize() const {
	return mSize;
}

/******************/

void Billboard::setSize(const glm::vec2& size) {
	mSize = size;
}

/******************/

float Billboard::getSpin() const {
	return mSpin;
}

/******************/

void Billboard::setSpin(float spin) {
	mSpin = spin;
}

/******************/

void Billboard::draw() {
	glm::mat4& view = State::viewMatrix;

	/*
	| left.x left.y left.z model.x |
	| up.x up.y up.z model.y |
	| forward.x forward.y forward.z model.z |
	| 0 0 0 1 |
	*/
	/*float modelMat[16] = { view[0][0], view[1][0], view[2][0], getPosition().x,
						   view[0][1], view[1][1], view[2][1], getPosition().y,
						   view[0][2], view[1][2], view[2][2], getPosition().z,
								  0.f,        0.f,        0.f,             1.f };
	glm::mat4 model = glm::make_mat4(modelMat);*/

	glm::mat4 model = view;
	model[3] = glm::vec4(getPosition(), 1.f);

	model = glm::rotate(model, glm::angle(getQuaternion()), glm::axis(getQuaternion()));
	model = glm::scale(model, glm::vec3(getSize(), 1.f) * getScale());
	State::modelMatrix = model;
	mMaterial.prepare();
	mBuffer->draw(*mMaterial.getShader());
}