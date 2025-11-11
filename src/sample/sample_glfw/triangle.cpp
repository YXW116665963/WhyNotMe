#include "triangle.h"

// »æÖÆÈý½ÇÐÎ
void Demo_DrawTriangles(unsigned int shaderProgram, unsigned int VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

