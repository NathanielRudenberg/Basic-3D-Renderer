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
	void rotate(float angle, RowVector3f& axis);
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
	void translate(RowVector3f& translateBy, float amount, int dir);
	void rotateX(float angle);
	void rotateY(float angle);
	void setPos(RowVector3f& pos);
	void setForward(RowVector3f& forward);
	void setUp(RowVector3f& up);

// Constructor
public:
	Camera();
	Camera(RowVector3f& pos, RowVector3f& forward, RowVector3f& up);
};