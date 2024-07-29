#pragma once
#include<Eigen/Core>

using Eigen::RowVector4f;
using Eigen::RowVector3f;

class Camera {
private:
	RowVector3f _pos;
	RowVector3f _forward;
	RowVector3f _up;
	RowVector3f _right;
	RowVector3f _left;
	RowVector3f _front;
	RowVector3f _back;
	void rotate(float angle, const RowVector3f& axis);
	RowVector3f getHoriz();

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
	RowVector3f _yAxis{0.0f, 1.0f, 0.0f};
	RowVector3f& getPos();
	RowVector3f& getForward();
	RowVector3f& getLeft();
	RowVector3f& getRight();
	RowVector3f& getUp();
	RowVector3f& getFront();
	RowVector3f& getBack();
	void translate(const RowVector3f& translateBy, float amount, int dir);
	void rotateX(float angle);
	void rotateY(float angle);
	void setPos(const RowVector3f& pos);
	void setForward(const RowVector3f& forward);
	void setUp(const RowVector3f& up);

// Constructor
public:
	Camera();
	Camera(const RowVector3f& pos, const RowVector3f& forward, const RowVector3f& up);
};