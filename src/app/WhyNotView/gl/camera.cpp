#include "glad/glad.h"
#include "camera.h"


namespace why
{
	Camera::Camera(const std::function<void()>& funcRender,
		glm::vec3 vec3CamPos, glm::vec3 vec3CamFront, glm::vec3 vec3CamUp, float32_t fYaw, float32_t fPitch)
		: m_funcRender(funcRender)
		, m_vec3CamPos(vec3CamPos), m_vec3CamFront(vec3CamFront), m_vec3CamUp(vec3CamUp)
		, m_fYaw(fYaw), m_fPitch(fPitch)
		, m_fZoom(ZOOM), m_fMovementSpeed(SPEED), m_fMouseSensitivity(SENSITIVITY)
		, m_vec3CamRight(glm::normalize(glm::cross(vec3CamFront, vec3CamUp)))
		, m_vec3WorldUp(vec3CamUp)
		, m_bLeftDown(false)
	{


	}

	Camera::~Camera()
	{

	}

	void Camera::HandleEvent(wxMouseEvent& event)
	{
		wxEventType type = event.GetEventType();

		if (wxEVT_LEFT_DOWN == type)
		{
			m_bLeftDown = true;
			m_pointLastFrame = event.GetPosition();
		}
		else if (wxEVT_LEFT_UP == type)
		{
			m_bLeftDown = false;
		}
		else if (wxEVT_MOTION == type)
		{
			if (m_bLeftDown && event.Dragging())
			{
				wxPoint currentPos = event.GetPosition();

				float deltaX = currentPos.x - m_pointLastFrame.x;
				float deltaY = m_pointLastFrame.y - currentPos.y;

				m_pointLastFrame = currentPos;

				m_fYaw += deltaX * m_fMouseSensitivity;
				m_fPitch += deltaY * m_fMouseSensitivity;

				if (m_fPitch > 89.0f)
					m_fPitch = 89.0f;
				if (m_fPitch < -89.0f)
					m_fPitch = -89.0f;

				UpdateCamVecs();
				m_funcRender();
			}

		}
		else if (wxEVT_MOUSEWHEEL == type)
		{
			// 滚轮旋转量：正数向上，负数向下
			int rot = event.GetWheelRotation();
			int delta = event.GetWheelDelta();
			float step = 0.5f; // 每次滚动缩放步长

			if (rot > 0)
			{
				// 滚轮向上：放大
				m_fZoom += step;
			}
			else
			{
				// 滚轮向下：缩小
				m_fZoom -= step;
			}
			m_funcRender();
		}
	}

	void Camera::UpdateCamVecs()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		front.y = sin(glm::radians(m_fPitch));
		front.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		m_vec3CamFront = glm::normalize(front);

		// 根据相机朝向向量更新重新确立相机坐标系
		m_vec3CamRight = glm::normalize(glm::cross(m_vec3CamFront, m_vec3WorldUp));
		m_vec3CamUp = glm::normalize(glm::cross(m_vec3CamRight, m_vec3CamFront));
	}

	void Camera::HandleEvent(wxKeyEvent& event)
	{
		int key = event.GetKeyCode();
		float cameraSpeed = 0.05f; // adjust accordingly
		switch (key)
		{
		case 'W':
		case 'w':
			// W 按下逻辑
			m_vec3CamPos += m_fMovementSpeed * m_vec3CamFront;
			break;
		case 'S':
		case 's':
			// S 按下逻辑
			m_vec3CamPos -= m_fMovementSpeed * m_vec3CamFront;
			break;
		case 'A':
		case 'a':
			// A 按下逻辑
			m_vec3CamPos -= glm::normalize(glm::cross(m_vec3CamFront, m_vec3CamUp)) * cameraSpeed;
			break;
		case 'D':
		case 'd':
			// D 按下逻辑
			m_vec3CamPos += glm::normalize(glm::cross(m_vec3CamFront, m_vec3CamUp)) * cameraSpeed;
			break;
		default:
			return; // 其他按键不渲染
		}

		m_funcRender();
	}

	glm::mat4 Camera::GetViewMat()
	{
		return glm::lookAt(m_vec3CamPos, m_vec3CamPos + m_vec3CamFront, m_vec3CamUp);
	}
}