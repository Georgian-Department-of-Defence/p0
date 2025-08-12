#pragma once
#include "Scene.h"
class GroundScene : public Scene
{
public:
	Camera camera;
	Model model;
	Shader hex_shader;
	RenderTexture2D hex_grid_target;

	// Shader uniforms
	int time_loc, res_loc, fg_col_loc, bg_col_loc, hex_res_loc, hex_thickness_loc;
	Vector3 fg_color = { 1.0f, 1.0f, 1.0f };
	Vector3 bg_color = { 0.0f, 0.0f, 0.0f };
	float hex_res = 10.0f;
	float hex_thickness = 0.1f;
	float time = 0.0f;
	float rotation_angle;
	Shader lit_shader;
	Mesh cube_mesh;
	Model model_preview;
	void OnLoad() final;
	void OnUnload() final;

	void OnStart() final;
	void OnStop() final;

	void OnUpdate()final;
	void OnDraw() final;

	void OnDrawDebug() final;
	void OnDrawGui() final;
};
