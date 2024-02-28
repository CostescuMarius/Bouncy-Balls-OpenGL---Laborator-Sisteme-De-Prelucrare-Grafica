#include <GLFW/glfw3.h>
#include <windows.h>
#include <gl/GLU.h>
#include <iostream>

using namespace std;

const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 900;

bool rightClickPressed = false;
double lastX = WINDOW_WIDTH / 2.0;
double lastY = WINDOW_HEIGHT / 2.0;
float yaw = -90.0f;
float pitch = 0.0f;

void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bouncy Ball", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);



        GLUquadric* quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_LINE);

        glColor3f(1.0f, 0.0f, 0.0f);
        gluSphere(quad, 0.2, 40, 20);
        gluDeleteQuadric(quad);


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// ESC closes windows
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    const float sensitivity = 0.2f;

    static double lastX = xpos;
    static double lastY = ypos;

    if (rightClickPressed)
    {
        double xOffset = (xpos - lastX) * sensitivity;
        double yOffset = (lastY - ypos) * sensitivity;

        yaw += xOffset;
        pitch += yOffset;
    }

    lastX = xpos;
    lastY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rightClickPressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rightClickPressed = false;
    }
}