#include "stdafx.h"
#include "Graphicsplosion.h"
#include "Time.h"
#include "render/Model.h" // Test model load

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

Graphicsplosion game;

Texture dbgTexture;
Model dbgModel;
VertexBuffer dbgModelBuffer;

void Graphicsplosion::Init() {
	// Create window and initialise renderer
	window.Create("Zomg, it's a game! M: Unlock mouse F: Fullscreen and pay respects", Vec2I(640, 480));
	window.SetMouseLock(true);

	render.Init(window);

	dbgTexture.Create(render, "texture.jpg");

	// Load the default shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Init(render, vertexShader, fragmentShader);

	// Generate the test triangle
	static Vertex g_vertex_buffer_data[] = {
		-1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f,

		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};

	// Create the test triangle
	triangle.Create(render, g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
	
	Model derp;
	dbgModel.Create("Assets/Bunny.fbx");

	// Spawn the player
	player.OnSpawn();
}

void Graphicsplosion::Shutdown() {
	// Clean up the test vertex buffer
	triangle.Destroy();

	dbgTexture.Destroy();
}

void Graphicsplosion::Update() {
	// Update the player
	player.Update(deltaTime);
}

void Graphicsplosion::Render() {
	// Render here
	render.BeginRender();

	// Use our test shader
	render.UseShaderProgram(defaultShaderProgram);

	// Test rotate the triangle
	triangleAngle += 6.28f * deltaTime;

	// Find the shader constants
	int uniMatWorld = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matWorld");
	int uniMatViewProj = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matViewProj");
	int uniTime = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "time");
	int uniTexture = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "textureSampler");

	// Set up the view/proj matrix
	const float playerHeight = 0.5f;
	glm::vec3 playerEye = player.GetPosition() + player.GetUp() * playerHeight;

	glm::mat4 matViewProj = glm::lookAtRH(playerEye, playerEye + player.GetForward(), player.GetUp());
	matViewProj = glm::perspectiveFov(70.0f, (float)window.GetSize().x, (float)window.GetSize().y, 0.1f, 100.0f) * matViewProj;

	// Set up the world matrix (just a fun rotation around angle)
	glm::mat4 matWorld = glm::rotate(triangleAngle, glm::vec3(0.0f, 0.0f, 1.0f));

	// Upload the uniform variables
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, (GLfloat*)&matWorld);
	glUniformMatrix4fv(uniMatViewProj, 1, GL_FALSE, (GLfloat*)&matViewProj);
	glUniform1f(uniTime, (float)game.frameTime);
	glUniform1i(uniTexture, 0);

	// Set the texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dbgTexture.GetTextureName());
	glBindSampler(uniTexture, 1);

	// Draw the model
	dbgModel.Render(render);

	// Draw the triangle
	render.UseVertexBuffer(&triangle);
	render.UseIndexBuffer(nullptr);

	render.DrawTriangles(0, 36);

	// Done!
	render.EndRender(window);
}