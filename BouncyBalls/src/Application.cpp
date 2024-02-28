#include <GLFW/glfw3.h>
#include <windows.h>
#include <gl/GLU.h>
#include <iostream>

using namespace std;

const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 900;

bool rightClickPressed = false;
float cameraPositionZ = -3.0f;
double lastX = WINDOW_WIDTH / 2.0;
double lastY = WINDOW_HEIGHT / 2.0;
float yaw = -90.0f;
float pitch = 0.0f;

void setCameraPerspective(GLFWwindow* window);

void drawSphere();

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setCameraPerspective(window);
        
        drawSphere();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void setCameraPerspective(GLFWwindow* window) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int sphereWidth;
    int sphereHeight;
    glfwGetFramebufferSize(window, &sphereWidth, &sphereHeight);
    gluPerspective(45.0f, (float)sphereWidth / (float)sphereHeight, 0.1f, 100.0f);

    glTranslatef(0.0f, 0.0f, cameraPositionZ);

    glRotatef(pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
}

void drawSphere() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_LINE);

    glColor3f(1.0f, 0.0f, 0.0f);
    gluSphere(quad, 0.3, 40, 40);
    gluDeleteQuadric(quad);
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    const float scrollSensitivity = 0.5f;


    cameraPositionZ += yoffset * scrollSensitivity;
}