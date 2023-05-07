#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; 
layout(location = 2) in vec3 normal; 
layout(location = 3) in vec2 uv; 

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push
{
	mat4 transform; // = projection * view * model
	mat4 normalMatrix;
} push;
//there must be no more than one push constant block statically used per shader entry point

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0,-3.0,-1.0));

const float AMBIENT = 0.02;

void main()
{
	gl_Position = push.transform * vec4(position, 1.0);

	//temporary: only works in certain situations
	//only works correctly if scale is uniform (sx == sy == sz)
	//vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);
	//vec3 normalWorldSpace = normalize((push.modelMatrix * vec4(normal, 0.0).xyz);

	//correct way to transform normals is to use inverse transpose
	//calculating inverse in shader can be expensive and should be avoided
//	mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
//	vec3 normalWorldSpace = normalize(normalMatrix * normal);

	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

	float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

	fragColor = lightIntensity * color;
}