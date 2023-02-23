#include "Renderer.h"
#include "GeometryNode.h"
#include "Tools.h"
#include "ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "OBJLoader.h"
#include <cmath>
#include <algorithm>
#include <array>
#include <iostream>
#include "windows.h" 

// RENDERER
Renderer::Renderer()
{
	this->m_nodes = {};
	this->m_collidables_nodes = {};
	this->m_continous_time = 0.0;
}

Renderer::~Renderer()
{
	glDeleteTextures(1, &m_fbo_depth_texture);
	glDeleteTextures(1, &m_fbo_pos_texture);
	glDeleteTextures(1, &m_fbo_normal_texture);
	glDeleteTextures(1, &m_fbo_albedo_texture);
	glDeleteTextures(1, &m_fbo_mask_texture);

	glDeleteFramebuffers(1, &m_fbo);

	glDeleteVertexArrays(1, &m_vao_fbo);
	glDeleteBuffers(1, &m_vbo_fbo_vertices);
}

bool Renderer::Init(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	this->m_screen_width = SCREEN_WIDTH;
	this->m_screen_height = SCREEN_HEIGHT;

	bool techniques_initialization = InitShaders();

	bool meshes_initialization = InitGeometricMeshes();

	bool common_initialization = InitCommonItems();
	bool inter_buffers_initialization = InitIntermediateBuffers();

	//If there was any errors
	if (Tools::CheckGLError() != GL_NO_ERROR)
	{
		printf("Exiting with error at Renderer::Init\n");
		return false;
	}

	this->BuildWorld();
	this->InitHero();
	this->InitCamera();
	bool light_initialization = InitLights();

	//If everything initialized
	return techniques_initialization && meshes_initialization &&
		common_initialization && inter_buffers_initialization;
}

void Renderer::BuildWorld()
{
	GeometryNode& hero = *this->m_nodes[0];
	GeometryNode& simple_room_1 = *this->m_nodes[1];
	GeometryNode& narrow_cor_1 = *this->m_nodes[2];
	GeometryNode& narrow_cor_2 = *this->m_nodes[3];
	GeometryNode& narrow_cor_cross_1 = *this->m_nodes[4];
	GeometryNode& narrow_cor_t_1 = *this->m_nodes[5];
	GeometryNode& medium_cor_1 = *this->m_nodes[6];
	GeometryNode& medium_cor_2 = *this->m_nodes[7];
	GeometryNode& simple_room_2 = *this->m_nodes[8];
	GeometryNode& narrow_cor_3 = *this->m_nodes[9];
	GeometryNode& simple_room_med_1 = *this->m_nodes[10];
	GeometryNode& narrow_cor_4 = *this->m_nodes[11];
	GeometryNode& narrow_cor_t_2 = *this->m_nodes[12];
	GeometryNode& hall = *this->m_nodes[13];
	GeometryNode& wall1 = *this->m_nodes[14];
	GeometryNode& wall2 = *this->m_nodes[15];
	GeometryNode& wall3 = *this->m_nodes[16];
	GeometryNode& wall4 = *this->m_nodes[17];
	GeometryNode& narrow_cor_5 = *this->m_nodes[18];
	GeometryNode& narrow_cor_t_3 = *this->m_nodes[19];
	GeometryNode& narrow_cor_6 = *this->m_nodes[20];
	GeometryNode& narrow_cor_7 = *this->m_nodes[21];
	GeometryNode& narrow_cor_8 = *this->m_nodes[22];
	GeometryNode& narrow_cor_9 = *this->m_nodes[23];
	GeometryNode& narrow_cor_10 = *this->m_nodes[24];
	GeometryNode& narrow_cor_t_4 = *this->m_nodes[25];
	GeometryNode& narrow_cor_11 = *this->m_nodes[26];
	GeometryNode& simple_room_med_2 = *this->m_nodes[27];
	GeometryNode& totem = *this->m_nodes[28];
	GeometryNode& pedestal1 = *this->m_nodes[29];
	GeometryNode& pedestal2 = *this->m_nodes[30];
	GeometryNode& dragon1 = *this->m_nodes[31];
	GeometryNode& dragon2 = *this->m_nodes[32];
	GeometryNode& well = *this->m_nodes[33];
	GeometryNode& trap1 = *this->m_nodes[34];
	GeometryNode& trap2 = *this->m_nodes[35];
	GeometryNode& trap3 = *this->m_nodes[36];
	GeometryNode& spikes1 = *this->m_nodes[37];
	GeometryNode& spikes2 = *this->m_nodes[38];
	GeometryNode& spikes3 = *this->m_nodes[39];
	GeometryNode& arrow = *this->m_nodes[40];
	GeometryNode& arrowOPP = *this->m_nodes[41];
	GeometryNode& door1 = *this->m_nodes[42];

	simple_room_1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)) * glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0, 1.f, 0.f));
	hero.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
	narrow_cor_1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.0f));
	narrow_cor_2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
	narrow_cor_cross_1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -7.0f));
	narrow_cor_t_1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.0f, 0.f, -7.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0, 1.f, 0.f));
	medium_cor_1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -11.0f));
	medium_cor_2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -13.0f));
	simple_room_2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -16.0f));
	narrow_cor_3.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(8.f, 0.f, -7.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));
	simple_room_med_1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(12.f, 0.f, -7.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1.f, 0.f));
	narrow_cor_4.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-4.f, 0.f, -7.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));;
	narrow_cor_t_2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.0f, 0.f, -7.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));
	hall.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.0f, 0.f, -11.0f));
	wall1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-3.0f, 0.f, -10.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));
	wall2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-3.0f, 0.f, -14.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));
	wall3.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-9.5f, 0.f, -10.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));
	wall4.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-9.5f, 0.f, -14.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 1.f, 0.f));
	narrow_cor_5.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 0.f, -17.0f));
	narrow_cor_t_3.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.0f, 0.f, -19.0f));
	narrow_cor_6.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -9.0f));
	narrow_cor_7.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -11.0f));
	narrow_cor_8.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -13.0f));
	narrow_cor_9.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -15.0f));
	narrow_cor_10.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -17.0f));
	narrow_cor_t_4.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -19.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1.f, 0.f));
	narrow_cor_11.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-4.f, 0.f, -19.0f)) * glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1.f, 0.f));
	simple_room_med_2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -25.0f));
	totem.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 0.f, -12.f));
	pedestal1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -16.0f));
	pedestal2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -26.0f));
	dragon1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -16.0f));
	dragon2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -26.0f));
	well.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 0.f, -12.f));
	trap1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -12.f));
	trap2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-4.5f, 0.f, -12.f));
	trap3.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-7.5f, 0.f, -12.f));
	spikes1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 0.f, -12.f));
	spikes2.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-4.5f, 0.f, -12.f));
	spikes3.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-7.5f, 0.f, -12.f));
	arrow.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 0.f, -12.f));
	arrowOPP.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 0.f, -12.f)) * glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0, 1.f, 0.f));
	door1.app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(9.5f, 0.f, -7.39f)) * glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1.f, 0.f));


	// Initialize m_border_map with 1000 values (1000) means the hero is not free to move
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			m_border_map[i][j].reset();
			m_border_map[i][j].set(3);
		}
	}
}

