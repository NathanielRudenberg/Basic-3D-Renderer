#pragma once
#include<Eigen/Core>

using Eigen::RowVector4f;
using Eigen::RowVector3f;

class Camera {
private:
	RowVector3f _pos;
	RowVector3f _forward;
	RowVector3f _up;
	void rotate(float angle, RowVector3f& axis);
	RowVector3f getHoriz();

public:
	RowVector3f _yAxis{0.0f, 1.0f, 0.0f};
	RowVector3f getPos();
	RowVector3f getForward();
	RowVector3f getLeft();
	RowVector3f getRight();
	RowVector3f getUp();
	void translate(RowVector3f& translateBy);
	void rotateX(float angle);
	void rotateY(float angle);
	void setPos(RowVector3f& pos);
	void setForward(RowVector3f& forward);
	void setUp(RowVector3f& up);

// Constructor
public:
	Camera(RowVector3f& pos, RowVector3f& forward, RowVector3f& up);
};