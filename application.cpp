#include "application.hpp"
#include "RenderSystem.hpp"
#include "pointLightSystem.hpp"
#include "camera.hpp"
#include "keyboardMovement.hpp"
#include <stdexcept>
#include <array>
#include <chrono>
#include "buffer.hpp"
#include <numeric>
#include "printToFile.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

float currentTicks = 0.0f;
float previousTicks = 0.0f;
float fDeltaTime = 0.0f;
float timeOfTest = 0.0f;
float timeTest = 0.0f;
int framerate = 0;

const double PI = 3.14159265358979323846f;

//float* verts = nullptr;
//float* norms = nullptr;
//unsigned int* tInds = nullptr;


//int numOfVerts;
//int numOfTris;

namespace lve
{
	FirstApplication::FirstApplication()
	{
		globalPool = LveDescriptorPool::Builder(lveDevice)
			.setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	FirstApplication::~FirstApplication()
	{
		
	}

	void FirstApplication::run()
	{
		std::vector <std::unique_ptr<Buffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<Buffer>(
				lveDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		//Buffer globalUboBuffer
		//{
		//	lveDevice,
		//	sizeof(GlobalUbo),
		//	LveSwapChain::MAX_FRAMES_IN_FLIGHT,
		//	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		//	lveDevice.properties.limits.minUniformBufferOffsetAlignment,
		//};

		auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			LveDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		RenderSystem renderSystem{ 
			lveDevice, 
			renderer.getSwapChainRenderPass(), 
			globalSetLayout->getDescriptorSetLayout()};
		PointLightSystem pointLightSystem{
			lveDevice,
			renderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout() };
		Camera camera{};
		//camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
		camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = GameObject::createGameObject();
		viewerObject.transform.translation.z = -2.5f;
		KeyboardMovement cameraController{};

		PrintToFile print{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vWindow.shouldClose())
		{
			//RUNS EVERY FRAME (INPUT FPS CHECKER HERE)
			glfwPollEvents();

			//calculate the deltatime
			currentTicks = std::clock();
			float deltaTicks = (float)(currentTicks - previousTicks);
			previousTicks = currentTicks;

			fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

			timeOfTest += fDeltaTime;

			float MAX_FRAME_TIME = 60.f;

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = 
				std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, frameTime);

			cameraController.moveInPlaneXZ(vWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = renderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = renderer.beginFrame())
			{
				//checks to see if certain amount of seconds has passed in the real world and adds the framerate
				//to the file
				if (timeTest < 1.0)
				{
					timeTest += fDeltaTime;
					framerate++;
				}
				else
				{
					print.printingMethod(framerate);
					if (timeOfTest >= 100.0f)
					{
						vWindow.shouldClose();
						break;
					}
					framerate = 0;
					timeTest = 0.0f;
				}

				int frameIndex = renderer.getFrameIndex();
				FrameInfo frameInfo
				{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects
				};

				//update
				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				ubo.inverseView = camera.getInverseView();
				pointLightSystem.update(frameInfo, ubo);
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				//render
				renderer.beginSwapChainRenderPass(commandBuffer);

				//order matters
				renderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);

				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	std::unique_ptr<Model> createSphere(LveDevice& device, int level)
	{
		Model::Builder modelBuilder{};

		float cx = .0f, cy = .0f, cz = .0f; //centre of sphere
		float r = 2; //radius of spheres
		int vertAmount = ((level - 2) * level + 2) * 3;
		float* verts = new float[vertAmount];

		int NormsAmount = ((level - 2) * level + 2) * 3;
		float* norms = new float[NormsAmount];

		int TindsAmount = (((level - 3) * (level - 1) + (level - 1)) * 2) * 3;
		unsigned int* tInds = new unsigned int[TindsAmount];
		

		int numOfTris = (((level - 3) * (level - 1) + (level - 1)) * 2);
		int numOfVerts = ((level - 2) * level + 2);

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

		cout << "Num of verts: " << numOfVerts << endl;
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
			v.position += glm::vec3(.0f,.0f,.0f);
		}

		return std::make_unique<Model>(device, modelBuilder);
	}

	// temporary helper function, creates a 1x1x1 cube centered at offset
	std::unique_ptr<Model> createCubeModel(LveDevice& device, glm::vec3 offset) 
	{
		Model::Builder modelBuilder{};
		modelBuilder.vertices = 
		{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		};

		for (auto& v : modelBuilder.vertices) 
		{
			v.position += offset;
		}

		modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		return std::make_unique<Model>(device, modelBuilder);
	}

	void FirstApplication::loadGameObjects()
	{
		std::cout << "loading game object\n";
		//std::shared_ptr<Model> model = createCubeModel(lveDevice, { .0f,.0f,.0f });
		std::shared_ptr<Model> model = createSphere(lveDevice, 20);
		//std::shared_ptr<Model> model = Model::createModelFromFile(lveDevice, "models/colored_cube.obj");

		//auto cube = GameObject::createGameObject();
		//cube.model = model;
		////value of translation in position 2 can make the object have a more faceted appearance (polygon look)
		//cube.transform.translation = { .0f,.0f,2.5f };
		////changing scale can squish the object or enlarge
		//cube.transform.scale = { .5f, .5f, .5f };
		//gameObjects.emplace(cube.getId(), std::move(cube));

		auto sphere = GameObject::createGameObject();
		sphere.model = model;
		//value of translation in position 2 can make the object have a more faceted appearance (polygon look)
		sphere.transform.translation = { .0f,.0f,2.5f };
		//changing scale can squish the object or enlarge
		sphere.transform.scale = { 1.f, 1.f, 1.f };
		gameObjects.emplace(sphere.getId(), std::move(sphere));

		//std::shared_ptr<Model> model = Model::createModelFromFile(lveDevice, "models/flat_vase.obj");
		//auto flatVase = GameObject::createGameObject();
		//flatVase.model = model;
		//flatVase.transform.translation = { -.5f,.5f,0.f };
		//flatVase.transform.scale = { 3.f,1.5f,3.f };
		//gameObjects.emplace(flatVase.getId(), std::move(flatVase));

		//model = Model::createModelFromFile(lveDevice, "models/smooth_vase.obj");
		//auto smoothVase = GameObject::createGameObject();
		//smoothVase.model = model;
		//smoothVase.transform.translation = { .5f,.5f,0.f };
		//smoothVase.transform.scale = { 3.f,1.5f,3.f };
		//gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

		//model = Model::createModelFromFile(lveDevice, "models/quad.obj");
		//auto floor = GameObject::createGameObject();
		//floor.model = model;
		//floor.transform.translation = { 0.f,.5f,0.f };
		//floor.transform.scale = { 3.f,1.f,3.f };
		//gameObjects.emplace(floor.getId(), std::move(floor));

		//model = Model::createModelFromFile(lveDevice, "models/sphere3145728.obj");
		//std::shared_ptr<Model> model = Model::createModelFromFile(lveDevice, "models/sphere3145728.obj");
		//auto sphere = GameObject::createGameObject();
		//sphere.model = model;
		//sphere.transform.translation = { 0.f,0.f,0.f };
		//sphere.transform.scale = { 1.f,1.f,1.f };
		//gameObjects.emplace(sphere.getId(), std::move(sphere));

		//Add more lights here as the for loop iterates thorough the size of the array and adds them
		//std::vector<glm::vec3> lightColors
		//{
		//	{1.f, .1f, .1f},
		//	{.1f, .1f, 1.f},
		//	{.1f, 1.f, .1f},
		//	{1.f, 1.f, .1f},
		//	{.1f, 1.f, 1.f},
		//	{1.f, 1.f, 1.f},
		//	{0.5f, 0.5f, 0.5f},
		//	{0.1f,0.1f,0.1f},
		//	{1.f, 0.5f, 0.5f}
		//};
		//
		//for (int i = 0; i < lightColors.size(); i++)
		//{
		//	auto pointLight = GameObject::makePointLight(0.2f);
		//	pointLight.color = lightColors[i];
		//	auto rotateLight = glm::rotate(
		//		glm::mat4(1.f),
		//		(i * glm::two_pi<float>()) / lightColors.size(),
		//		{ 0.f, -1.f, 0.f });
		//	pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
		//	gameObjects.emplace(pointLight.getId(), std::move(pointLight));
		//}
	}
}


