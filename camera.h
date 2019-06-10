#pragma

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


//1. Set our Camera (Front, Right, Postion and Up) values
//2. Create the Look At Matrix (contain all previous data) 
//3. Send it back to the Main Lopop

//4. wait for Mouse/Scroll/Keyboard changes 
//5. Apply the opeartion (Keyboard = positions, Scroll = zoom, Mouse = yaw, pitch)
//6. Update the values (Front, Right, Up)
//7. Send the new LookAt




// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,  //0 
	BACKWARD, //1
	LEFT,     //2
	RIGHT     //3
};

// Default camera values
const float YAW = -90.0f;        //Mouse
const float PITCH = 0.0f;        //Mouse
const float SPEED = 2.5f;        //Fixed
const float SENSITIVITY = 0.1f;  //Fixed
const float ZOOM = 45.0f;        //Scroll

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position; 
	glm::vec3 Front;    //z axis
	glm::vec3 Up;       // y axis
	glm::vec3 Right;    //x acis 
	glm::vec3 WorldUp; 
	// Euler Angles
	float Yaw;    //rotated around y axis
	float Pitch;  //rotated around x axis
	// Camera options
	float MovementSpeed;     //Fixed
	float MouseSensitivity;  //Fixed
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up); //Camera Position, target Position (current position + direction vector), Up vector in World Space
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		//No need to call updateCameraVector
		float velocity = MovementSpeed * deltaTime; 
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		  //2.Add the offset values to the camera's yaw and pitch values.
         //3.Add some constraints to the maximum / minimum pitch values
         //4.Calculate the direction vector
		
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f) //at 90 degrees the view tends to reverse
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		//When scrolling, the yoffset value represents the amount we scrolled vertically
		//No need to call updateCameraVector
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset; 
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif