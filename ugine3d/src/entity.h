#pragma once

#include "common.h"

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

class Entity {
public:
	static EntityPtr create();

	const std::string&	getName() const { return mName; }
	void				setName(const std::string& name) { mName = name; }

	const glm::vec3&	getPosition() const { return mPosition; }
	void				setPosition(const glm::vec3& pos) { mPosition = pos; }
	
	const				glm::vec3& getEuler() const { return mEuler; }
	const void			setEuler(const glm::vec3& rot) { mEuler = rot; mQuat = glm::quat(glm::radians(rot)); }
	
	const glm::quat&	getQuaternion() const { return mQuat; }
	const void			setQuaternion(const glm::quat& q) { mQuat = q; mEuler = glm::degrees(glm::eulerAngles(q)); }
	
	const glm::vec3&	getScale() const { return mScale; }
	void				setScale(const glm::vec3& scale) { mScale = scale; }

	void				move(const glm::vec3& vec) { mPosition += mQuat * vec; }

	virtual void		update(float deltaTime) {}
	virtual void		draw() {}
protected:
	Entity() : mScale(1, 1, 1) {}
	virtual ~Entity() {}
private:
	std::string	mName;
	glm::vec3	mPosition;
	glm::vec3	mEuler;
	glm::quat	mQuat;
	glm::vec3	mScale;
};

inline EntityPtr Entity::create() {
	std::shared_ptr<Entity> p(new Entity, [](Entity* p) {
		delete p;
	});
	return p;
}