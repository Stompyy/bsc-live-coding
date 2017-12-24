#include "PostProcessing.h"

PostProcessing::PostProcessing()
{
	m_ColourBufferID = CreateTexture(1000, 800);

	glGenRenderbuffers(1, &m_DepthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1000, 800);

	glGenFramebuffers(1, &m_FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColourBufferID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unable to create frame buffer for post processing", "Frame Buffer Error", NULL);
	}

	//create screen aligned quad
	GLfloat screenVerts[] =
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};

	glGenBuffers(1, &m_ScreenQuadVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ScreenQuadVBOID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), screenVerts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_ScreenVAO);
	glBindVertexArray(m_ScreenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ScreenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}


PostProcessing::~PostProcessing()
{
	// Call destroy here too just in case
}

void PostProcessing::destroy()
{
	glDeleteProgram(m_PostProcessingProgramID);
	glDeleteVertexArrays(1, &m_ScreenVAO);
	glDeleteBuffers(1, &m_ScreenQuadVBOID);

	glDeleteFramebuffers(1, &m_FrameBufferID);
	glDeleteRenderbuffers(1, &m_DepthRenderBufferID);
	glDeleteTextures(1, &m_ColourBufferID);
}

void PostProcessing::bindFrameBuffer()
{
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessing::render()
{
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind post processing shaders
	glUseProgram(m_PostProcessingProgramID);

	// Activate texture unit 0 for the colour buffer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColourBufferID);
	glUniform1i(m_Texture0Location, 0);

	glBindVertexArray(m_ScreenVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
