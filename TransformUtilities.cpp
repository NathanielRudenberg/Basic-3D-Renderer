#include "TransformUtilities.h"

Matrix4f getPointAtMatrix(const RowVector3f& pos, const RowVector3f& target, const RowVector3f& up) {
	RowVector3f newForward = (target - pos).normalized();
	RowVector3f a = newForward * up.dot(newForward);
	RowVector3f newUp = (up - a).normalized();
	RowVector3f newRight = newUp.cross(newForward).normalized();

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

Matrix4f getRotationMatrix(float thetaX, float thetaY, float thetaZ) {
	return getZRot(thetaZ) * getYRot(thetaY) * getXRot(thetaX);
}

Matrix4f getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	Matrix4f projMat = Matrix4f::Zero();
	projMat(0, 0) = -(aspectRatio * fovRadians);
	projMat(1, 1) = -fovRadians;
	projMat(2, 2) = farPlane / (farPlane - nearPlane);
	projMat(3, 2) = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projMat(2, 3) = 1.0f;

	return projMat;
}

RowVector4f project(const RowVector4f& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	RowVector4f tmpProj = toProject * getProjectionMatrix(fovRadians, aspectRatio, nearPlane, farPlane);
	RowVector4f projected;
	if (tmpProj[coordIndices::W] != 0.0f) {
		projected << tmpProj[coordIndices::X] / tmpProj[coordIndices::W], tmpProj[coordIndices::Y] / tmpProj[coordIndices::W], tmpProj[coordIndices::Z] / tmpProj[coordIndices::W], tmpProj[coordIndices::W];
	}
	else {
		projected << tmpProj[coordIndices::X], tmpProj[coordIndices::Y], tmpProj[coordIndices::Z], tmpProj[coordIndices::W];
	}

	return projected;
}

RowVector4f vectorPlaneIntersect(const RowVector3f& planePoint, const RowVector3f& planeNormal, const RowVector4f& lineStart, const RowVector4f& lineEnd) {
	//planeNormal.normalize();
	// RowVector3f pN;
	RowVector4f start, end, clippedPoint, pPoint, pN;
	start << lineStart[coordIndices::X], lineStart[coordIndices::Y], lineStart[coordIndices::Z], lineStart[coordIndices::W];
	end << lineEnd[coordIndices::X], lineEnd[coordIndices::Y], lineEnd[coordIndices::Z], lineEnd[coordIndices::W];
	pN << planeNormal.normalized(), 0.0f;
	pPoint << planePoint, 0.0f;

	float planeD = -1.0f * pN.dot(pPoint);
	float ad = start.dot(pN);
	float bd = end.dot(pN);
	float t = (-planeD - ad) / (bd - ad);
	RowVector4f lineStartToEnd = end - start;
	RowVector4f lineToIntersect = lineStartToEnd * t;
	clippedPoint << start + lineToIntersect;
	return clippedPoint;
}

int clipTriangleAgainstPlane(const RowVector3f& planePoint, const RowVector3f& pN, Triangle& inTri, Triangle& outTri1, Triangle& outTri2) {
	RowVector3f planeNormal = pN.normalized();

	// Get shortest distance from point to plane
	auto dist = [&](RowVector4f& n) {
		RowVector3f p;
		p << n[coordIndices::X], n[coordIndices::Y], n[coordIndices::Z];

		return (planeNormal[coordIndices::X] * p[coordIndices::X]
			+ planeNormal[coordIndices::Y] * p[coordIndices::Y]
			+ planeNormal[coordIndices::Z] * p[coordIndices::Z]
			- planeNormal.dot(planePoint));
	};

	// Classify vertices on either side of the plane
	// Vertices on the inside have a positive sign
	RowVector4f* pointsInside[3]; int insidePointCount = 0;
	RowVector4f* pointsOutside[3]; int outsidePointCount = 0;

	// Get distance of each triangle vertex to plane
	float d0 = dist(inTri.getVerts()[0]);
	float d1 = dist(inTri.getVerts()[1]);
	float d2 = dist(inTri.getVerts()[2]);

	if (d0 >= 0) {
		pointsInside[insidePointCount++] = &inTri.getVerts()[0];
	}
	else {
		pointsOutside[outsidePointCount++] = &inTri.getVerts()[0];
	}
	if (d1 >= 0) {
		pointsInside[insidePointCount++] = &inTri.getVerts()[1];
	}
	else {
		pointsOutside[outsidePointCount++] = &inTri.getVerts()[1];
	}
	if (d2 >= 0) {
		pointsInside[insidePointCount++] = &inTri.getVerts()[2];
	}
	else {
		pointsOutside[outsidePointCount++] = &inTri.getVerts()[2];
	}

	if (d0 >= 0 && d1 < 0 && d2 >= 0) {
		std::swap(pointsInside[0], pointsInside[1]);
	}
	if(d0 < 0 && d1 >= 0 && d2 < 0) {
		std::swap(pointsOutside[0], pointsOutside[1]);
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
		outTri1.setLuminance(inTri.getLuminance());

		// Keep the inside vertex
		outTri1.getVerts()[0] = (*pointsInside[0]);

		// Clip the other two vertices
		outTri1.getVerts()[1] = (vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[0], *pointsOutside[0]));
		outTri1.getVerts()[2] = (vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[0], *pointsOutside[1]));

		return 1;
	}
	if (insidePointCount == 2 && outsidePointCount == 1) {
		// Clip triangle, return two new ones
		outTri1.setLuminance(inTri.getLuminance());
		outTri2.setLuminance(inTri.getLuminance());

		// The first new triangle keeps the two inside vertices
		outTri1.getVerts()[0] = (*pointsInside[0]);
		outTri1.getVerts()[1] = (*pointsInside[1]);
		outTri1.getVerts()[2] = (vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[0], *pointsOutside[0]));

		// The second new triangle keeps one inside vertex
		outTri2.getVerts()[0] = (*pointsInside[1]);
		outTri2.getVerts()[1] = (vectorPlaneIntersect(planePoint, planeNormal, *pointsInside[1], *pointsOutside[0]));
		outTri2.getVerts()[2] = (outTri1.getVerts()[2]);

		return 2;
	}
}

void transformTriangle(Triangle& tri, const Matrix4f& transformationMatrix) {
	for (int i = 0; i < 3; i++) {
		tri.getVerts()[i] = tri.getVerts()[i] * transformationMatrix;
	}
}

void projectTriangle(Triangle& tri, int width, int height, Plane& nearPlane, Plane& farPlane) {
	float fov = 80.0f;
	float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);
	float aspectRatio = (float)height / (float)width;

	for (int i = 0; i < 3; i++) {
		// Project onto the screen
		tri.getVerts()[i] = project(tri.getVerts()[i], fovRad, aspectRatio, nearPlane.point()[Z] /*near dist*/, farPlane.point()[Z]) /*far dist*/;

		// Normalize and scale into view
		tri.getVerts()[i][X] += 1.0f;
		tri.getVerts()[i][Y] += 1.0f;

		tri.getVerts()[i][X] *= 0.5f * (float)width;
		tri.getVerts()[i][Y] *= 0.5f * (float)height;
	}
}
