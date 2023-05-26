#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "device.hpp"
#include "gameObject.hpp"

namespace lve
{
	class Sphere
	{
	private:
		int numOfVerts;
		int numOfTris;
		int vertAmount;
		int NormsAmount;
		int TindsAmount;

		const double PI = 3.14159265358979323846f;

		float* verts;
		float* norms;
		unsigned int* tInds;

		float cx, cy, cz; //centre of the sphere
		float r;		//radius of the sphere
	public:
		Sphere();
		std::unique_ptr<Model> createSphere(LveDevice& device, int level);
		void setRadius(float rad);
		void setCentre(float x, float y, float z);
		glm::vec3 getCentre();
		float getRadius();
	};
}
