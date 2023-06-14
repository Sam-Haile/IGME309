#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	for (int i = 0; i < m_uMeshCount; i++)
	{
		MyMesh* mesh = new MyMesh();
		m_pMeshList.push_back(mesh);
		m_pMeshList[i]->GenerateCube(1.0f, C_BLACK);
	}		
}

void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	static float timer = 0;
	static uint uClock = m_pSystem->GenClock(); 
	timer += static_cast<float>(m_pSystem->GetDeltaTime(uClock));

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	static float horizontal = -10.0f;
	static vector3 v3Position(horizontal, 0.0f, 0.0f);

	v3Position.x = horizontal + timer * 4.0f;
	v3Position.y = 0;
	matrix4 m4Position = glm::translate(vector3(-5.0f, -3.0f, -15.0f)) * glm::translate(v3Position);

	int cubeIndex = 0;
	float xPosition = 0;
	float yPosition = 0;

	// Accessing the elements of the 2D array
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 11; j++) 
		{
			if (arr2D[i][j] == 'x')
			{
				m_pMeshList[cubeIndex]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(xPosition, yPosition, 0.0f)));
				cubeIndex++;
			}
			xPosition++;
		}
		xPosition = 0;
		yPosition--;
	}

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	//SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}