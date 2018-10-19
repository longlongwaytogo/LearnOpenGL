#ifndef __OGL_CAMERA_H__
#define __OGL_CAMERA_H__

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = - 90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat ROLL = 0.0f;
const GLfloat SPEED = 1.0f;
const GLfloat SENSITIVITY = 0.05f;
const GLfloat ZOOM 	= 45.0f;

 class Camera
 {
private:
	glm::vec3 m_vPosition;
	glm::vec3 m_vFront;
	glm::vec3 m_vUp;
	glm::vec3 m_vRight;
	glm::vec3 m_vWorldUp;
	GLfloat   m_fYaw;
	GLfloat   m_fPitch;
	GLfloat   m_fRoll;
	
	GLfloat   m_fMovementSpeed;
	GLfloat   m_fMouseSensitivity;
	GLfloat   m_fZoom;
	bool 	  m_bCtr; // use mouse control
public:	
	Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH, GLfloat roll = ROLL):m_vFront(glm::vec3(0.0f,0.0f,-1.0f)),
		m_fMovementSpeed(SPEED),m_fMouseSensitivity(SENSITIVITY),m_fZoom(ZOOM)
		{
			m_vPosition = position;
			m_vWorldUp = up;
			m_fYaw = yaw;
			m_fPitch = pitch;
			m_fRoll = roll;
			
			m_bCtr = true;
			
			updateCameraVector();
		}
	 
	 glm::mat4 GetViewMatrix()
	 {
		 return glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);
	 }
	 
	 void Processkeyboard(Camera_Movement direction, GLfloat deltaTime)
	 {
		GLfloat velocity = this->m_fMovementSpeed * deltaTime * 3.0f;
		if(direction == FORWARD)
			m_vPosition += m_vFront * velocity;
		if(direction == BACKWARD)
			m_vPosition -= m_vFront * velocity;
		if(direction == LEFT)
			m_vPosition -= m_vRight * velocity;
		if(direction == RIGHT)
			m_vPosition += m_vRight * velocity;	
	 }
	 
	 void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	 {
		 xoffset *= m_fMouseSensitivity;
		 yoffset *= m_fMouseSensitivity;
		 
		 m_fYaw  += xoffset;
		 m_fPitch += yoffset;
		 
		 if(constrainPitch)
		 {
			if(m_fPitch > 89.0f)
				m_fPitch = 89.0f;
			if(m_fYaw > 89.0f)
				m_fYaw = 89.0f;
		 }
		 
		 if(m_bCtr)
			updateCameraVector();
	 }
	 
	 void ProcessMouseScrool(GLfloat yoffset)
	 {
		m_fZoom -= yoffset;
		if(m_fZoom <= 1.0f)
			m_fZoom = 1.0f;
		if(m_fZoom >= 179.0f)
			m_fZoom = 179.0f;
	 }
	 
private:
	 void updateCameraVector()
	 {
		glm::vec3 front;
		front.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		front.y = sin(glm::radians(m_fPitch));
		front.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		
		m_vFront = glm::normalize(front);
		m_vRight = glm::normalize(glm::cross(m_vFront,m_vWorldUp));
		m_vUp = glm::normalize(glm::cross(m_vRight,m_vFront));
	 }
 };
 
#endif // __OGL_CAMERA_H__



