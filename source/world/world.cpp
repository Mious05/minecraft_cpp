#include "world.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "../core/camera.hpp"
#include "block.hpp"
#include <iostream>

World::World():
	m_shader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl"),
	m_tilset("resources/images/tileset.png") {
	
	// set shader uniforms
	glm::mat4 model_matrix(1.0f);
	glm::mat4 projection_matrix = glm::perspective(3.141592853f / 2.f, 16.f / 9.f, .01f, 100.f);

	m_shader.use();
	m_shader.setMat4("projection", glm::value_ptr(projection_matrix));

	// construct chunks
	m_chunks = new Chunk[CHUNK_X * CHUNK_Z];
	for(int i = 0; i < WORLD_X; ++i)
		for (int j = 0; j < WORLD_Z; ++j) {
			Chunk &current_chunk = m_chunks[j * CHUNK_X + i];
			current_chunk.setPosition(i * 16, 0, j * 16);
			current_chunk.generateTerrain();

			Chunk *px = nullptr, *mx = nullptr, *py = nullptr, *my = nullptr;

			if(i > 0 )
				mx = &m_chunks[ i - 1 + CHUNK_X * j];
			if(i != WORLD_X - 1)
				px = &m_chunks[ i + 1 + CHUNK_X * j];
			if(j > 0 )
				my = &m_chunks[ i + CHUNK_X * (j - 1)];
			if(j != WORLD_Z - 1)
				py = &m_chunks[ i + CHUNK_X * (j + 1)];

			current_chunk.setNeighbours(px, mx, py, my);
			
		}
}

World::~World(){
	delete[] m_chunks;
}

void World::setBlock(int x, int y, int z, unsigned char type) {
	if (x >= CHUNK_X * WORLD_X || x < 0 || y >= CHUNK_Y || y < 0 || z >= CHUNK_Z * WORLD_Z || z < 0)
		return;

	int chunk_x = x / CHUNK_X,
		chunk_z = z / CHUNK_Z;

	int coord_x = x % CHUNK_X,
		coord_z = z % CHUNK_Z;
	
	m_chunks[chunk_x + chunk_z * CHUNK_X].setBlock(coord_x, y, coord_z, type);
}

unsigned char World::getBlock(int x, int y, int z) {
	if (x >= CHUNK_X * WORLD_X || x < 0 || y >= CHUNK_Y || y < 0 || z >= CHUNK_Z * WORLD_Z || z < 0)
		return Blocks::AIR;

	int chunk_x = x / CHUNK_X,
		chunk_z = z / CHUNK_Z;

	int coord_x = x % CHUNK_X,
		coord_z = z % CHUNK_Z;

	return m_chunks[chunk_x + chunk_z * CHUNK_X].getBlock(coord_x, y, coord_z);
}

void World::draw( Camera &camera ) {
	m_shader.use();

	glm::mat4 view_matrix = camera.getViewMatrix();
	m_shader.setMat4("view", glm::value_ptr(view_matrix));

	for(int i = 0; i < WORLD_X; ++i)
		for(int j = 0; j < WORLD_Z; ++j)
			m_chunks[j * CHUNK_X + i].draw(camera, m_tilset, m_shader);
}