#ifndef BIM_ENGINE_RENDERER_H
#define BIM_ENGINE_RENDERER_H

#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "ShaderProgram.h"
#include "GeometryNode.h"
#include "CollidableNode.h"
#include "LightNode.h"
#include <bitset>


class Renderer
{
public:
	// Empty

protected:
	int	m_screen_width, m_screen_height;
	glm::mat4 m_world_matrix;
	glm::mat4 m_view_matrix;
	glm::mat4 m_projection_matrix;

	glm::vec3 m_camera_position;
	glm::vec3 m_camera_target_position;
	glm::vec3 m_camera_up_vector;

	float m_camera_distance = 3.f;
	float angle_around_hero = -180.f;

	float pitch = 70.f;

	glm::vec3 hero_pos;
	glm::vec3 m_hero_position;
	glm::vec3 m_hero_movement;
	glm::vec3 m_hero_direction;
	float rot_change;
	float m_hero_rotation;


	float m_continous_time;

	std::bitset<4> m_border_map[48][60];
	float m_ttl = 2.f;
	bool arrow_fired = false;
	float time_fired = 2.f;
	float time_elapsed;

	bool spike_up = false;
	float time_spike_down = 0;
	float time_spike_up = 0;

	bool canOpen = false;
	bool opened = false;
	float rotationAngle = 0.0f;
	bool canClose = false;

	// Protected Functions
	bool InitShaders();
	bool InitGeometricMeshes();
	bool InitCommonItems();
	bool InitLights();
	bool InitIntermediateBuffers();
	void BuildWorld();
	void InitCamera();
	void InitHero();
	void RenderGeometry();
	void RenderDeferredShading();
	void RenderStaticGeometry();
	void RenderCollidableGeometry();
	void RenderShadowMaps();
	void RenderPostProcess();

	enum OBJECTS
	{
		CRAFT_1 = 0,
		CRAFT_2 = 0,
		SIZE_ALL
	};

	std::vector<GeometryNode*> m_nodes;
	std::vector<CollidableNode*> m_collidables_nodes;

	LightNode		m_light;
	LightNode		m_spotlight;
	ShaderProgram	m_geometry_program;
	ShaderProgram	m_deferred_program;
	ShaderProgram	m_post_program;
	ShaderProgram	m_spot_light_shadow_map_program;

	GLuint m_fbo;
	GLuint m_vao_fbo;
	GLuint m_vbo_fbo_vertices;

	GLuint m_fbo_texture;

	GLuint m_fbo_depth_texture;
	GLuint m_fbo_pos_texture;
	GLuint m_fbo_normal_texture;
	GLuint m_fbo_albedo_texture;
	GLuint m_fbo_mask_texture;

public:

	Renderer();
	~Renderer();
	bool Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void Update(float dt);
	bool ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void UpdateGeometry(float dt);
	void ThrowArrow(float dt);
	void SpikeEnable(float dt);
	void UpdateDoor(float dt);
	void UpdateCamera();
	void UpdateHero(float dt);
	bool ReloadShaders();
	void Render();

	void CameraMoveForward(bool enable);
	void CameraMoveBackWard(bool enable);
	void CameraMoveLeft(bool enable);
	void CameraMoveRight(bool enable);
	void CameraLook(glm::vec2 lookDir);
	void CameraZoom(float amount);

	void HeroDoorCheck();

};

#endif
