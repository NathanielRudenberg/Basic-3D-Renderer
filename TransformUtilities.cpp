#include "TransformUtilities.h"

mat4 getPointAtMatrix(const vec3& pos, const vec3& target, const vec3& up) {
	vec3 newForward = normalize(target - pos);
	vec3 a = newForward * dot(up, newForward);
	vec3 newUp = normalize(up - a);
	vec3 newRight = normalize(cross(newUp, newForward));

	mat4 pointAt = mat4({ newRight[X], newUp[X], newForward[X], pos[X] },
						{ newRight[Y], newUp[Y], newForward[Y], pos[Y] },
						{ newRight[Z], newUp[Z], newForward[Z], pos[Z] },
						{ 0,		   0,		 0,				1 });

	return pointAt;
}

mat4 getTranslationMatrix(float x, float y, float z) {
	mat4 translation = mat4(1.0f);
	translation[0][3] = x;
	translation[1][3] = y;
	translation[2][3] = z;

	return translation;
}

mat4 getXRot(float theta) {
	mat4 m = mat4(0.0f);
	m[0][0] = 1.0f;
	m[1][1] = cosf(theta);
	m[2][1] = sinf(theta);
	m[1][2] = -sinf(theta);
	m[2][2] = cosf(theta);
	m[3][3] = 1.0f;

	return m;
}

mat4 getYRot(float theta) {
	mat4 m = mat4(0.0f);
	m[0][0] = cosf(theta);
	m[2][0] = -sinf(theta);
	m[1][1] = 1.0f;
	m[0][2] = sinf(theta);
	m[2][2] = cosf(theta);
	m[3][3] = 1.0f;

	return m;
}

mat4 getZRot(float theta) {
	mat4 m = mat4(0.0f);
	m[0][0] = cosf(theta);
	m[1][0] = sinf(theta);
	m[0][1] = -sinf(theta);
	m[1][1] = cosf(theta);
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;

	return m;
}

mat4 getRotationMatrix(float thetaX, float thetaY, float thetaZ) {
	return getZRot(thetaZ) * getYRot(thetaY) * getXRot(thetaX);
}

mat4 getProjectionMatrix(float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	mat4 projMat = mat4(0.0f);
	projMat[0][0] = -(aspectRatio * fovRadians);
	projMat[1][1] = -fovRadians;
	projMat[2][2] = farPlane / (farPlane - nearPlane);
	projMat[2][3] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projMat[3][2] = 1.0f;

	return projMat;
}

vec4 project(const vec4& toProject, float fovRadians, float aspectRatio, float nearPlane, float farPlane) {
	vec4 tmpProj = toProject * getProjectionMatrix(fovRadians, aspectRatio, nearPlane, farPlane);
	vec4 projected;
	if (tmpProj[W] != 0.0f) {
		projected = vec4(tmpProj[X] / tmpProj[W], tmpProj[Y] / tmpProj[W], tmpProj[Z] / tmpProj[W], tmpProj[W]);
	}
	else {
		projected = vec4(tmpProj[X], tmpProj[Y], tmpProj[Z], tmpProj[W]);
	}

	return projected;
}

vec4 vectorPlaneIntersect(Plane& plane, const vec4& lineStart, const vec4& lineEnd) {
	// plane.normal() should already be normalized
	vec4 start = vec4(lineStart[X], lineStart[Y], lineStart[Z], lineStart[W]);
	vec4 end = vec4(lineEnd[X], lineEnd[Y], lineEnd[Z], lineEnd[W]);
	vec4 pN = vec4(plane.normal()[X], plane.normal()[Y], plane.normal()[Z], 0.0f);
	vec4 pPoint = vec4(plane.point()[X], plane.point()[Y], plane.point()[Z], 0.0f);

	float planeD = -1.0f * dot(pN, pPoint);
	float ad = dot(start, pN);
	float bd = dot(end, pN);
	float t = (-planeD - ad) / (bd - ad);
	vec4 lineStartToEnd = end - start;
	vec4 lineToIntersect = lineStartToEnd * t;
	vec4 clippedPoint = vec4(start + lineToIntersect);
	return clippedPoint;
}

int clipTriangleAgainstPlane(Plane& plane, Triangle& inTri, Triangle& outTri1, Triangle& outTri2) {
	// plane.normal() should already be normalized

	// Get shortest distance from point to plane
	auto dist = [&](vec4& n) {
		vec3 p = vec3(n[X], n[Y], n[Z]);

		return (plane.normal()[X] * p[X]
			+ plane.normal()[Y] * p[Y]
			+ plane.normal()[Z] * p[Z]
			- dot(plane.normal(), plane.point()));
	};

	// Classify vertices on either side of the plane
	// Vertices on the inside have a positive sign
	vec4* pointsInside[3]; int insidePointCount = 0;
	vec4* pointsOutside[3]; int outsidePointCount = 0;

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
		outTri1.getVerts()[1] = (vectorPlaneIntersect(plane, *pointsInside[0], *pointsOutside[0]));
		outTri1.getVerts()[2] = (vectorPlaneIntersect(plane, *pointsInside[0], *pointsOutside[1]));

		return 1;
	}
	if (insidePointCount == 2 && outsidePointCount == 1) {
		// Clip triangle, return two new ones
		outTri1.setLuminance(inTri.getLuminance());
		outTri2.setLuminance(inTri.getLuminance());

		// The first new triangle keeps the two inside vertices
		outTri1.getVerts()[0] = (*pointsInside[0]);
		outTri1.getVerts()[1] = (*pointsInside[1]);
		outTri1.getVerts()[2] = (vectorPlaneIntersect(plane, *pointsInside[0], *pointsOutside[0]));

		// The second new triangle keeps one inside vertex
		outTri2.getVerts()[0] = (*pointsInside[1]);
		outTri2.getVerts()[1] = (vectorPlaneIntersect(plane, *pointsInside[1], *pointsOutside[0]));
		outTri2.getVerts()[2] = (outTri1.getVerts()[2]);

		return 2;
	}
}

void transformTriangle(Triangle& tri, const mat4& transformationMatrix) {
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