void Renderer::InitCamera()
{
	this->m_camera_position = glm::vec3(0, 2, 3);
	this->m_camera_target_position = glm::vec3(0, 1.5, 0);
	this->m_camera_up_vector = glm::vec3(0, 1, 0);

	this->m_view_matrix = glm::lookAt(
		this->m_camera_position,
		this->m_hero_position,
		m_camera_up_vector);

	this->m_projection_matrix = glm::perspective(
		glm::radians(45.f),
		this->m_screen_width / (float)this->m_screen_height,
		0.1f, 100.f);
}

void Renderer::InitHero() {
	this->m_hero_position = glm::vec3(0.f, 0.f, 0.f);
	this->m_hero_rotation = 0.f;
}


bool Renderer::InitLights()
{
	this->m_spotlight.SetColor(glm::vec3(50.f));
	this->m_spotlight.SetPosition(glm::vec3(-6.f, 2.f, -12.f));
	this->m_spotlight.SetTarget(glm::vec3(m_spotlight.GetPosition().x, 0.f, m_spotlight.GetPosition().z));
	this->m_spotlight.SetConeSize(90, 90);
	this->m_spotlight.CastShadow(false);

	this->m_light.SetColor(glm::vec3(50.f));
	this->m_light.SetPosition(m_camera_position);
	this->m_light.SetTarget(hero_pos);
	this->m_light.SetConeSize(90, 90);
	this->m_light.CastShadow(false);


	return true;
}

