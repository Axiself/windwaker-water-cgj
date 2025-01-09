////////////////////////////////////////////////////////////////////////////////
//
//  Loading meshes from external files
//
// Copyright (c) 2023-24 by Carlos Martinho
//
// INTRODUCES:
// MODEL DATA, ASSIMP, mglMesh.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include "scene.hpp"
#include <chrono>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) override;
    void scrollCallback(GLFWwindow* win, double xoffset, double yoffset) override;
    void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override;
    void cursorCallback(GLFWwindow* win, double xpos, double ypos) override;

private:
    SceneGraph scene;
	SceneNode Skybox;
    SceneNode Ocean;
    SceneNode WaterPlane;
    SceneNode WaterFoam;
    SceneNode Brick;

    double mouse_x, mouse_y;
    bool rotating = false;

    float t = 0.0f;
    bool leftKeyPressed = false;
    bool rightKeyPressed = false;

    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

    void createMeshes();
    void createShaderPrograms();
    void createCamera();
    void drawScene(double elapsed);
    void createScene();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    WaterPlane.addMesh("../assets/models/WaterPlane.obj");
	WaterFoam.addMesh("../assets/models/Foam.obj");
	Brick.addMesh("../assets/models/brick.obj");
	Skybox.addMesh("../assets/models/cube-v.obj");
}

///////////////////////////////////////////////////////////////////////// SCENE

void MyApp::createScene() {
	// create skybox
    Skybox.toggleSkybox();
	Skybox.loadCubeMap("../assets/textures/lost_at_sea", ".png");
    WaterPlane.loadCubeMap("../assets/textures/lost_at_sea", ".png");
	Brick.loadNormalMap("../assets/textures/normal.png", "../assets/textures/diffuse.png");

    // set object colors
	WaterFoam.setColor(1.0f, 1.0f, 1.0f, 1.0f);
    WaterFoam.translate(glm::vec3(0.0f, 0.01f, 0.0f));
    WaterPlane.setColor(0.0f, 0.3f, 0.6f, 1.0f);
    WaterPlane.toggleWater();
    Brick.toggleBrick();

    // add objects to the root object
	Ocean.add(&Skybox);
	Ocean.add(&WaterFoam);
	Ocean.add(&Brick);
    Ocean.add(&WaterPlane);

    // set root
    scene.setRoot(&Ocean);
}


///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    scene.createShaderPrograms();
}

///////////////////////////////////////////////////////////////////////// CAMERA

void MyApp::createCamera() {
    scene.createCamera();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene(double elapsed) {
    scene.draw(scene.getCameraPos());
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    createMeshes();
    createScene();
    createShaderPrograms();  // after mesh;
    createCamera();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
    scene.updateProjections(static_cast<float>(winx), static_cast<float>(winy));
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(elapsed); }

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    scene.cameraZoom(static_cast<float>(yoffset));
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(win, &mouse_x, &mouse_y);
        rotating = true;
    }
    else if (action == GLFW_RELEASE) {
        rotating = false;
    }
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
    if (!rotating) return;

    const double sensitivity = 0.005f;

    double dx = xpos - mouse_x;
    double dy = ypos - mouse_y;

    mouse_x = xpos;
    mouse_y = ypos;

    float angle_x = static_cast<float>(-dx * sensitivity);
    float angle_y = static_cast<float>(-dy * sensitivity);

    scene.rotateCamera(angle_x, angle_y);
}


/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(new MyApp());
    engine.setOpenGL(4, 6);
    engine.setWindow(800, 600, "Windwaker Water CGJ", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
