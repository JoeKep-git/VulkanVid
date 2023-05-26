#include "Sphere.hpp"
#include <iostream>

namespace lve
{
	std::unique_ptr<Model> Sphere::createSphere(LveDevice& device, int level)
	{
		Model::Builder modelBuilder{};

		vertAmount = ((level - 2) * level + 2) * 3;
		verts = new float[vertAmount];

		NormsAmount = ((level - 2) * level + 2) * 3;
		norms = new float[NormsAmount];

		TindsAmount = (((level - 3) * (level - 1) + (level - 1)) * 2) * 3;
		tInds = new unsigned int[TindsAmount];

		numOfTris = (((level - 3) * (level - 1) + (level - 1)) * 2);
		numOfVerts = ((level - 2) * level + 2);

		std::cout << "Num of tris: " << numOfTris << std::endl;

		//populate the arrays
		float theta, phi;
		int vertCount = 0;
		int i, j, t;

		for (t = 0, j = 1; j < level - 1; j++)
		{
			for (i = 0; i < level; i++)
			{
				theta = float(j) / (level - 1) * PI;
				phi = float(i) / (level - 1) * PI * 2;

				verts[vertCount + t] = ((sinf(theta) * cosf(phi)) * r) + cx;
				verts[vertCount + t + 1] = (cosf(theta) * r) + cy;
				verts[vertCount + t + 2] = ((-sinf(theta) * sinf(phi)) * r) + cz;

				norms[vertCount + t] = (sinf(theta) * cosf(phi));
				norms[vertCount + t + 1] = cosf(theta);
				norms[vertCount + t + 2] = -(sinf(theta) * sinf(phi));

				t += 3;
			}
		}
		verts[vertCount + t] = cx;
		verts[vertCount + t + 1] = r + cy;
		verts[vertCount + t + 2] = cz;

		norms[vertCount + t] = 0;
		norms[vertCount + t + 1] = 1;
		norms[vertCount + t + 2] = 0;
		t += 3;

		verts[vertCount + t] = cx;
		verts[vertCount + t + 1] = -r + cy;
		verts[vertCount + t + 2] = cz;

		norms[vertCount + t] = 0;
		norms[vertCount + t + 1] = -1;
		norms[vertCount + t + 2] = 0;
		t += 3;

		int vc3 = vertCount / 3;
		int triCount = 0;
		for (t = 0, j = 0; j < level - 3; j++)
		{
			for (i = 0; i < level - 1; i++)
			{
				tInds[triCount + t] = vc3 + ((j)*level + i);    t++;
				tInds[triCount + t] = vc3 + ((j + 1) * level + i + 1);    t++;
				tInds[triCount + t] = vc3 + ((j)*level + i + 1);    t++;
				tInds[triCount + t] = vc3 + ((j)*level + i);    t++;
				tInds[triCount + t] = vc3 + ((j + 1) * level + i);    t++;
				tInds[triCount + t] = vc3 + ((j + 1) * level + i + 1);  t++;
			}
		}
		for (i = 0; i < level - 1; i++)
		{
			tInds[triCount + t] = vc3 + ((level - 2) * level);  t++;
			tInds[triCount + t] = vc3 + (i);    t++;
			tInds[triCount + t] = vc3 + (i + 1);    t++;
			tInds[triCount + t] = vc3 + ((level - 2) * level + 1);    t++;
			tInds[triCount + t] = vc3 + ((level - 3) * level + i + 1);    t++;
			tInds[triCount + t] = vc3 + ((level - 3) * level + i);  t++;
		}
		triCount += t;

		for (int x = 0; x < vertAmount; x += 3)
		{
			modelBuilder.vertices.push_back(Model::Vertex(glm::vec3(verts[x], verts[x + 1], verts[x + 2]), glm::vec3(1.f, 1.f, 1.f)));
		}

		for (int x = 0; x < TindsAmount; x++)
		{
			modelBuilder.indices.push_back((uint32_t)tInds[x]);
		}

		for (auto& v : modelBuilder.vertices)
		{
			v.position += glm::vec3(.0f, .0f, .0f);
		}

		return std::make_unique<Model>(device, modelBuilder);
	}
	
	Sphere::Sphere()
	{
		cx = cy = cz = 0.0;
		r = 0.5;

		numOfVerts = 0;
		numOfTris = 0;

		verts = nullptr;
		norms = nullptr;
		tInds = nullptr;
	}

	void Sphere::setRadius(float rad)
	{
		r = rad;
	}

	void Sphere::setCentre(float x, float y, float z)
	{
		cx = x;
		cy = y;
		cz = z;
	}

	glm::vec3 Sphere::getCentre()
	{
		return glm::vec3(cx, cy, cz);
	}

	float Sphere::getRadius()
	{
		return r;
	}
}