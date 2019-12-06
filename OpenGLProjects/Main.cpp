﻿#include"stdafx.h"
#include"Renderer.h"
#include"MeshObject.h"
#include"Mesh.h"
#include"Physics.h"
#include"Robot.h"


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

Renderer* renderer;
Physics physics;
vector<MeshObject*> objectList;

vector<Robot*> robots;

Robot* player;


unordered_map<string,Mesh*> meshMap;


auto prevTime = chrono::high_resolution_clock::now();

float mouseStartX, mouseStartY, mouseEndX, mouseEndY;

#define DETECT_DISTANCE 6




void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		player->SetMoveDirAndRotate({ 0,0,-1 });
		break;
	case 's':
		player->SetMoveDirAndRotate({ 0,0,1 });
		break;
	case 'a':
		player->SetMoveDirAndRotate({ -1,0,0 });
		break;
	case 'd':
		player->SetMoveDirAndRotate({ 1,0,0 });
		break;
	}
}


void KeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		player->moveDir = { 0,0,0 };
		break;
	case 's':
		player->moveDir = { 0,0,0 };
		break;
	case 'a':
		player->moveDir = { 0,0,0 };
		break;
	case 'd':
		player->moveDir = { 0,0,0 };
		break;
	}
}

void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		//do something here
		break;
	case GLUT_KEY_DOWN:
		//do something here
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	}
}


bool Initialize()
{
	bool check = true;

	InitDesc desc;
	desc.width = WIDTH;
	desc.height = HEIGHT;


	renderer = new Renderer{ desc };
	renderer->AddShaderWithTwoParam("vertex.glsl", "pixel.glsl", "basic");
	renderer->AddShaderWithTwoParam("lineVertex.glsl", "pixel.glsl", "line");
	renderer->AddShaderWithTwoParam("vertexBasic.glsl", "lightPixel.glsl","obj");

	renderer->SetViewMatrix({ 0,20,20 }, { 0,0,0 }, { 0,1,0 });
	renderer->SetProjMatrix(90.f, 0.1f, 100.0f);

	meshMap["Cube"] = new Mesh();
	check = meshMap["Cube"]->CreateMeshByObj("Mesh/cube.obj");

	meshMap["pyramid"] = new Mesh();
	check = meshMap["pyramid"]->CreateMeshByObj("Mesh/pyramid.obj");

	//meshMap["SpaceShip"] = new Mesh();
	//check = meshMap["SpaceShip"]->CreateMeshByObj("Mesh/SpaceShip.obj");



	if (!check)
	{
		cout << "Mesh Load Failed" << endl;
		return false;
	}

	//Floor//
	auto tempObject = new MeshObject();
	BasicObjectDesc objDesc;
	objDesc.primitiveType = GL_TRIANGLES;
	tempObject->color = { 0.1,0.5,0.3 ,1};
	tempObject->Initialize(objDesc, renderer, meshMap["Cube"], { 0,-2 ,0 }, { 0,0,0 }, { 20,1,10});

	objectList.push_back(tempObject);

	glm::vec3 robotsPosition[4] = { {3,0,0},{0,0,7},{4,0,4},{2,0,5} };

	for (int i = 0; i < 4; i++)
	{
		//Robots//
		auto robot = new Robot();
		objDesc.primitiveType = GL_TRIANGLES;
		robot->color = { BASIC_COLOR[rand() % 5] };
		robot->Initialize(objDesc, renderer, meshMap["pyramid"], robotsPosition[i], { 0,0,0 }, {1,1,1 });
		robot->speed = 2.0f;
		robot->SetMoveDirAndRotate(rand() % 4);
		robot->maxBoundary.x = 17;
		robot->maxBoundary.z = 7;
		robot->minBoundary.x = -17;
		robot->minBoundary.z = -7;
		robots.push_back(robot);
	}


	//Robots//
	player = new Robot();
	objDesc.primitiveType = GL_TRIANGLES;
	player->color = { BASIC_COLOR[rand() % 5] };
	player->Initialize(objDesc, renderer, meshMap["pyramid"], {0,0,-5}, { 0,0,0 }, { 1,1,1 });
	player->speed = 5.0f;
	//robot->moveDir = glm::normalize(glm::vec3{ 1,0,0 });


	return true;
}


void CleanUp()
{
	for (auto& data : objectList)
	{
		delete data;
	}

	for (auto iter = meshMap.begin(); iter != meshMap.end(); iter++)
	{
		(*iter).second->Delete();
		delete (*iter).second;
	}

	delete renderer;

}

void MousDrag(int x,int y) {
	float ox;
	float oy;
	convertDeviceXYOpneglXY(x, y, &ox, &oy);
}


void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//좌표계변환
		float ox;
		float oy;
		convertDeviceXYOpneglXY(x, y, &ox, &oy);		
		mouseStartX = ox;
		mouseStartY = oy;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		

	}
}



int main(int argc, char** argv) // 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	srand(time(NULL));

	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	int window = glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	if (!Initialize())
		return false;


	//// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	//glEnable(GL_CULL_FACE);

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(MousDrag);
	glutSpecialFunc(SpecialInput);
	glutMainLoop(); // 이벤트 처리 시작 

	CleanUp();//메모리 해제

	return true;
}



GLvoid drawScene() // 콜백 함수: 출력 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 기본 흰색
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);

	



	std::chrono::duration<double> diff = chrono::high_resolution_clock::now() - prevTime;
	prevTime = chrono::high_resolution_clock::now();


	renderer->SetCurrentShader("obj");//그리기 전에 어떤 쉐이더로 그릴지 꼭 지정해야뎌!
	for(auto& data:objectList)
	{
		data->Update(diff.count());//월드행렬 계산
		data->Render();//쉐이더한테 값 넘겨주고
	}

	player->Update(diff.count());
	player->Render();

	for (auto& data : robots)
	{
		if (glm::length(player->GetPosition() - data->GetPosition()) < DETECT_DISTANCE)
		{
			data->SetMoveDirAndRotate(player->GetPosition() - data->GetPosition());
		}


		data->Update(diff.count());//월드행렬 계산
		data->Render();//쉐이더한테 값 넘겨주고
	}
	



	glutSwapBuffers();
	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기 
{
	glViewport(0, 0, w, h);
}


