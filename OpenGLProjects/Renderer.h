#pragma once

struct InitDesc
{
	float width;
	float height;
};





class Renderer
{
public:
	Renderer(InitDesc initDesc);

	void SetViewMatrix(const glm::vec3& cameraPosition, const glm::vec3& lookDir, const glm::vec3& cameraUp);
	void SetProjMatrix( float fov ,  float nearZ,  float farZ);
	void SetCurrentShader(const char* shaderName) /*inline*/ {
		glUseProgram(shaderProgramMap[shaderName]);
		currentShaderID = shaderProgramMap[shaderName]; 
	};


	bool AddShaderWithTwoParam(const string& vertexShaderPath, const string& pixelShaderPath, const string& shaderName);
	bool AddShaderWithFourParam(const string& vertexShaderPath, const string& pixelShaderPath, const string& shaderName);
	~Renderer();



	void DrawMeshObject(const glm::mat4& finalMatrix, const int primitiveType, const GLuint vao, const int count);


public:
	InitDesc initDesc;

	glm::mat4 viewMatrix = glm::mat4{};
	glm::mat4 projMatrix = glm::mat4{};

	unordered_map<string, GLuint> shaderProgramMap;



private:
	GLuint currentShaderID;


	glm::vec3 cameraPosition;
	glm::vec3 lookDir;
	glm::vec3 cameraUp;
};
