#include "Shader.h"

Shader::Shader(const char* _vertFileName, const char* _fragFileName) {

	vertFileName = _vertFileName;
	fragFileName = _fragFileName;
}

char* Shader::ReadShaderSource(const char* shaderFile) {
	FILE* fp = fopen(shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

void Shader::AddShader(GLuint shaderProgram, const char* shaderText, GLenum shaderType) {
	
	/* Create a shader object */
	GLuint shaderObject = glCreateShader(shaderType);

	if (shaderObject == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(0);
	}
	const char* shaderSource = ReadShaderSource(shaderText);

	/* Bind the source code to the shader, this happens before compilation */
	glShaderSource(shaderObject, 1, (const GLchar**)&shaderSource, NULL);
	/* Compile the shader and check for errors */
	glCompileShader(shaderObject);
	GLint success;
	/* Check for shader related errors using glGetShaderiv */
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObject, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}
	/* Attach the compiled shader object to the program object */
	glAttachShader(shaderProgram, shaderObject);
}

GLuint Shader::CompileShaders() {
	
	/* Start the process of setting up our shaders by creating a program ID */
	GLuint shaderProgramID = glCreateProgram();

	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	/* Create two shader objects, one for the vertex, and one for the fragment shader */
	AddShader(shaderProgramID, vertFileName, GL_VERTEX_SHADER);
	AddShader(shaderProgramID, fragFileName, GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = { 0 };
	/* After compiling all shader objects and attaching them to the program, we can finally link it */
	glLinkProgram(shaderProgramID);
	/* Check for program related errors using glGetProgramiv */
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	/* Program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state */
	glValidateProgram(shaderProgramID);
	/* Check for program related errors using glGetProgramiv */
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}
	/* Finally, use the linked shader program */
	/* Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use */
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}