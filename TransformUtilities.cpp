#include "TransformUtilities.h"

Matrix4f getPointAtMatrix(RowVector3f& pos, RowVector3f& target, RowVector3f& up) {
	RowVector3f newForward = (target - pos).normalized();
	RowVector3f a = newForward * up.dot(newForward);
	RowVector3f newUp = (up - a).normalized();
	RowVector3f newRight = newUp.cross(newForward);

	Matrix4f pointAt;
	pointAt <<	newRight, 0,
				newUp, 0,
				newForward, 0,
				pos, 1;

	return pointAt;
}

Matrix4f getTranslationMatrix(float x, float y, float z) {
	Matrix4f translation = Matrix4f::Identity();
	translation(3, 0) = x;
	translation(3, 1) = y;
	translation(3, 2) = z;

	return translation;
}

Matrix4f getXRot(float theta) {
	Matrix4f m = Matrix4f::Zero();
	m(0, 0) = 1.0f;
	m(1, 1) = cosf(theta);
	m(1, 2) = sinf(theta);
	m(2, 1) = -sinf(theta);
	m(2, 2) = cosf(theta);
	m(3, 3) = 1.0f;

	return m;
}

Matrix4f getYRot(float theta) {
	Matrix4f m = Matrix4f::Zero();
	m(0, 0) = cosf(theta);
	m(0, 2) = -sinf(theta);
	m(1, 1) = 1.0f;
	m(2, 0) = sinf(theta);
	m(2, 2) = cosf(theta);
	m(3, 3) = 1.0f;

	return m;
}

Matrix4f getZRot(float theta) {
	Matrix4f m = Matrix4f::Zero();
	m(0, 0) = cosf(theta);
	m(0, 1) = sinf(theta);
	m(1, 0) = -sinf(theta);
	m(1, 1) = cosf(theta);
	m(2, 2) = 1.0f;
	m(3, 3) = 1.0f;

	return m;
}

Matrix4f getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	Matrix4f projMat = Matrix4f::Zero();
	projMat(0, 0) = (aspectRatio * fovRadians) * -1.0f;
	projMat(1, 1) = fovRadians * -1.0f;
	projMat(2, 2) = farPlane / (farPlane - nearPlane);
	projMat(3, 2) = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projMat(2, 3) = 1.0f;

	return projMat;
}

RowVector4f project(Eigen::RowVector4f& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	RowVector4f tmpProj = toProject * getProjectionMatrix(fovRadians, aspectRatio, nearPlane, farPlane);
	RowVector4f projected;
	if (tmpProj[Engine::coordIndices::W] != 0.0f) {
		projected << tmpProj[Engine::coordIndices::X] / tmpProj[Engine::coordIndices::W], tmpProj[Engine::coordIndices::Y] / tmpProj[Engine::coordIndices::W], tmpProj[Engine::coordIndices::Z] / tmpProj[Engine::coordIndices::W], 1.0f;
	}
	else {
		projected << tmpProj[Engine::coordIndices::X], tmpProj[Engine::coordIndices::Y], tmpProj[Engine::coordIndices::Z], 1.0f;
	}

	return projected;
}

RowVector4f vectorPlaneIntersect(RowVector3f& planePoint, RowVector3f& planeNormal, RowVector4f& lineStart, RowVector4f& lineEnd) {
	planeNormal.normalize();
	RowVector3f start, end;
	RowVector4f toReturn;
	start << lineStart[Engine::coordIndices::X], lineStart[Engine::coordIndices::Y], lineStart[Engine::coordIndices::Z];
	end << lineEnd[Engine::coordIndices::X], lineEnd[Engine::coordIndices::Y], lineEnd[Engine::coordIndices::Z];

	float planeD = -1.0f * planeNormal.dot(planePoint);
	float ad = start.dot(planeNormal);
	float bd = end.dot(planeNormal);
	float t = (-planeD - ad) / (bd - ad);
	RowVector3f lineStartToEnd = end - start;
	RowVector3f lineToIntersect = lineStartToEnd * t;
	toReturn << start + lineToIntersect, 1.0f;
	return toReturn;
}

int clipTriangleAgainstPlane(RowVector3f& planePoint, RowVector3f& planeNormal, Engine::Trigon& inTri, Engine::Trigon& outTri1, Engine::Trigon& outTri2) {
	planeNormal.normalize();

	// Get shortest distance from point to plane
	auto dist = [&](RowVector4f& n) {
		RowVector3f p;
		p << n[Engine::coordIndices::X], n[Engine::coordIndices::Y], n[Engine::coordIndices::Z];

		return (planeNormal[Engine::coordIndices::X] * p[Engine::coordIndices::X]
			+ planeNormal[Engine::coordIndices::Y] * p[Engine::coordIndices::Y]
			+ planeNormal[Engine::coordIndices::Z] * p[Engine::coordIndices::Z]
			- planeNormal.dot(planePoint));
	};

	// Classify vertices on either side of the plane
	// Vertices on the inside have a positive sign
	RowVector4f* pointsInside[3]; int insidePointCount = 0;
	RowVector4f* pointsOutside[3]; int outsidePointCount = 0;

	// Get distance of each triangle vertex to plane
	float d0 = dist(inTri.v[0]);
	float d1 = dist(inTri.v[1]);
	float d2 = dist(inTri.v[2]);

	if (d0 >= 0) {
		pointsInside[insidePointCount++] = &inTri.v[0];
	}
	else {
		pointsOutside[outsidePointCount++] = &inTri.v[0];
	}
	if (d1 >= 0) {
		pointsInside[insidePointCount++] = &inTri.v[1];
	}
	else {
		pointsOutside[outsidePointCount++] = &inTri.v[1];
	}
	if (d2 >= 0) {
		pointsInside[insidePointCount++] = &inTri.v[2];
	}
	else {
		pointsOutside[outsidePointCount++] = &inTri.v[2];
	}

	// Classify triangles based on how many vertices are outside the plane
	if (insidePointCount == 0) {
		return 0;
	}
	if (insidePointCount == 3) {
		outTri1 = inTri;
		return 1;
	}
	if (insidePointCount == 1 && outsidePointCount == 2) {
		outTri1.luminance = inTri.luminance;

		// Keep the inside vertex
		outTri1.v[0] = *pointsInside[0];

		// Clip the other two vertices
		outTri1.v[1] = vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[0], *pointsOutside[0]);
		outTri1.v[2] = vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[0], *pointsOutside[1]);

		return 1;
	}
	if (insidePointCount == 2 && outsidePointCount == 1) {
		// Clip triangle, return two new ones
		outTri1.luminance = inTri.luminance;
		outTri2.luminance = inTri.luminance;

		// The first new triangle keeps the two inside vertices
		outTri1.v[0] = *pointsInside[0];
		outTri1.v[1] = *pointsInside[1];
		outTri1.v[2] = vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[0], *pointsOutside[0]);

		// The second new triangle keeps one inside vertex
		outTri2.v[0] = *pointsInside[1];
		outTri2.v[1] = outTri1.v[2];
		outTri2.v[2] = vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[1], *pointsOutside[0]);

		return 2;
	}
}