bool Renderer::InitShaders()
{
	std::string vertex_shader_path = "Assets/Shaders/geometry pass.vert";
	std::string geometry_shader_path = "Assets/Shaders/geometry pass.geom";
	std::string fragment_shader_path = "Assets/Shaders/geometry pass.frag";

	m_geometry_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_geometry_program.LoadGeometryShaderFromFile(geometry_shader_path.c_str());
	m_geometry_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_geometry_program.CreateProgram();

	vertex_shader_path = "Assets/Shaders/deferred pass.vert";
	fragment_shader_path = "Assets/Shaders/deferred pass.frag";

	m_deferred_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_deferred_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_deferred_program.CreateProgram();

	vertex_shader_path = "Assets/Shaders/post_process.vert";
	fragment_shader_path = "Assets/Shaders/post_process.frag";

	m_post_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_post_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_post_program.CreateProgram();

	vertex_shader_path = "Assets/Shaders/shadow_map_rendering.vert";
	fragment_shader_path = "Assets/Shaders/shadow_map_rendering.frag";

	m_spot_light_shadow_map_program.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	m_spot_light_shadow_map_program.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	m_spot_light_shadow_map_program.CreateProgram();

	return true;
}

bool Renderer::InitIntermediateBuffers()
{
	glGenTextures(1, &m_fbo_depth_texture);
	glGenTextures(1, &m_fbo_pos_texture);
	glGenTextures(1, &m_fbo_normal_texture);
	glGenTextures(1, &m_fbo_albedo_texture);
	glGenTextures(1, &m_fbo_mask_texture);
	glGenTextures(1, &m_fbo_texture);

	glGenFramebuffers(1, &m_fbo);

	return ResizeBuffers(m_screen_width, m_screen_height);
}

bool Renderer::ResizeBuffers(int width, int height)
{
	m_screen_width = width;
	m_screen_height = height;

	// texture
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_fbo_pos_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_fbo_normal_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_fbo_albedo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_fbo_mask_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_fbo_depth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// framebuffer to link to everything together
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_pos_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_fbo_normal_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_fbo_albedo_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_fbo_mask_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fbo_depth_texture, 0);

	GLenum status = Tools::CheckFramebufferStatus(m_fbo);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool Renderer::InitCommonItems()
{
	glGenVertexArrays(1, &m_vao_fbo);
	glBindVertexArray(m_vao_fbo);

	GLfloat fbo_vertices[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1, };

	glGenBuffers(1, &m_vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	return true;
}

bool Renderer::InitGeometricMeshes()
{
	std::vector<const char*> assets = {
		"Assets/Dungeon/Warrior.obj",
		"Assets/Dungeon/Room1_Simple_Small.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow_Cross.obj",
		"Assets/Dungeon/Corridor1_Narrow_T_Junction.obj",
		"Assets/Dungeon/Corridor1_Medium.obj",
		"Assets/Dungeon/Corridor1_Medium.obj",
		"Assets/Dungeon/Room1_Simple_Small.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Room1_Simple_Medium.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow_Turn.obj",
		"Assets/Dungeon/Hall1.obj",
		"Assets/Dungeon/Wall1_H1.0.obj",
		"Assets/Dungeon/Wall1_H1.0.obj",
		"Assets/Dungeon/Wall1_H1.0.obj",
		"Assets/Dungeon/Wall1_H1.0.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow_Turn.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Corridor1_Narrow_T_Junction.obj",
		"Assets/Dungeon/Corridor1_Narrow.obj",
		"Assets/Dungeon/Room1_Simple_Medium.obj",
		"Assets/Dungeon/Totem.obj",
		"Assets/Dungeon/Pedestal.obj",
		"Assets/Dungeon/Pedestal.obj",
		"Assets/Dungeon/GoldenDragon.obj",
		"Assets/Dungeon/GoldenDragon.obj",
		"Assets/Dungeon/Well.obj",
		"Assets/Dungeon/SpikeTrap.obj",
		"Assets/Dungeon/SpikeTrap.obj",
		"Assets/Dungeon/SpikeTrap.obj",
		"Assets/Dungeon/Spikes.obj",
		"Assets/Dungeon/Spikes.obj",
		"Assets/Dungeon/Spikes.obj",
		"Assets/Dungeon/Arrow.obj",
		"Assets/Dungeon/Arrow.obj",
		"Assets/Dungeon/Door1.obj"
	};

	bool initialized = true;
	OBJLoader loader;

	for (auto& asset : assets)
	{
		GeometricMesh* mesh = loader.load(asset);

		if (mesh != nullptr)
		{
			GeometryNode* node = new GeometryNode();
			node->Init(asset, mesh);
			this->m_nodes.push_back(node);
		}
		else
		{
			initialized = false;
			break;
		}
	}

	return initialized;
}

void Renderer::Update(float dt)
{
	this->UpdateGeometry(dt);
	this->UpdateCamera();
	this->UpdateHero(dt);
	this->UpdateDoor(dt);

	m_continous_time += dt;
	time_elapsed += dt;
}

void Renderer::UpdateGeometry(float dt)
{
	this->m_nodes[31]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(4.f, 1.f, -16.f)) *
		glm::rotate(glm::mat4(1.f), m_continous_time, glm::vec3(0.f, 1.f, 0.f)) *
		glm::translate(glm::mat4(1.f), -this->m_nodes[31]->m_aabb.center) * this->m_nodes[31]->model_matrix;
	this->m_nodes[32]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, -26.f)) *
		glm::rotate(glm::mat4(1.f), m_continous_time, glm::vec3(0.f, 1.f, 0.f)) *
		glm::translate(glm::mat4(1.f), -this->m_nodes[32]->m_aabb.center) * this->m_nodes[32]->model_matrix;
	this->m_nodes[28]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 1.f, -12.f)) *
		glm::rotate(glm::mat4(1.f), m_continous_time * 0.5f, glm::vec3(0.f, 1.f, 0.f)) *
		glm::translate(glm::mat4(1.f), -this->m_nodes[28]->m_aabb.center) * this->m_nodes[28]->model_matrix;
	if (!arrow_fired && time_elapsed >= time_fired)
	{
		this->m_nodes[40]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 1.3f, -12.f)) *
			glm::rotate(glm::mat4(1.f), m_continous_time * 0.5f, glm::vec3(0.f, 1.3f, 0.f)) *
			glm::translate(glm::mat4(1.f), -this->m_nodes[40]->m_aabb.center) * this->m_nodes[40]->model_matrix;
		this->m_nodes[41]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 1.3f, -12.f)) *
			glm::rotate(glm::mat4(1.f), m_continous_time * 0.5f + glm::radians(180.f), glm::vec3(0.f, 1.3f, 0.f)) *
			glm::translate(glm::mat4(1.f), -this->m_nodes[41]->m_aabb.center) * this->m_nodes[41]->model_matrix;

		arrow_fired = true;
	}


	ThrowArrow(dt);
	SpikeEnable(dt);


}
void Renderer::ThrowArrow(float dt) {

	if (m_ttl > 0.f) {
		this->m_nodes[40]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -this->m_nodes[40]->app_model_matrix[1].z + dt * 2));
		this->m_nodes[41]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, this->m_nodes[41]->app_model_matrix[1].z + dt * 2));
		//std::cout <<"Z" << this->m_nodes[38]->app_model_matrix[3].z << std::endl;
		m_ttl -= dt;

	}
	else if (m_ttl <= 0.f) {
		arrow_fired = false;
		m_ttl = 2.f;
		time_elapsed = 0.f;
	}
}

