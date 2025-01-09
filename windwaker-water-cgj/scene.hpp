#include <vector>
#include "../libs/mgl/mgl.hpp"
#include "../libs/mgl/mglTexture.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

typedef struct {
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
} CameraStatus;

typedef struct {
    float fovy;
    float aspect;
    float near;
    float far;
} PerspectiveProjection;

class SceneNode {
private:
    const GLuint UBO_BP = 0;
    mgl::Mesh* mesh = nullptr;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
    std::vector<SceneNode*> children;
    GLint ModelMatrixId;
    GLint ColorId;
    GLint LightPosId;
    GLint CameraPosId;
    mgl::ShaderProgram* Shaders = nullptr;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	bool isSkybox = false;
    GLint SkyboxTextureId;
    mgl::TextureCubeMap texture;

    bool isWater = false;

public:
    void addMesh(std::string mesh_fullname);
    void toggleSkybox();
	void loadCubeMap(std::string prefix, std::string suffix);
    void toggleWater();
    void draw(glm::vec3 cameraPos);
    void add(SceneNode* child);
    void rotate(float angle, glm::vec3 axis);
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void setPosition(glm::vec3 vector);
    void createShaderPrograms();
    void setColor(float r, float g, float b, float a);
    std::vector<SceneNode*> getChildren();

};

class SceneGraph {
private:
    SceneNode* root;

    const GLuint UBO_BP = 0;
    mgl::Camera* camera = nullptr;

    uint8_t currentCamera = 0;
    CameraStatus cameraStatus = {
        glm::vec3(0.0f, 5.0f, 0.1f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    PerspectiveProjection perspectiveProjection = {
        30.0f,
        640.0f / 480.0f,
        0.1f,
        100.0f
    };

public:
    void createCamera();
    void updateProjections(float width, float height);
    void cameraZoom(float zoom);
    void rotateCamera(float angle_x, float angle_y);
    void setRoot(SceneNode* node);
    void draw(glm::vec3 cameraPos);
    void createShaderPrograms();
    glm::vec3 getCameraPos();
};