#pragma once

#include "Mesh.h"


struct PointContainer
{
	std::vector<glm::vec3> data;
	GLuint vertexBuffer;

	static GLuint VertexArrayID;

	static void setupVAO()
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
	}

	static void clearVAO()
	{
		glDeleteVertexArrays(1, &VertexArrayID);
	}

	PointContainer(std::vector<glm::vec3>& data)
		: data(data)
	{
		if (VertexArrayID == 777) {
			PointContainer::setupVAO();
		}
		glBindVertexArray(VertexArrayID);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, (data.size()) * 3 * sizeof(float), data.data(), GL_STATIC_DRAW);
	}

	~PointContainer()
	{
		glBindVertexArray(VertexArrayID);
		glDeleteBuffers(1, &vertexBuffer);
	}
};

GLuint PointContainer::VertexArrayID = 777;


template <typename InputReaction>
struct SimpleRenderer
{
	glm::vec3             scale;
	glm::quat             rotation;
	glm::vec3             position;

	GLuint                vertexBuffer;
	size_t                verticesNum;
	glm::vec3             objectColor;

	InputReaction         reactOnInput;


	SimpleRenderer(PointContainer& points)
	{
		vertexBuffer = points.vertexBuffer;
		verticesNum = points.data.size();
		glm::vec3 eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat(eulerAngles);
		position = glm::vec3(0.0f);
		objectColor = glm::vec3(0.73f, 0.37f, 0.43f);
		scale = glm::vec3(1.0f);
	}

	inline void setupRender()
	{
		glBindVertexArray(PointContainer::VertexArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // index in layout in shader
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(0);
	}

	inline void Render(
		GLuint shader, glm::mat4 view, glm::mat4 projection,
		GLuint modelID, GLuint viewID, GLuint projectionId, GLuint colorID)
	{
		glUseProgram(shader);
		auto model = ComputeModelMatrix();
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionId, 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(colorID, 1, &objectColor[0]);

		glBindVertexArray(PointContainer::VertexArrayID);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_POLYGON, 0, (GLsizei)verticesNum);
	}

	inline glm::mat4 ComputeModelMatrix() {
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotMat = glm::mat4(rotation);
		glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
		return transMat * rotMat * scaleMat;
	}

	//  random accessors
	inline void ReactOnInput(SDL_Event& e) {
		reactOnInput(e, scale);
	}

	inline void Paint(glm::vec3 color) { objectColor = color; }

	void Translate(glm::vec3& vec) { position += vec; }

	void Scale(glm::vec3& alpha) { scale = alpha; }

	void Scale(float alpha) { scale = glm::vec3(alpha); }

	void Rotate(glm::quat& rot) { rotation = rot; }

	void Rotate(glm::vec3& rot) { rotation = glm::quat(rot); }

	const glm::vec3 Pos() const { return position; }

	const glm::vec3 Scale() const { return scale; }
};
