#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Sam Haile - sdh5898@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 30.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);
	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.5f, C_WHITE);
	m_StartingPos = vector3(0.0f, 0.0f, 0.0f);
	m_index = 0;
	m_points.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_points.push_back(vector3(1.0f, -2.0f, 5.0f));
	m_points.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_points.push_back(vector3(2.0f, -1.0f, 3.0f));
	m_points.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_points.push_back(vector3(3.0f, 0.0f, 0.0f));
	m_points.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_points.push_back(vector3(4.0f, 1.0f, -3.0f));
	m_points.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_points.push_back(vector3(5.0f, 2.0f, -5.0f));
	m_points.push_back(vector3(1.0f, 3.0f, -5.0f));
	m_points.push_back(vector3(-4.0f, -2.0f, 5.0f));
}

void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Create a model matrix out of the arcball quaternion
	matrix4 m4Model = ToMatrix4(m_qArcBall);

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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//Timer
	static float elapsedTime = 0;	
	static uint uClock = m_pSystem->GenClock();
	elapsedTime += m_pSystem->GetDeltaTime(uClock); 
	float duration = 1.0f;

	// interpolation factor
	float interpolationFactor = glm::clamp(elapsedTime / duration, 0.0f, 1.0f);
	// Interpolate cubes position between the starting position and the target position
	vector3 interpolatedPosition = glm::mix(m_StartingPos, m_points[m_index], interpolationFactor);
	// Translation matrix
	matrix4 m4Translate = glm::translate(IDENTITY_M4, interpolatedPosition);
	// Scale matrix
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(.75f, .75f, .75f));
	// Model matrix 
	matrix4 m4Model = m4Translate * m4Scale;

	if (interpolationFactor >= 1.0f)
	{	
		if (m_index >= m_points.size() - 1)
		{
			m_index = 0;
		}
		// Reset the elapsed time
		elapsedTime = 0.0f;
		m_StartingPos = m_points[m_index];
		m_index++;
	}

	// draw stops to know we are within stops
	for (uint i = 0; i < m_points.size(); ++i)
		m_pModelMngr->AddSphereToRenderList(glm::translate(m_points[i]) * glm::scale(vector3(.15)), C_RED, RENDER_WIRE);

	m_pMesh->Render(m4Projection, m4View, m4Model);
	
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
	SafeDelete(m_pMesh);
	//release GUI
	ShutdownGUI();
}