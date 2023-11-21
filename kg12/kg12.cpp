#include <iostream>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// ID шейдерной программы
GLuint Program;
// ID атрибута
GLint Attrib_vertex;
// ID Vertex Buffer Object
GLuint VBO;

struct Vertex {
	GLfloat x;
	GLfloat y;
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

////  исходный код вершинного шейдера

const char* VertexShaderSource = R"(
	#version 330 core
	in vec2 coord;
	in vec3 color; //  вход для цвета
	out vec3 vertexColor; //  выход для цвета
	void main() {
		gl_Position = vec4(coord, 0.0, 1.0);
		vertexColor = color; // Передаем цвет во фрагментный шейдер
	}
)";

//  исходный код фрагментного шейдера
const char* FragShaderSource = R"(
	#version 330 core
	in vec3 vertexColor; // цвет из вершинного шейдера
	out vec4 fragColor;
	void main() {
		fragColor = vec4(vertexColor, 1.0); // Используем цвет для закрашивания
	}
)";

//void SetIcon(sf::Window& wnd);
void Init();
void Draw();

// Освобождение буфера
void ReleaseVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
}

// Освобождение шейдеров
void ReleaseShader() {
	// Передавая ноль, мы отключаем шейдерную программу
	glUseProgram(0);
	// Удаляем шейдерную программу
	glDeleteProgram(Program);
}

void Release() {
	// Шейдеры
	ReleaseShader();
	// Вершинный буфер
	ReleaseVBO();
}

int main() {
	// Создание окна с использованием SFML
	sf::Window window(sf::VideoMode(700, 700), "My OpenGL window",
		sf::Style::Default, sf::ContextSettings(24));
	// Включение вертикальной синхронизации для ограничения FPS
	window.setVerticalSyncEnabled(true);
	// Установка окна как активного контекста для OpenGL
	window.setActive(true);
	// Инициализация GLEW для загрузки OpenGL-расширений
	glewInit();
	// Инициализация OpenGL и создание шейдеров
	Init();
	// Главный цикл приложения
	while (window.isOpen()) {
		// Обработка событий, таких как закрытие окна
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::Resized) {
				// Изменение области отрисовки при изменении размера окна
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}
		// Очистка буферов кадра и глубины
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Отрисовка сцены
		Draw();
		// Отображение результата в окне
		window.display();
	}
	// Освобождение ресурсов OpenGL перед завершением
	Release();
	return 0;
}


void checkOpenGLerror()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		// Process/log the error.
		std::cout << err << std::endl;
	}
}

void ShaderLog(unsigned int shader)
{
	int infologLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 1)
	{
		int charsWritten = 0;
		std::vector<char> infoLog(infologLen);
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
		std::cout << "InfoLog: " << infoLog.data() << std::endl;
	}
}

void InitShader() {
	// Создаем вершинный шейдер
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// Передаем исходный код
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	// Компилируем шейдер
	glCompileShader(vShader);
	std::cout << "vertex shader \n";
	// Функция печати лога шейдера
	ShaderLog(vShader);
	// Создаем фрагментный шейдер
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Передаем исходный код
	glShaderSource(fShader, 1, &FragShaderSource, NULL);
	// Компилируем шейдер
	glCompileShader(fShader);
	std::cout << "fragment shader \n";
	// Функция печати лога шейдера
	ShaderLog(fShader);
	// Создаем программу и прикрепляем шейдеры к ней
	Program = glCreateProgram();
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	// Линкуем шейдерную программу
	glLinkProgram(Program);
	// Проверяем статус сборки
	int link_ok;
	// 0 - false
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		std::cout << "error attach shaders \n";
		return;
	}
	// Вытягиваем ID атрибута из собранной программы
	const char* attr_name = "coord"; //имя в шейдере
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1) {
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void InitVBO() {
	//VBO - это буфер, в котором будут храниться данные вершин.
	glGenBuffers(1, &VBO);
	// Вершины нашего треугольника
	Vertex triangle[15] = {
		//// Квадрат
		//{ -0.75f, 0.25f, 1.0f, 0.0f, 0.0f }, 
		//{ -0.25f, 0.25f, 0.0f, 1.0f, 0.0f }, 
		//{ -0.25f, 0.75f, 0.0f, 0.0f, 1.0f }, 
		//{ -0.75f, 0.75f, 1.0f, 1.0f, 0.0f }, 

		{ -0.75f, 0.25f, 1.0f, 0.0f, 0.0f },
		{ -0.25f, 0.25f, 1.0f, 0.0f, 0.0f },
		{ -0.25f, 0.75f, 1.0f, 0.0f, 0.0f },
		{ -0.75f, 0.75f, 1.0f, 0.0f, 0.0f },
		// Веер
		{ 0.5f, 0.25f, 1.0f, 0.0f, 1.0f },
		{ 0.25f, 0.4f, 0.0f, 1.0f, 1.0f },
		{ 0.35f, 0.5f, 1.0f, 0.5f, 0.0f },
		{ 0.5f, 0.55f, 0.5f, 0.5f, 0.5f },
		{ 0.65f, 0.5f, 0.2f, 0.8f, 0.2f },
		{ 0.75f, 0.4f, 0.8f, 0.2f, 0.8f },
		// Правильный пятиугольник
		{ -0.365f, -1.0f, 0.0f, 1.0f, 0.0f },
		{ -0.6f, -0.3f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.1f, 1.0f, 0.0f, 1.0f },
		{ 0.6f, -0.3f, 1.0f, 1.0f, 0.0f },
		{ 0.365f, -1.0f, 0.0f, 1.0f, 0.0f },
	};
	// Передаем вершины в буфер
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	checkOpenGLerror(); //Пример функции есть в лабораторной
	// Проверка ошибок OpenGL, если есть то вывод в консоль тип ошибки
}

void Draw() {
	// Устанавливаем шейдерную программу текущей
	glUseProgram(Program);
	// Включаем массив атрибутов для координат
	glEnableVertexAttribArray(Attrib_vertex);
	// Подключаем VBO для координат
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	// Включаем массив атрибутов для цвета
	glEnableVertexAttribArray(1);
	// Подключаем VBO для цвета (начинаем с третьего компонента, потому что первые два - координаты)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	// квадрат
	glDrawArrays(GL_QUADS, 0, 4);
	// веер
	glDrawArrays(GL_TRIANGLE_FAN, 4, 6);
	// пятиугольник
	glDrawArrays(GL_POLYGON, 10, 5);

	// Отключаем массив атрибутов
	glDisableVertexAttribArray(Attrib_vertex);
	glDisableVertexAttribArray(1);

	// Отключаем шейдерную программу
	glUseProgram(0);
	checkOpenGLerror();
}


// В момент инициализации разумно произвести загрузку текстур, моделей и других вещей
void Init() {
	// Шейдеры
	InitShader();
	//

	//Вершинный буфер
	InitVBO();
}