void Renderer::SpikeEnable(float dt) {


	if (!spike_up)
	{

		if (this->m_nodes[37]->app_model_matrix[3].y <= 1.5 && this->m_nodes[38]->app_model_matrix[3].y && this->m_nodes[39]->app_model_matrix[3].y) {
			this->m_nodes[37]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, this->m_nodes[37]->app_model_matrix[0].y + dt * 8, 0.f));
			this->m_nodes[38]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, this->m_nodes[38]->app_model_matrix[0].y + dt * 8, 0.f));
			this->m_nodes[39]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, this->m_nodes[39]->app_model_matrix[0].y + dt * 8, 0.f));
		}
		else {
			time_spike_up += dt;
			if (time_spike_up >= 2)
			{
				spike_up = true;
				time_spike_up = 0;
			}
		}
	}
	else {

		if (this->m_nodes[37]->app_model_matrix[3].y >= 0 && this->m_nodes[38]->app_model_matrix[3].y >= 0 && this->m_nodes[39]->app_model_matrix[3].y >= 0)
		{
			this->m_nodes[37]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, this->m_nodes[37]->app_model_matrix[0].y - dt * 1.1, 0.f));
			this->m_nodes[38]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, this->m_nodes[38]->app_model_matrix[0].y - dt * 1.1, 0.f));
			this->m_nodes[39]->app_model_matrix *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, this->m_nodes[39]->app_model_matrix[0].y - dt * 1.1, 0.f));

		}
		else {
			time_spike_down += dt;
			if (time_spike_down >= 2.5) {
				spike_up = false;
				time_spike_down = 0;
			}
		}
	}
}


