#pragma once

#include <GL\glew.h>
#include "texture.h"

class PostProcessing
{
public:
	PostProcessing();
	~PostProcessing();
	 
	GLuint getPostProcessingProgramID() { return m_PostProcessingProgramID; }

	void setPostProcessingProgramID(GLuint ID) { m_PostProcessingProgramID = ID; }
	void setTexture0Location(GLint location) { m_Texture0Location = location; }

	void bindFrameBuffer();
	void render();
	void destroy();

private:
	GLuint m_ColourBufferID;
	GLuint m_DepthRenderBufferID;
	GLuint m_FrameBufferID;
	GLuint m_ScreenQuadVBOID;
	GLuint m_ScreenVAO;
	GLuint m_PostProcessingProgramID;
	GLint m_Texture0Location;
	GLfloat m_ScreenVertss[];
};

