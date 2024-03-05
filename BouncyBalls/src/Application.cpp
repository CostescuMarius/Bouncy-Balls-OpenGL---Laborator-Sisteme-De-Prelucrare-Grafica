#include <GLFW/glfw3.h>
#include <windows.h>
#include <gl/GLU.h>
#include <iostream>

using namespace std;

void setCameraPerspective(GLFWwindow* window);

void drawSphere();
void drawPlane();

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 900;

bool rightClickPressed = false;

double lastX = WINDOW_WIDTH / 2.0;
double lastY = WINDOW_HEIGHT / 2.0;

float cameraPositionX = 0.0f;
float cameraPositionY = 0.0f;
float cameraPositionZ = 3.0f;

float yaw = -90.0f;
float pitch = 30.0f;

float currentFallPosition = 2.0f;
float fallPosition = currentFallPosition;
float fallSpeed = 0.001f;
float fallAcceleration = 0.00001f;
float jumpAcceleration = 0.00001f;
boolean isFalling = true;


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

        drawPlane();

        drawSphere();

        setCameraPerspective(window);


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
    gluPerspective(120.0f, (float)sphereWidth / (float)sphereHeight, 0.1f, 100.0f);

    glTranslatef(-cameraPositionX, -cameraPositionY, -cameraPositionZ);

    glRotatef(pitch, 1.0f, 0.0f, 0.0f);

    glRotatef(yaw, 0.0f, 1.0f, 0.0f);

    glTranslatef(cameraPositionX, cameraPositionY, cameraPositionZ);

    gluLookAt(cameraPositionX, cameraPositionY, cameraPositionZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f);
}

void drawSphere() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_LINE);

    glTranslatef(0.0f, fallPosition, 0.0f);

    if (currentFallPosition > 0.3f) {
        if (isFalling) {
            if (fallPosition > 0.3f) {
                fallPosition -= fallSpeed;
                fallSpeed += fallAcceleration;
            }
            else {
                isFalling = false;
            }
        }
        else {
            if (fallPosition <= 0.7 * currentFallPosition) {
                fallPosition += fallSpeed;
                fallSpeed -= jumpAcceleration;
            }
            else {
                currentFallPosition = 0.7 * currentFallPosition;
                fallSpeed = 0.001f;
                isFalling = true;
            }
        }
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    gluSphere(quad, 0.3, 40, 40);
    gluDeleteQuadric(quad);
}

void drawPlane() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);    

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, 0.0f, -2.0f);
    glVertex3f(-2.0f, 0.0f, 2.0f);
    glVertex3f(2.0f, 0.0f, 2.0f);
    glVertex3f(2.0f, 0.0f, -2.0f);
    glEnd();
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

    if (cameraPositionZ < 1.0f)
        cameraPositionZ = 1.0f;
    if (cameraPositionZ > 10.0f)
        cameraPositionZ = 10.0f;
}