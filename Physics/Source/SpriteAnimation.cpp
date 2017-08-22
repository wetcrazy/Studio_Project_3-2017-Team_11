#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"


SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
	: Mesh(meshName)
	, m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
	{
		delete m_anim; // Delete will go to the location of the pointer and delete what is storeed there ~
		               //what happens to the pointer ? 
		m_anim = NULL;
	}
		
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim == NULL)
		return;

	if (m_anim->animActive == false)
		return;

	//this is the workings
	m_currentTime += static_cast<float>(dt);
	int numFrame = Math::Max(1, m_anim->endFrame - 
		m_anim->startFrame + 1);
	float frameTime = m_anim->animTime / numFrame;

	//Magic
	m_currentFrame = Math::Min(m_anim->endFrame, 
		m_anim->startFrame + static_cast<int>
		(m_currentTime / frameTime));


	//Reset it 
	if (m_currentTime >= m_anim->animTime)
	{
		if (m_anim->repeatCount == 0)
		{
			m_anim->animActive = false;
			m_currentTime = 0.0f;
			m_currentFrame = m_anim->startFrame;
			m_anim->ended = true;
		}

		if (m_anim->repeatCount == 1)
		{
			m_currentTime = 0.0f;
			m_currentFrame = m_anim->startFrame;
		}
	}
	
}


void SpriteAnimation::Render()
{
	// Insert codes. Hint: same as in Mesh.cpp.

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof
	(Color)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof
	(Color) + sizeof(Vector3)));
	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof
		(GLint)));
	else if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof
		(GLint)));
	else
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof
		(GLint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	
}

