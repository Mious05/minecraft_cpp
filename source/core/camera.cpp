#include "camera.hpp"
#include "input.hpp"

#include <glm/ext.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& up):
	m_position(position), m_up(up) {
}

void Camera::setPosition(const glm::vec3& position) {
	m_position = position;
}

void Camera::setUp(const glm::vec3& up) {
	m_up = up;
}

void Camera::update(Input& input, double delta_time) {

	if(input.isKeyPressed(SDL_SCANCODE_W))
		m_position += m_direction * (float)delta_time;
	if(input.isKeyPressed(SDL_SCANCODE_S))
		m_position -= m_direction * (float)delta_time;
	if(input.isKeyPressed(SDL_SCANCODE_D))
		m_position += glm::normalize(glm::cross(m_direction, m_up)) * (float)delta_time;
	if(input.isKeyPressed(SDL_SCANCODE_A))
		m_position -= glm::normalize(glm::cross(m_direction, m_up)) * (float)delta_time;


	static glm::ivec2 last_mouse_position(-1, -1); 

	if (last_mouse_position.x == -1)
		// if it is the first time, we define last_mouse_position
		last_mouse_position = input.getMousePosition();
	
	// calculate the mouse offset
	glm::ivec2 delta_mouse = input.getMousePosition() - last_mouse_position;
	last_mouse_position = input.getMousePosition();


	m_pitch -= 0.005 * delta_mouse.y;
	m_yaw += 0.005 * delta_mouse.x;

	const float piovertwo = glm::pi<float>() / 2.0f;

	if (m_pitch >= piovertwo - 0.01f) m_pitch = piovertwo - 0.01f;
	if (m_pitch <= -piovertwo + 0.01f) m_pitch = -piovertwo + 0.01f;


	glm::vec3 direction;
	direction.x = cos(m_yaw) * cos(m_pitch);
	direction.y = sin(m_pitch);
	direction.z = sin(m_yaw) * cos(m_pitch);
	m_direction = glm::normalize(direction);
}


glm::mat4 Camera::getViewMatrix() {
	// translate by -m_position, rotates -yaw about y, amd -m_pitch about x
	return glm::lookAt(m_position, m_position + m_direction, m_up);
}