// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <gl/glew.h>

// Include GLFW
#include <glfw/glfw3.h>

static hd::application::Client *m_client;

static Vec3 lastCursorPosition;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	hd::input::EventQueue *eventQueue = m_client->GetEventQueue();

	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			eventQueue->OnPointerSelect(hd::input::pointer::Button::LEFT_BUTTON, hd::input::pointer::Control::MOUSE, lastCursorPosition);
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			eventQueue->OnPointerSelect(hd::input::pointer::Button::RIGHT_BUTTON, hd::input::pointer::Control::MOUSE, lastCursorPosition);
		}
	} else if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			eventQueue->OnPointerRelease(hd::input::pointer::Button::LEFT_BUTTON, hd::input::pointer::Control::MOUSE, lastCursorPosition);
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			eventQueue->OnPointerRelease(hd::input::pointer::Button::RIGHT_BUTTON, hd::input::pointer::Control::MOUSE, lastCursorPosition);
		}
	}
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	hd::input::EventQueue *eventQueue = m_client->GetEventQueue();

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	Vec3 location(xpos, ypos, 0.0);
	Vec3 delta = lastCursorPosition - location;
	lastCursorPosition = location;

	location[1] = height - location[1];
	delta[1] = -delta[1];

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		eventQueue->OnPointerDrag(hd::input::pointer::Button::LEFT_BUTTON, hd::input::pointer::Control::MOUSE, location, delta);
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		eventQueue->OnPointerDrag(hd::input::pointer::Button::RIGHT_BUTTON, hd::input::pointer::Control::MOUSE, location, delta);
	} else {
		eventQueue->OnPointerMove(hd::input::pointer::Control::MOUSE, location, delta);
	}
}


int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//const unsigned int PREFERRED_WINDOW_WIDTH = 1920;
	//const unsigned int PREFERRED_WINDOW_HEIGHT = 1080;

	const unsigned int PREFERRED_WINDOW_WIDTH = 2364; // 1182 * 2
	const unsigned int PREFERRED_WINDOW_HEIGHT = 1464;

	hd::Device::SetDisplayScreenSize(PREFERRED_WINDOW_WIDTH, PREFERRED_WINDOW_HEIGHT);

	glfwWindowHint(GLFW_SAMPLES, 0); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global)
	window = glfwCreateWindow(PREFERRED_WINDOW_WIDTH, PREFERRED_WINDOW_HEIGHT, "Vast", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	lastCursorPosition.Set(xPos, yPos, 0.0);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

#ifdef _DEBUG
#ifdef DEVELOPER_FILE_SYSTEM_ROOT
	hd::FileSystem::Init(DEVELOPER_FILE_SYSTEM_ROOT);
#else
	hd::FileSystem::Init("C:/Users/david/Projects/VastApps/");
#endif
#else
	hd::FileSystem::Init(DEVELOPER_FILE_SYSTEM_ROOT); //[[[NSBundle mainBundle] resourcePath] UTF8String]);
#endif
	hd::graphics::ShaderLoader::Instance()->SetShaderLocation("resources/shaders/gl3/");

	m_client = new hd::application::Client();

	glfwSwapInterval(0);

	static BOOL clientInitted = false;
	do{
		glfwPollEvents();
		if (clientInitted == false) {
			glShadeModel(GL_SMOOTH);
			glClearDepth(1.0f);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPointSize(1.0f);
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_VERTEX_ARRAY);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glDepthMask(TRUE);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			m_client->Init();
			clientInitted = true;
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			m_client->Update();
			m_client->Render();
			glfwSwapBuffers(window);
		}
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	return 0;
}