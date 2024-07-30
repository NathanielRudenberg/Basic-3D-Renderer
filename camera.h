#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/quaternion.hpp>

using glm::vec3;
using glm::mat3;
using glm::quat;
using glm::toMat4;
using glm::angleAxis;

class Camera {
private:
	vec3 _pos;
	vec3 _forward;
	vec3 _up;
	vec3 _right;
	vec3 _left;
	vec3 _front;
	vec3 _back;
	void rotate(float angle, const vec3& axis);
	vec3 getHoriz();

public:
	enum direction {
		PLUS,
		MINUS
	};
	enum coords {
		X,
		Y,
		Z
	};
	vec3 _yAxis{0.0f, 1.0f, 0.0f};
	vec3& getPos();
	vec3& getForward();
	vec3& getLeft();
	vec3& getRight();
	vec3& getUp();
	vec3& getFront();
	vec3& getBack();
	void translate(const vec3& translateBy, float amount, int dir);
	void rotateX(float angle);
	void rotateY(float angle);
	void setPos(const vec3& pos);
	void setForward(const vec3& forward);
	void setUp(const vec3& up);

// Constructor
public:
	Camera();
	Camera(const vec3& pos, const vec3& forward, const vec3& up);
};