void Renderer::UpdateDoor(float dt) {
	if (canOpen && !opened) {
		if (rotationAngle < 2.8) {
			rotationAngle += dt * 2;
			m_nodes[42]->app_model_matrix *= glm::rotate(glm::mat4(1.f), glm::radians(rotationAngle), glm::vec3(0.f, 1.f, 0.f));
		}
		else {
			std::cout << rotationAngle << std::endl;
			m_nodes[42]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(9.5f, 0.f, -7.39f)) * glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(0, 1.f, 0.f));
			rotationAngle = 2.8;
			opened = true;
		}
	}

	if (canClose && opened) {
		if (rotationAngle > 0) {
			rotationAngle -= dt * 2;
			m_nodes[42]->app_model_matrix *= glm::rotate(glm::mat4(1.f), glm::radians(-rotationAngle), glm::vec3(0.f, 1.f, 0.f));
		}
		else {
			m_nodes[42]->app_model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(9.5f, 0.f, -7.39f)) * glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1.f, 0.f));
			rotationAngle = 0;
			opened = false;
		}
	}

}
void Renderer::UpdateCamera()
{ 
	//updatecamera
	//angle_around_hero -= m_hero_movement.z*1.5;//rotate around player
	float theta = angle_around_hero;
	float horizontaldist = m_camera_distance * cos(glm::radians(pitch));
	float verticaldist = m_camera_distance * sin(glm::radians(pitch));
	float offsetX = horizontaldist * sin(glm::radians(theta));
	float offsetZ = horizontaldist * cos(glm::radians(theta));
	m_camera_position.x = m_hero_position.x - offsetX;
	m_camera_position.z = m_hero_position.z - offsetZ;
	m_camera_position.y = m_hero_position.y + verticaldist +25;
	this->m_light.SetTarget(m_hero_position);
	this->m_light.SetPosition(glm::vec3(m_hero_position.x, 5.f, m_hero_position.z));
	std::cout << "pos x: " << m_hero_position.x << "pos z: " << m_hero_position.z << std::endl;
	m_view_matrix = glm::lookAt(m_camera_position, m_hero_position + glm::vec3(0.f, 1.6f, 0.f), m_camera_up_vector);
	
}

void::Renderer::UpdateHero(float dt) {
	
	//if (m_border_map[]
		glm::vec3 pos_change = glm::vec3((m_hero_movement.x * 1.f * dt) * sin(m_hero_rotation), 0.f, (m_hero_movement.x * 1.f * dt) * cos(m_hero_rotation));
		rot_change = m_hero_movement.z * 2.5 * dt;
		m_hero_position = m_hero_position + pos_change;
		m_hero_rotation = m_hero_rotation + rot_change;
		glm::vec3 pos_change_model = glm::vec3((m_hero_movement.x * 1.f * dt) * sin(rot_change), 0.f, (m_hero_movement.x * 1.f * dt) * cos(rot_change));
		this->m_nodes[0]->app_model_matrix *= glm::translate(glm::mat4(1.f), pos_change_model) * glm::rotate(glm::mat4(1.f), rot_change, glm::vec3(0.f, 1.f, 0.f));
		std::cout << "pos x: " << hero_pos.x<< "pos z: " << hero_pos.z << std::endl;
}

bool Renderer::ReloadShaders()
{
	m_geometry_program.ReloadProgram();
	m_post_program.ReloadProgram();
	m_deferred_program.ReloadProgram();
	m_spot_light_shadow_map_program.ReloadProgram();
	return true;
}

void Renderer::Render()
{
	RenderShadowMaps();
	RenderGeometry();
	RenderDeferredShading();
	RenderPostProcess();

	GLenum error = Tools::CheckGLError();

	if (error != GL_NO_ERROR)
	{
		printf("Reanderer:Draw GL Error\n");
		system("pause");
	}
}

void Renderer::RenderPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 0.f, 0.8f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	m_post_program.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	m_post_program.loadInt("uniform_texture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_spotlight.GetShadowMapDepthTexture());
	m_post_program.loadInt("uniform_shadow_map", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_fbo_pos_texture);
	m_post_program.loadInt("uniform_tex_pos", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_fbo_normal_texture);
	m_post_program.loadInt("uniform_tex_normal", 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_fbo_albedo_texture);
	m_post_program.loadInt("uniform_tex_albedo", 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_fbo_mask_texture);
	m_post_program.loadInt("uniform_tex_mask", 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_fbo_depth_texture);
	m_post_program.loadInt("uniform_tex_depth", 6);

	glBindVertexArray(m_vao_fbo);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_post_program.Unbind();
}

