#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace why
{
	// 工作平面参数
	const float PLANE_SIZE = 50.0f;    // 平面半宽，总尺寸 100x100
	const float GRID_STEP = 1.0f;      // 网格一格1单位
	const glm::vec3 PLANE_COLOR(0.25f, 0.25f, 0.25f); // 底色深灰
	const glm::vec3 GRID_COLOR(0.0f, 0.0f, 0.0f);      // 网格黑色
	const float GRID_LINE_WIDTH = 1.0f;


    class WorkPlane
    {
    public:
        GLuint vaoFill, vboFill;   // 填充四边形缓冲
        GLuint vaoGrid, vboGrid;   // 网格线缓冲
        int gridLineCount;         // 网格线段总数

        // 初始化缓冲
        void init()
        {
            // 1. 初始化底色填充四边形
            std::vector<glm::vec3> fillVerts = {
                {-PLANE_SIZE, 0, -PLANE_SIZE},
                { PLANE_SIZE, 0, -PLANE_SIZE},
                { PLANE_SIZE, 0,  PLANE_SIZE},
                {-PLANE_SIZE, 0,  PLANE_SIZE},
            };
            glGenVertexArrays(1, &vaoFill);
            glGenBuffers(1, &vboFill);
            glBindVertexArray(vaoFill);
            glBindBuffer(GL_ARRAY_BUFFER, vboFill);
            glBufferData(GL_ARRAY_BUFFER, fillVerts.size() * sizeof(glm::vec3), fillVerts.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);

            // 2. 预生成所有网格线段顶点
            std::vector<glm::vec3> gridVerts;
            // X向横线
            for (float z = -PLANE_SIZE; z <= PLANE_SIZE; z += GRID_STEP)
            {
                gridVerts.emplace_back(-PLANE_SIZE, 0, z);
                gridVerts.emplace_back(PLANE_SIZE, 0, z);
            }
            // Z向竖线
            for (float x = -PLANE_SIZE; x <= PLANE_SIZE; x += GRID_STEP)
            {
                gridVerts.emplace_back(x, 0, -PLANE_SIZE);
                gridVerts.emplace_back(x, 0, PLANE_SIZE);
            }
            gridLineCount = gridVerts.size() / 2;

            glGenVertexArrays(1, &vaoGrid);
            glGenBuffers(1, &vboGrid);
            glBindVertexArray(vaoGrid);
            glBindBuffer(GL_ARRAY_BUFFER, vboGrid);
            glBufferData(GL_ARRAY_BUFFER, gridVerts.size() * sizeof(glm::vec3), gridVerts.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        // 渲染平面
        void render(const glm::mat4& mvp, GLuint shaderProgram)
        {
            glUseProgram(shaderProgram);
            GLint locMvp = glGetUniformLocation(shaderProgram, "mvp");
            GLint locCol = glGetUniformLocation(shaderProgram, "uColor");
            GLint locAlpha = glGetUniformLocation(shaderProgram, "uAlpha");
            glUniformMatrix4fv(locMvp, 1, GL_FALSE, &mvp[0][0]);

            // 绘制半透明底色
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glUniform3fv(locCol, 1, &PLANE_COLOR[0]);
            glUniform1f(locAlpha, 0.6f);
            glBindVertexArray(vaoFill);
            glDrawArrays(GL_QUADS, 0, 4);

            // 绘制网格线
            glDisable(GL_BLEND);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(GRID_LINE_WIDTH);
            glUniform3fv(locCol, 1, &GRID_COLOR[0]);
            glUniform1f(locAlpha, 1.0f);
            glBindVertexArray(vaoGrid);
            glDrawArrays(GL_LINES, 0, gridLineCount * 2);

            glBindVertexArray(0);
            glUseProgram(0);
        }

        // 释放资源
        void destroy()
        {
            glDeleteVertexArrays(1, &vaoFill);
            glDeleteBuffers(1, &vboFill);
            glDeleteVertexArrays(1, &vaoGrid);
            glDeleteBuffers(1, &vboGrid);
        }
    };

}