#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() : 
	m_particles(nullptr), m_firstDead(0), m_maxParticles(0),
	m_position(0,0,0), m_vao(0), m_vbo(0), m_ibo(0), m_vertexData(nullptr)
{
}

ParticleEmitter::ParticleEmitter(unsigned int _maxParticles, unsigned int _emitRate, 
	float _lifeTimeMin, float _lifeTimeMax, float _velocityMin, float _velocityMax, 
	float _startSize, float _endSize, const glm::vec4& _startColor, const glm::vec4& _endColor) :
	m_maxParticles(_maxParticles), m_lifespanMin(_lifeTimeMin), m_lifespanMax(_lifeTimeMax),
	m_velocityMin(_velocityMin), m_velocityMax(_velocityMax), m_startSize(_startSize),
	m_endSize(_endSize), m_startColor(_startColor), m_endColor(_endColor)
{

}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::Initialise(unsigned int _maxParticles, unsigned int _emitRate, 
	float _lifeTimeMin, float _lifeTimeMax, float _velocityMin, float _velocityMax, 
	float _startSize, float _endSize, const glm::vec4& _startColor, const glm::vec4& _endColor)
{
	// Set up emit timer
	m_emitRate = 1.0 / _emitRate;
	m_emitTimer = 0;

	// Store the variables that we pass in
	m_lifespanMin	= _lifeTimeMin;
	m_lifespanMax	= _lifeTimeMax;

	m_velocityMin	= _velocityMin;
	m_velocityMax	= _velocityMax;

	m_startSize		= _startSize;
	m_endSize		= _endSize;

	m_startColor	= _startColor;
	m_endColor		= _endColor;

	m_maxParticles	= _maxParticles;

	// Create a particle array
	m_particles = new Particle[m_maxParticles];
	m_firstDead = 0;

	// We need to create an array of vertices for the particles 
	// There needs the be four (4) verticies per particles to make a quad
	// This will be generated as an update emitter
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// Indicies that will be used for all required verticies
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; i++)
	{
		indexData[i * 6]	 = i * 4;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// Time to create the OpenGL buffers!
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(ParticleVertex), 
		m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int),
		indexData, GL_STATIC_DRAW);

	// This is the position of the particle for their shader
	glEnableVertexAttribArray(0);

	// This is the color of the particle for their shader
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
		sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 
		sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void ParticleEmitter::Emit()
{
	// Check if there are any available particles for the system to emit
	if (m_firstDead >= m_maxParticles)
		return;

	// Return a dead particle our available list
	Particle& particle = m_particles[m_firstDead++];
	
	// Reset the position of the returned particle
	particle.position = m_position;

	// Reset the lifespan of the returned particle
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) * 
		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// Reset the velocity of the returned particle
	float velocity = (rand() / (float)RAND_MAX) * 
		(m_velocityMax - m_velocityMin) + m_velocityMin;

	// Reset the color the returned particle
	particle.color = m_startColor;

	// Reset the size of the returned particle;
	particle.size = m_startSize;

	particle.velocity.x = (rand() / (float)RAND_MAX * 2 - 1);
	particle.velocity.y = (rand() / (float)RAND_MAX * 2 - 1);
	particle.velocity.z = (rand() / (float)RAND_MAX * 2 - 1);

	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

void ParticleEmitter::Update(float _dt, const glm::mat4& _cameraTransform)
{
	// This will move and update all of the alive particles
	// Then it will remove the dead particles
	// It will then emit the particles based on the emitter's provided rate
	// Finally we will then update the vertex array and setup our billboarding

	m_emitTimer += _dt;

	// Spawn the particles
	while (m_emitTimer > m_emitRate)
	{
		Emit();
		m_emitTimer -= m_emitRate;
	}

	unsigned int quad = 0;

	// This will update all particles and make sure they 
	// work as billboarding quads

	for (unsigned int i = 0; i < m_firstDead; i++)
	{
		Particle* particle = &m_particles[i];

		particle->lifetime += _dt;
		if (particle->lifetime >= particle->lifespan)
		{
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			particle->position += particle->velocity * _dt; // This will need to be changed to use gravity

			// Set the scale of the particle 
			particle->size = glm::mix(m_startSize, m_endSize,
				particle->lifetime / particle->lifespan);

			particle->color = glm::mix(m_startColor, m_endColor,
				particle->lifetime / particle->lifespan);

			// Finally to set up the quad using the correct position, color and scale
			float halfSize = particle->size * 0.5f;
			m_vertexData[quad * 4].position		= glm::vec4( halfSize,  halfSize, 0, 1);
			m_vertexData[quad * 4].color		= particle->color;

			m_vertexData[quad * 4 + 1].position	= glm::vec4(-halfSize,  halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].color	= particle->color;

			m_vertexData[quad * 4 + 2].position	= glm::vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].color	= particle->color;

			m_vertexData[quad * 4 + 3].position	= glm::vec4( halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].color	= particle->color;

			// Set up our billboard's transform
			glm::vec3 zAxis = glm::normalize(glm::vec3(_cameraTransform[3]) - particle->position);
			glm::vec3 xAxis = glm::cross(glm::vec3(_cameraTransform[1]), zAxis);
			glm::vec3 yAxis = glm::cross(zAxis, xAxis);

			glm::mat4 billboard(glm::vec4(xAxis, 0),
								glm::vec4(yAxis, 0),
								glm::vec4(zAxis, 0),
								glm::vec4(0, 0, 0, 1));

			m_vertexData[quad * 4].position		= billboard *
			m_vertexData[quad * 4].position		+ glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 1].position = billboard *
			m_vertexData[quad * 4 + 1].position + glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 2].position = billboard *
			m_vertexData[quad * 4 + 2].position + glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 3].position = billboard *
			m_vertexData[quad * 4 + 3].position + glm::vec4(particle->position, 0);

			++quad;
		}
	}
}

void ParticleEmitter::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 
		m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}