void Renderer::RenderStaticGeometry()
{
	glm::mat4 proj = m_projection_matrix * m_view_matrix * m_world_matrix;

	for (auto& node : this->m_nodes)
	{
		glBindVertexArray(node->m_vao);

		m_geometry_program.loadMat4("uniform_projection_matrix", proj * node->app_model_matrix);
		m_geometry_program.loadMat4("uniform_normal_matrix", glm::transpose(glm::inverse(m_world_matrix * node->app_model_matrix)));
		m_geometry_program.loadMat4("uniform_world_matrix", m_world_matrix * node->app_model_matrix);

		for (int j = 0; j < node->parts.size(); ++j)
		{
			m_geometry_program.loadVec3("uniform_diffuse", node->parts[j].diffuse);
			m_geometry_program.loadVec3("uniform_ambient", node->parts[j].ambient);
			m_geometry_program.loadVec3("uniform_specular", node->parts[j].specular);
			m_geometry_program.loadFloat("uniform_shininess", node->parts[j].shininess);
			m_geometry_program.loadFloat("uniform_metallic", node->parts[j].metallic);
			m_geometry_program.loadInt("uniform_has_tex_diffuse", (node->parts[j].diffuse_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_has_tex_emissive", (node->parts[j].emissive_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_has_tex_mask", (node->parts[j].mask_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_has_tex_normal", (node->parts[j].bump_textureID > 0 || node->parts[j].normal_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_is_tex_bumb", (node->parts[j].bump_textureID > 0) ? 1 : 0);

			glActiveTexture(GL_TEXTURE0);
			m_geometry_program.loadInt("uniform_tex_diffuse", 0);
			glBindTexture(GL_TEXTURE_2D, node->parts[j].diffuse_textureID);

			if (node->parts[j].mask_textureID > 0)
			{
				glActiveTexture(GL_TEXTURE1);
				m_geometry_program.loadInt("uniform_tex_mask", 1);
				glBindTexture(GL_TEXTURE_2D, node->parts[j].mask_textureID);
			}

			if ((node->parts[j].bump_textureID > 0 || node->parts[j].normal_textureID > 0))
			{
				glActiveTexture(GL_TEXTURE2);
				m_geometry_program.loadInt("uniform_tex_normal", 2);
				glBindTexture(GL_TEXTURE_2D, node->parts[j].bump_textureID > 0 ?
					node->parts[j].bump_textureID : node->parts[j].normal_textureID);
			}

			if (node->parts[j].emissive_textureID > 0)
			{
				glActiveTexture(GL_TEXTURE3);
				m_geometry_program.loadInt("uniform_tex_emissive", 3);
				glBindTexture(GL_TEXTURE_2D, node->parts[j].emissive_textureID);
			}

			glDrawArrays(GL_TRIANGLES, node->parts[j].start_offset, node->parts[j].count);
		}

		glBindVertexArray(0);
	}
}

void Renderer::RenderCollidableGeometry()
{
	glm::mat4 proj = m_projection_matrix * m_view_matrix * m_world_matrix;

	glm::vec3 camera_dir = normalize(m_camera_target_position - m_camera_position);

	for (auto& node : this->m_collidables_nodes)
	{
		float_t isectT = 0.f;
		int32_t primID = -1;
		int32_t totalRenderedPrims = 0;

		if (node->intersectRay(m_camera_position, camera_dir, m_world_matrix, isectT, primID)) continue;
		node->intersectRay(m_camera_position, camera_dir, m_world_matrix, isectT, primID);
		node->intersectRay(m_camera_position, camera_dir, m_world_matrix, isectT, primID);

		glBindVertexArray(node->m_vao);

		m_geometry_program.loadMat4("uniform_projection_matrix", proj * node->app_model_matrix);
		m_geometry_program.loadMat4("uniform_normal_matrix", glm::transpose(glm::inverse(m_world_matrix * node->app_model_matrix)));
		m_geometry_program.loadMat4("uniform_world_matrix", m_world_matrix * node->app_model_matrix);
		m_geometry_program.loadFloat("uniform_time", m_continous_time);

		for (int j = 0; j < node->parts.size(); ++j)
		{
			m_geometry_program.loadVec3("uniform_diffuse", node->parts[j].diffuse);
			m_geometry_program.loadVec3("uniform_ambient", node->parts[j].ambient);
			m_geometry_program.loadVec3("uniform_specular", node->parts[j].specular);
			m_geometry_program.loadFloat("uniform_shininess", node->parts[j].shininess);
			m_geometry_program.loadInt("uniform_has_tex_diffuse", (node->parts[j].diffuse_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_has_tex_mask", (node->parts[j].mask_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_has_tex_emissive", (node->parts[j].emissive_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_has_tex_normal", (node->parts[j].bump_textureID > 0 || node->parts[j].normal_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_is_tex_bumb", (node->parts[j].bump_textureID > 0) ? 1 : 0);
			m_geometry_program.loadInt("uniform_prim_id", primID - totalRenderedPrims);

			glActiveTexture(GL_TEXTURE0);
			m_geometry_program.loadInt("uniform_tex_diffuse", 0);
			glBindTexture(GL_TEXTURE_2D, node->parts[j].diffuse_textureID);

			if (node->parts[j].mask_textureID > 0)
			{
				glActiveTexture(GL_TEXTURE1);
				m_geometry_program.loadInt("uniform_tex_mask", 1);
				glBindTexture(GL_TEXTURE_2D, node->parts[j].mask_textureID);
			}

			if ((node->parts[j].bump_textureID > 0 || node->parts[j].normal_textureID > 0))
			{
				glActiveTexture(GL_TEXTURE2);
				m_geometry_program.loadInt("uniform_tex_normal", 2);
				glBindTexture(GL_TEXTURE_2D, node->parts[j].bump_textureID > 0 ?
					node->parts[j].bump_textureID : node->parts[j].normal_textureID);
			}

			if (node->parts[j].emissive_textureID > 0)
			{
				glActiveTexture(GL_TEXTURE3);
				m_geometry_program.loadInt("uniform_tex_emissive", 3);
				glBindTexture(GL_TEXTURE_2D, node->parts[j].emissive_textureID);
			}

			glDrawArrays(GL_TRIANGLES, node->parts[j].start_offset, node->parts[j].count);
			totalRenderedPrims += node->parts[j].count;
		}

		glBindVertexArray(0);
	}
}

void Renderer::RenderDeferredShading()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);

	GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, drawbuffers);

	glViewport(0, 0, m_screen_width, m_screen_height);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glClear(GL_COLOR_BUFFER_BIT);

	m_deferred_program.Bind();

	m_deferred_program.loadVec3("uniform_light_color", m_spotlight.GetColor());
	m_deferred_program.loadVec3("uniform_light_dir", m_spotlight.GetDirection());
	m_deferred_program.loadVec3("uniform_light_pos", m_spotlight.GetPosition());

	m_deferred_program.loadFloat("uniform_light_umbra", m_spotlight.GetUmbra());
	m_deferred_program.loadFloat("uniform_light_penumbra", m_spotlight.GetPenumbra());

	m_deferred_program.loadVec3("uniform_camera_pos", m_camera_position);
	m_deferred_program.loadVec3("uniform_camera_dir", normalize(m_camera_target_position - m_camera_position));

	m_deferred_program.loadMat4("uniform_light_projection_view", m_spotlight.GetProjectionMatrix() * m_spotlight.GetViewMatrix());
	m_deferred_program.loadInt("uniform_cast_shadows", m_spotlight.GetCastShadowsStatus() ? 1 : 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo_pos_texture);
	m_deferred_program.loadInt("uniform_tex_pos", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fbo_normal_texture);
	m_deferred_program.loadInt("uniform_tex_normal", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_fbo_albedo_texture);
	m_deferred_program.loadInt("uniform_tex_albedo", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_fbo_mask_texture);
	m_deferred_program.loadInt("uniform_tex_mask", 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_fbo_depth_texture);
	m_deferred_program.loadInt("uniform_tex_depth", 4);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_spotlight.GetShadowMapDepthTexture());
	m_deferred_program.loadInt("uniform_shadow_map", 10);

	glBindVertexArray(m_vao_fbo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);


	m_deferred_program.loadVec3("uniform_light_color", m_light.GetColor());
	m_deferred_program.loadVec3("uniform_light_dir", m_light.GetDirection());
	m_deferred_program.loadVec3("uniform_light_pos", m_light.GetPosition());

	m_deferred_program.loadFloat("uniform_light_umbra", m_light.GetUmbra());
	m_deferred_program.loadFloat("uniform_light_penumbra", m_light.GetPenumbra());

	m_deferred_program.loadVec3("uniform_camera_pos", m_camera_position);
	m_deferred_program.loadVec3("uniform_camera_dir", normalize(m_camera_target_position - m_camera_position));

	m_deferred_program.loadMat4("uniform_light_projection_view", m_light.GetProjectionMatrix() * m_light.GetViewMatrix());
	m_deferred_program.loadInt("uniform_cast_shadows", m_light.GetCastShadowsStatus() ? 1 : 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo_pos_texture);
	m_deferred_program.loadInt("uniform_tex_pos", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fbo_normal_texture);
	m_deferred_program.loadInt("uniform_tex_normal", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_fbo_albedo_texture);
	m_deferred_program.loadInt("uniform_tex_albedo", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_fbo_mask_texture);
	m_deferred_program.loadInt("uniform_tex_mask", 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_fbo_depth_texture);
	m_deferred_program.loadInt("uniform_tex_depth", 4);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_light.GetShadowMapDepthTexture());
	m_deferred_program.loadInt("uniform_shadow_map", 10);

	glBindVertexArray(m_vao_fbo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	m_deferred_program.Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
}

