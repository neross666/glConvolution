#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <random>

GLuint program;
GLuint textureID;
int width = 1024, height = 1024;

// 7x7均值滤波器
float kernel[49];

GLuint loadShader(GLenum type, const char* filename) {
	std::ifstream file(filename);
	std::string source((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	const char* src = source.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	// 检查编译错误
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
	}
	return shader;
}

void loadTexture() {
	// 生成椒盐噪声测试数据
	std::vector<GLubyte> image(width * height);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	const float noiseProbability = 0.05f; // 5%的噪声比例

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (dis(gen) < noiseProbability) {
				image[y * width + x] = (dis(gen) < 0.5f) ? 0 : 255; // 随机黑点或白点
			}
			else {
				image[y * width + x] = 128; // 灰色背景
			}
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
		GL_RED, GL_UNSIGNED_BYTE, image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void init() {
	glewInit();

	// 初始化7x7均值滤波器
	const float kernelValue = 1.0f / 49.0f;
	for (int i = 0; i < 49; ++i) {
		kernel[i] = kernelValue;
	}

	// 加载着色器
	GLuint vertShader = loadShader(GL_VERTEX_SHADER, "shader.vert");
	GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, "shader.frag");

	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	// 检查链接错误
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "Program linking error:\n" << infoLog << std::endl;
	}

	loadTexture();
	glUseProgram(program);

	// 设置卷积核uniform
	GLint kernelLoc = glGetUniformLocation(program, "kernel");
	glUniform1fv(kernelLoc, 49, kernel);

	// 设置纹理单元
	GLint imageLoc = glGetUniformLocation(program, "image");
	glUniform1i(imageLoc, 0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("OpenGL Convolution");

	glutDisplayFunc(display);

	init();
	glutMainLoop();
	return 0;
}