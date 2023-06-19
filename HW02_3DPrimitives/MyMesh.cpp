#include "MyMesh.h"
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	// if the requested size is too small
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	// Calculate the angle increment for each subdivision on the cone
	float angleIncrement = 360.0f / a_nSubdivisions;

	Release();
	Init();

	// Get half the cone's height
	float halfHeight = a_fHeight / 2.0f;

	// Store positions of the cone's base vertices
	std::vector<vector3> baseVertices;
	float theta = 0.0f; // used to calculate the angle for each vertex
	float delta = static_cast<GLfloat>(2.0 * PI / static_cast<float>(a_nSubdivisions)); // calculates the angular difference between each vertex on the base of the cone
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Calculate position of vertices on the base of the cone
		vector3 baseVertex(cos(theta) * a_fRadius, sin(theta) * a_fRadius, -halfHeight);
		theta += delta;
		baseVertices.push_back(baseVertex);
	}

	// Sides of the cone
	vector3 apex(0.0f, 0.0f, halfHeight); // apex of the cone
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(baseVertices[i], baseVertices[nextIndex], apex);
	}

	// Bottom of the cone
	vector3 center(0.0f, 0.0f, -halfHeight); // center of the bottom
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(baseVertices[nextIndex], baseVertices[i], center);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Calculate the angle increment for each subdivision on the cylinder
	float angleIncrement = 360.0f / a_nSubdivisions;

	// Get half the cylinder's height
	float halfHeight = a_fHeight / 2.0f;

	// Store positions of the cylinder's vertices
	std::vector<vector3> topVertices;
	std::vector<vector3> bottomVertices;
	float theta = 0.0f; // used to calculate the angle for each vertex
	float delta = static_cast<float>(2.0 * PI / static_cast<float>(a_nSubdivisions)); // calculates the angular difference between each vertex on the base of the cylinder
	
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Calculate position of vertices on the top and bottom bases of the cylinder
		vector3 topVertex(cos(theta) * a_fRadius, sin(theta) * a_fRadius, halfHeight);
		vector3 bottomVertex(cos(theta) * a_fRadius, sin(theta) * a_fRadius, -halfHeight);
		theta += delta;
		topVertices.push_back(topVertex);
		bottomVertices.push_back(bottomVertex);
	}

	// Generate the sides of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddQuad(topVertices[i], topVertices[nextIndex], bottomVertices[nextIndex], bottomVertices[i]);
	}

	// Generate the top base of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(topVertices[i], topVertices[nextIndex], vector3(0.0f, 0.0f, halfHeight));
	}

	// Generate the bottom base of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(bottomVertices[i], vector3(0.0f, 0.0f, -halfHeight), bottomVertices[nextIndex]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Calculate the angle increment for each subdivision on the cylinder
	float angleIncrement = 360.0f / a_nSubdivisions;

	// Get half the cylinder's height
	float halfHeight = a_fHeight / 2.0f;

	// Store positions of the cylinder's vertices
	std::vector<vector3> outerTVertices;
	std::vector<vector3> outerBVertices;
	std::vector<vector3> innerTVertices;
	std::vector<vector3> innerBVertices;
	GLfloat theta = 0.0f; // used to calculate the angle for each vertex
	GLfloat delta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisions)); // calculates the angular difference between each vertex on the base of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Calculate position of vertices on the outer and inner surfaces of the cylinder
		vector3 outerTopVertex(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, halfHeight);
		vector3 outerBottomVertex(cos(theta) * a_fOuterRadius, sin(theta) * a_fOuterRadius, -halfHeight);
		vector3 innerTopVertex(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, halfHeight);
		vector3 innerBottomVertex(cos(theta) * a_fInnerRadius, sin(theta) * a_fInnerRadius, -halfHeight);
		theta += delta;
		outerTVertices.push_back(outerTopVertex);
		outerBVertices.push_back(outerBottomVertex);
		innerTVertices.push_back(innerTopVertex);
		innerBVertices.push_back(innerBottomVertex);
	}

	// Generate the sides of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddQuad(outerTVertices[i], outerTVertices[nextIndex], outerBVertices[nextIndex], outerBVertices[i]);
		AddQuad(innerTVertices[nextIndex], innerTVertices[i], innerBVertices[i], innerBVertices[nextIndex]);
		AddQuad(innerTVertices[i], innerTVertices[nextIndex], outerTVertices[nextIndex], outerTVertices[i]);
		AddQuad(outerBVertices[nextIndex], outerBVertices[i], innerBVertices[i], innerBVertices[nextIndex]);
	}

	// Generate the top base of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddQuad(outerTVertices[i], outerTVertices[nextIndex], innerTVertices[nextIndex], innerTVertices[i]);
	}

	// Generate the bottom base of the cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddQuad(innerBVertices[i], innerBVertices[nextIndex], outerBVertices[nextIndex], outerBVertices[i]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Calculate angle increments
	float angleIncrementA = 360.0f / static_cast<float>(a_nSubdivisionsA);
	float angleIncrementB = 360.0f / static_cast<float>(a_nSubdivisionsB);

	// Generate torus vertices
	for (int i = 0; i < a_nSubdivisionsA; ++i)
	{
		float angleA = i * angleIncrementA;

		for (int j = 0; j < a_nSubdivisionsB; ++j)
		{
			float angleB = j * angleIncrementB;

			// Calculate vertex positions
			vector3 vertex1, vertex2, vertex3, vertex4;
			vertex1.x = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB))) * cos(glm::radians(angleA));
			vertex1.y = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB))) * sin(glm::radians(angleA));
			vertex1.z = a_fInnerRadius * sin(glm::radians(angleB));

			vertex2.x = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB + angleIncrementB))) * cos(glm::radians(angleA));
			vertex2.y = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB + angleIncrementB))) * sin(glm::radians(angleA));
			vertex2.z = a_fInnerRadius * sin(glm::radians(angleB + angleIncrementB));

			vertex3.x = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB))) * cos(glm::radians(angleA + angleIncrementA));
			vertex3.y = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB))) * sin(glm::radians(angleA + angleIncrementA));
			vertex3.z = a_fInnerRadius * sin(glm::radians(angleB));

			vertex4.x = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB + angleIncrementB))) * cos(glm::radians(angleA + angleIncrementA));
			vertex4.y = (a_fOuterRadius + a_fInnerRadius * cos(glm::radians(angleB + angleIncrementB))) * sin(glm::radians(angleA + angleIncrementA));
			vertex4.z = a_fInnerRadius * sin(glm::radians(angleB + angleIncrementB));

			// Add triangles or quads based on the subdivision count
			if (a_nSubdivisionsB == 3)
			{
				AddTri(vertex1, vertex2, vertex3);
				AddTri(vertex3, vertex2, vertex4);
			}
			else
			{
				AddQuad(vertex1, vertex2, vertex3, vertex4);
			}
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	// Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	float fDeltaPhi = glm::pi<float>() / static_cast<float>(a_nSubdivisions);
	float fDeltaTheta = glm::two_pi<float>() / static_cast<float>(a_nSubdivisions);

	// Generate vertices
	std::vector<vector3> vertices;
	for (int i = 0; i <= a_nSubdivisions; i++)
	{
		float fPhi = i * fDeltaPhi;

		for (int j = 0; j <= a_nSubdivisions; j++)
		{
			float fTheta = j * fDeltaTheta;

			float x = a_fRadius * sin(fPhi) * cos(fTheta);
			float y = a_fRadius * sin(fPhi) * sin(fTheta);
			float z = a_fRadius * cos(fPhi);

			vertices.push_back(vector3(x, y, z));
		}
	}

	// Generate triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			int i0 = i * (a_nSubdivisions + 1) + j;
			int i1 = i0 + 1;
			int i2 = (i + 1) * (a_nSubdivisions + 1) + j;
			int i3 = i2 + 1;

			AddQuad(vertices[i0], vertices[i1], vertices[i2], vertices[i3]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}