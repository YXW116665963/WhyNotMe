#pragma once
#include "glad/glad.h"
#include "wx/wx.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "util.h"

namespace why
{
    // Default camera values
    const float YAW         = -90.0f;
    const float PITCH       = 0.0f;
    const float SPEED       = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM        = 45.0f;

    class Camera
    {
    public:
        Camera(const std::function<void()>& funcRender,
            glm::vec3 vec3CamPos = glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3 vec3CamFront = glm::vec3(0.0f,0.0f,-1.0f),
            glm::vec3 vec3CamUp = glm::vec3(0.0f, 1.0f, 0.0f),
            float32_t fYaw = YAW, float32_t fPitch = PITCH);
        ~Camera();

        void HandleEvent(wxMouseEvent& event);
        void HandleEvent(wxKeyEvent& event);

        glm::mat4 GetViewMat();
        float32_t GetZoom() { return m_fZoom; };

    private:
        // 更新相机向量
        void UpdateCamVecs();

    private:
        // camera Attributes
        // 相机位置
        glm::vec3 m_vec3CamPos;
        // 相机朝向
        glm::vec3 m_vec3CamFront;
        // 上向量
        glm::vec3 m_vec3CamUp;
        // 右向量
        glm::vec3 m_vec3CamRight;
        // ？世界上向量
        glm::vec3 m_vec3WorldUp;

        // Default camera values
        //偏航角
        float32_t m_fYaw;
        //俯仰角
        float32_t m_fPitch;

        // camera options
        // fov
        float32_t m_fZoom;
        // 鼠标滚轮灵敏度
        float32_t m_fMouseSensitivity;
        // 键盘移动速度
        float32_t m_fMovementSpeed;

    private:
        //上一帧鼠标位置
        wxPoint m_pointLastFrame;
        // 鼠标左键是否摁下
        bool    m_bLeftDown;
        // opengl绘制方法
        std::function<void()> m_funcRender;
    };


}