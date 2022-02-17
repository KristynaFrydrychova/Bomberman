#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh_init.h"
#include "texture.h"

mesh gen_mesh_brick(const float brick_size, const char* cesta)
{
	mesh mesh;
	
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));
	
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));
	
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, 0.0f));
	
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));

	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));
	

	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.textureID = textureInit(cesta, false, false);

	return mesh;
}

/*mesh gen_mesh_cube(void)
{
	/*struct vertex {
		glm::vec3 position;
		glm::vec4 color;
	};
	mesh tmpmesh;
	std::vector<vertex> vertices = {
		{glm::vec3(1,-1,1), glm::vec2(1.0f, 0.0f)}, //F0
		{glm::vec3(1,-1,-1), glm::vec2(0.0f, 0.0f)}, //B1
		{glm::vec3(-1,-1,-1), glm::vec2(1.0f, 0.0f)}, //A2
		{glm::vec3(-1,-1,1), glm::vec2(0.0f, 0.0f)}, //E3
		{glm::vec3(1,1,1), glm::vec2(1.0f, 1.0f)}, //G4
		{glm::vec3(1,1,-1), glm::vec2(0.0f, 1.0f)}, //C5
		{glm::vec3(-1,1,-1), glm::vec2(1.0f, 1.0f)}, //D6 
		{glm::vec3(-1,1,1), glm::vec2(0.0f, 1.0f)}, //H7
		{glm::vec3(1,-1,-1), glm::vec2(0.0f, 0.0f)}, //B1_8
		{glm::vec3(-1,-1,-1), glm::vec2(0.0f, 1.0f)}, //A2_9
		{glm::vec3(1,1,-1), glm::vec2(1.0f, 0.0f)}, //C5_10
		{glm::vec3(-1,1,-1), glm::vec2(0.0f, 0.0f)} //D6_11 
	};
	std::vector<GLuint> indices = { 1,2,6,1,6,5,
									0,1,5,0,5,4,
									10,11,7,10,7,4,
									2,3,7,2,7,6,
									3,0,4,3,4,7,
									0,3,9,0,9,8 };
	cv::Mat image = cv::imread("resources/box.png", cv::IMREAD_UNCHANGED);
	tmpmesh.init(GL_TRIANGLES, vertices, indices, image);

	return tmpmesh;
}*/

mesh gen_mesh_gun(void)
{
	mesh mesh;

	// top
	mesh.texcoords.push_back(glm::vec2(0.93, 0.78));
	mesh.vertices.push_back(glm::vec3(-0.3, -1, 0));
	mesh.texcoords.push_back(glm::vec2(1.0, 0.78));
	mesh.vertices.push_back(glm::vec3(0.3, -1, 0));
	mesh.texcoords.push_back(glm::vec2(1.0, 0.0));
	mesh.vertices.push_back(glm::vec3(0.3, -1, -4));
	mesh.texcoords.push_back(glm::vec2(0.92, 0.0));
	mesh.vertices.push_back(glm::vec3(-0.3, -1, -4));

	//back
	mesh.texcoords.push_back(glm::vec2(0.0, 0.39));
	mesh.vertices.push_back(glm::vec3(-0.3, -1.4, 0.0));
	mesh.texcoords.push_back(glm::vec2(0.07, 0.39));
	mesh.vertices.push_back(glm::vec3(0.3, -1.4, 0.0));
	mesh.texcoords.push_back(glm::vec2(0.07, 0.27));
	mesh.vertices.push_back(glm::vec3(0.3, -1.0, 0.0));
	mesh.texcoords.push_back(glm::vec2(0.0, 0.27));
	mesh.vertices.push_back(glm::vec3(-0.3, -1.0, 0.0));

	//right
	mesh.texcoords.push_back(glm::vec2(0.0, 0.1));
	mesh.vertices.push_back(glm::vec3(0.3, -1.0, 0));
	mesh.texcoords.push_back(glm::vec2(0.75, 0.1));
	mesh.vertices.push_back(glm::vec3(0.3, -1.0, -4));
	mesh.texcoords.push_back(glm::vec2(0.75, 0.2));
	mesh.vertices.push_back(glm::vec3(0.3, -1.4, -4));
	mesh.texcoords.push_back(glm::vec2(0.0, 0.2));
	mesh.vertices.push_back(glm::vec3(0.3, -1.4, 0));

	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.textureID = textureInit("resources/gun.bmp", false, false);

	return mesh;
}