void Renderer::RenderGeometry()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_pos_texture, 0);

	GLenum drawbuffers[4] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(4, drawbuffers);

	glViewport(0, 0, m_screen_width, m_screen_height);
	glClearColor(0.f, 0.f, 0.8f, 0.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_geometry_program.Bind();
	RenderStaticGeometry();
	auto e = glGetError();
	RenderCollidableGeometry();

	m_geometry_program.Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void Renderer::RenderShadowMaps()
{
	if (m_spotlight.GetCastShadowsStatus())
	{
		int m_depth_texture_resolution = m_spotlight.GetShadowMapResolution();

		glBindFramebuffer(GL_FRAMEBUFFER, m_spotlight.GetShadowMapFBO());
		glViewport(0, 0, m_depth_texture_resolution, m_depth_texture_resolution);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Bind the shadow mapping program
		m_spot_light_shadow_map_program.Bind();

		glm::mat4 proj = m_spotlight.GetProjectionMatrix() * m_spotlight.GetViewMatrix() * m_world_matrix;

		for (auto& node : this->m_nodes)
		{
			glBindVertexArray(node->m_vao);

			m_spot_light_shadow_map_program.loadMat4("uniform_projection_matrix", proj * node->app_model_matrix);

			for (int j = 0; j < node->parts.size(); ++j)
			{
				glDrawArrays(GL_TRIANGLES, node->parts[j].start_offset, node->parts[j].count);
			}

			glBindVertexArray(0);
		}

		glm::vec3 camera_dir = normalize(m_camera_target_position - m_camera_position);
		float_t isectT = 0.f;
		int32_t primID;

		for (auto& node : this->m_collidables_nodes)
		{
			if (node->intersectRay(m_camera_position, camera_dir, m_world_matrix, isectT, primID)) continue;
			node->intersectRay(m_camera_position, camera_dir, m_world_matrix, isectT, primID);

			glBindVertexArray(node->m_vao);

			m_spot_light_shadow_map_program.loadMat4("uniform_projection_matrix", proj * node->app_model_matrix);

			for (int j = 0; j < node->parts.size(); ++j)
			{
				glDrawArrays(GL_TRIANGLES, node->parts[j].start_offset, node->parts[j].count);
			}

			glBindVertexArray(0);
		}

		m_spot_light_shadow_map_program.Unbind();
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Renderer::CameraMoveForward(bool enable)
{
	m_hero_movement.x = (enable) ? -1 : 0;

}
void Renderer::CameraMoveBackWard(bool enable)
{

	m_hero_movement.x = (enable) ? 0.5 : 0;
}

void Renderer::CameraMoveLeft(bool enable)
{
	m_hero_movement.z = (enable) ? 1 : 0;

}
void Renderer::CameraMoveRight(bool enable)
{
	m_hero_movement.z = (enable) ? -1 : 0;
}

void Renderer::CameraLook(glm::vec2 lookDir)
{
	pitch -= lookDir.y * 0.1f;
	angle_around_hero += lookDir.x * 0.1f;

}

void Renderer::CameraZoom(float amount)
{
	if (m_camera_distance >= 2 && m_camera_distance <= 4) {
		m_camera_distance += amount / 4;
		std::cout << m_camera_distance << std::endl;
	}
	else if (m_camera_distance == 1.875) {
		m_camera_distance = 2;
	}
	else if (m_camera_distance == 4.125) {
		m_camera_distance = 4;
	}
}

void Renderer::HeroDoorCheck() {
	std::cout << "z" << hero_pos.x << std::endl;
	if ((hero_pos.x <= m_nodes[42]->app_model_matrix[3].x - 1 && hero_pos.x >= m_nodes[42]->app_model_matrix[3].x - 2 ||//7,5 8,5
		hero_pos.x >= m_nodes[42]->app_model_matrix[3].x + 1 && hero_pos.x <= m_nodes[42]->app_model_matrix[3].x + 2)) {//[10,5 1.5		
		if (!opened) {
			canOpen = true;
			canClose = false;
		}

		if (opened) {
			canClose = true;
			canOpen = false;
		}
	}
}
