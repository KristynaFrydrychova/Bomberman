#pragma once

#include <vector>
#include <string>

#include <glm/fwd.hpp>

struct vertex {
	glm::vec3 position;
	glm::vec2 texcoord;
};

class mesh
{
public:
	std::vector< glm::vec3 > vertices;
	std::vector< GLuint > indices;
	std::vector< glm::vec4 > colors;
	std::vector< glm::vec2 > texcoords;
	std::vector< glm::vec3 > normals;
	GLuint textureID = 0;
	std::string				 name;

	bool indirect = false;
	bool texture_used = false;
	bool normals_used = false;
	bool colors_used = false;

	GLenum primitive_type = GL_POINTS;

	mesh() {};

	void clear(void)
	{
		delete_texture();
		vertices.clear();
		indices.clear();
		texcoords.clear();
		colors.clear();
		normals.clear();
		indirect = false;
		texture_used = false;
		normals_used = false;
		colors_used = false;
		primitive_type = GL_POINTS;
		textureID = 0;
	};
	void delete_texture(void) { if (texture_used) glDeleteTextures(1, &textureID); texture_used = false; };
};

typedef struct  PlayerAvatar {   // player Position
	float       posX;
	float       posY;      //height
	float       posZ;
	float		radius;
	float       move_h_angle;   // Yaw
	float		move_v_angle;   // Pitch
	float		movement_speed;
	mesh		object;
} PlayerAvatar;

bool loadOBJ(mesh & loaded_mesh, std::string obj_path);
bool loadTexture(mesh & out_mesh, std::string texture_path);
bool mesh_init(mesh & out_mesh, std::string obj_path, std::string texture_path);

// simple
void mesh_draw_vertex(mesh & mesh);
void mesh_draw_arrays(mesh & mesh);
void mesh_draw_arrayelement(mesh & mesh);
void mesh_draw_elements(mesh & mesh);

// complex
void mesh_draw(mesh & mesh);

//Player position
PlayerAvatar avatarMoveForward(PlayerAvatar& avatar);
PlayerAvatar avatarMoveBackward(PlayerAvatar& avatar);
PlayerAvatar avatarMoveLeft(PlayerAvatar& avatar);
PlayerAvatar avatarMoveRight(PlayerAvatar& avatar);