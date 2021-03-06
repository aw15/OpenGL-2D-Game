#pragma once
class Renderer;



struct VertexData
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	void CreateCube();
	void CreateCube(const glm::vec3 color);
	void CreatePyramid();
	void CreateTriangle();
	void CreateRectangle();
	void CreateMeshByVertices(const vector<glm::vec3>& vertex, const vector<glm::vec3>& colors);
	void CreateMeshByObj(const char* path);


	GLuint GetVAO() const { return vao; };

	void Delete();

private:
	void ObjReadFace(vector<string>& tokens, string& s, vector<glm::vec3>& position, vector<glm::vec3>& normal, vector<glm::vec2>& uv);
public:
	int size = 0;
	vector<VertexData> vertices;
	
private:
	vector<glm::vec3> colors;

	GLuint  vao = UINT_MAX;
	GLuint  vbo[2] = {0,0};

};