mesh gen_mesh_floor(const cv::Mat& map, const float brick_size)
{
	mesh mesh;

	for (int j = 0; j < map.cols; j++) {
		for (int i = 0; i < map.rows; i++) {
			mesh.texcoords.push_back(glm::vec2(0.0, 1.0));
			mesh.vertices.push_back(glm::vec3(brick_size * j, brick_size + brick_size * i, 0.0f));

			mesh.texcoords.push_back(glm::vec2(1.0, 1.0));
			mesh.vertices.push_back(glm::vec3(brick_size + brick_size * j, brick_size + brick_size * i, 0.0f));

			mesh.texcoords.push_back(glm::vec2(1.0, 0.0));
			mesh.vertices.push_back(glm::vec3(brick_size + brick_size * j, brick_size * i, 0.0f));

			mesh.texcoords.push_back(glm::vec2(0.0, 0.0));
			mesh.vertices.push_back(glm::vec3(brick_size * j, brick_size * i, 0.0f));
		}
	}

	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.textureID = textureInit("resources/trava.png", false, false);

	return mesh;
}

/*/void gen_mesh_magazines(std::vector<mesh>& magazines)
{
	magazines.clear();

	for (int i = 0; i < 6; i++)
	{
		float texture_coord_w0, texture_coord_w1;
		mesh mesh;

		switch (i)
		{
		case 0:
			texture_coord_w0 = 0.27f; texture_coord_w1 = 0.06f;
			break;
		case 1:
			texture_coord_w0 = 0.00f; texture_coord_w1 = 0.32f;
			break;
		case 2:
			texture_coord_w0 = 0.09f; texture_coord_w1 = 0.49f;
			break;
		case 3:
			texture_coord_w0 = 0.27f; texture_coord_w1 = 0.66f;
			break;
		case 4:
			texture_coord_w0 = 0.44f; texture_coord_w1 = 0.83f;
			break;
		case 5:
			texture_coord_w0 = 0.62f; texture_coord_w1 = 1.00f;
			break;
		default:
			std::cerr << "Da fuq?" << std::endl;
			exit(1);
			break;
		}

		mesh.texcoords.push_back(glm::vec2(0.0f, texture_coord_w1));
		mesh.vertices.push_back(glm::vec3(1.7f, -1.0f, 0.0f));
		mesh.texcoords.push_back(glm::vec2(0.45f, texture_coord_w1));
		mesh.vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
		mesh.texcoords.push_back(glm::vec2(0.45f, texture_coord_w0));
		mesh.vertices.push_back(glm::vec3(1.0f, -1.5f, 0.0f));
		mesh.texcoords.push_back(glm::vec2(0.0f, texture_coord_w0));
		mesh.vertices.push_back(glm::vec3(1.7f, -1.5f, 0.0f));

		mesh.primitive_type = GL_QUADS;
		mesh.texture_used = true;

		magazines.push_back(mesh);
	}
}


/**********************************************************************************************************************/

mesh gen_mesh_circle(const float radius, const unsigned int num_segments)
{
	mesh mesh;
	float theta;
	glm::vec3 vertex;

	mesh.primitive_type = GL_LINE_LOOP;

	for (unsigned int u = 0; u < num_segments; u++)
	{
		theta = 2.0f * 3.1415926f * float(u) / float(num_segments);

		vertex.x = radius * cosf(theta);
		vertex.y = radius * sinf(theta);
		vertex.z = 0.0;

		mesh.vertices.push_back(vertex);
		mesh.indices.push_back(u);
	}

	return mesh;
}


