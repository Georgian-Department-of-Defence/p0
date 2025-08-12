#include "GroundScene.h"
#include "Camera.h"
#include "rlgl.h"

// Im sure this stuff exists else where, but for now,
// GBuffer data
typedef struct GBuffer {
    unsigned int framebuffer;

    unsigned int positionTexture;
    unsigned int normalTexture;
    unsigned int albedoSpecTexture;

    unsigned int depthRenderbuffer;
} GBuffer;

// Deferred mode passes
typedef enum {
    DEFERRED_POSITION,
    DEFERRED_NORMAL,
    DEFERRED_ALBEDO,
    DEFERRED_SHADING
} DeferredMode;
GBuffer gBuffer = { 0 };

//this is from ray lib example, I think I need something like this ???
//gBuffer.framebuffer = rlLoadFramebuffer();
void GroundScene::OnLoad()
{
   
    camera.position = { 5.0f, 4.0f, 5.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

  
    hex_shader = LoadShader(0, TextFormat("assets/shaders/hexagon_grid_raymarch.frag"));
    lit_shader = LoadShader(0, "assets/shaders/lighting.fs");
    
    time_loc = GetShaderLocation(hex_shader, "u_time");
    res_loc = GetShaderLocation(hex_shader, "u_resolution");
    fg_col_loc = GetShaderLocation(hex_shader, "u_fg_col");
    bg_col_loc = GetShaderLocation(hex_shader, "u_bg_col");
    hex_res_loc = GetShaderLocation(hex_shader, "u_hex_res");
    hex_thickness_loc = GetShaderLocation(hex_shader, "u_hex_thickness");

   
    fg_color = { 1.0f, 0.10f, 1.0f };
    bg_color = { 0.0f, 1.0f, 0.0f };
    hex_res = 10.0f;
    hex_thickness = 0.1f;

    
    hex_grid_target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight()); 
    SetTextureFilter(hex_grid_target.texture, TEXTURE_FILTER_BILINEAR);
    cube_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    model_preview = LoadModel("assets/meshes/mech.glb"); //LoadModelFromMesh(cube_mesh);
    model_preview.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = hex_grid_target.texture;
    model_preview.materials[0].shader = lit_shader;
}

void GroundScene::OnUnload()
{
    UnloadShader(hex_shader);
    UnloadRenderTexture(hex_grid_target);
}

void GroundScene::OnStart()
{
    
}

void GroundScene::OnStop()
{
   
}

void GroundScene::OnUpdate()
{
    time = GetTime(); // Store for use in drawing
  
}

void GroundScene::OnDraw()
{
  
    BeginTextureMode(hex_grid_target);
    BeginShaderMode(hex_shader);
    Vector2 resolution = { (float)hex_grid_target.texture.width, (float)hex_grid_target.texture.height };
    ClearBackground(BLACK);
    SetShaderValue(hex_shader, time_loc, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(hex_shader, res_loc, &resolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(hex_shader, fg_col_loc, &fg_color, SHADER_UNIFORM_VEC3);
    SetShaderValue(hex_shader, bg_col_loc, &bg_color, SHADER_UNIFORM_VEC3);
    SetShaderValue(hex_shader, hex_res_loc, &hex_res, SHADER_UNIFORM_FLOAT);
    SetShaderValue(hex_shader, hex_thickness_loc, &hex_thickness, SHADER_UNIFORM_FLOAT);

    
    DrawRectangle(0, 0, resolution.x, resolution.y, WHITE);
    EndShaderMode();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    DrawModel(model_preview, { 0.0f, 0.0f, 0.0f }, 0.20f, WHITE);
    EndMode3D();
    EndDrawing();

}

void GroundScene::OnDrawDebug()
{
}

void GroundScene::OnDrawGui()
{
}
