#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace RenderEngine
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		bool isCompiled() const { return m_isCompiled; }
		void use() const;
		void setInt(const std::string& name, const GLint value);
		void setFloat(const std::string& name, const GLfloat value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	private:
		bool createShader(const std::string& source, GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}
