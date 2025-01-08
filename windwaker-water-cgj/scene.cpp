#include "scene.hpp"
#include <iostream>


void SceneGraph::createShaderPrograms() {
	root->createShaderPrograms();
}

void SceneGraph::createCamera() {
	camera = new mgl::Camera(UBO_BP);

	camera->setViewMatrix(glm::lookAt(
		cameraStatus.eye,
		cameraStatus.center,
		cameraStatus.up));

	camera->setProjectionMatrix(glm::perspective(
		glm::radians(perspectiveProjection.fovy),
		perspectiveProjection.aspect,
		perspectiveProjection.near,
		perspectiveProjection.far));
}

void SceneGraph::updateProjections(float width, float height) {
	float aspect_ratio = width / height;
	perspectiveProjection.aspect = aspect_ratio;

	camera->setProjectionMatrix(glm::perspective(
		glm::radians(perspectiveProjection.fovy),
		perspectiveProjection.aspect,
		perspectiveProjection.near,
		perspectiveProjection.far));
}

void SceneGraph::rotateCamera(float angle_x, float angle_y) {
	glm::quat yaw = glm::angleAxis(angle_x, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 cam_direction = glm::normalize(cameraStatus.center - cameraStatus.eye);
	glm::vec3 cam_right = glm::normalize(glm::cross(cam_direction, cameraStatus.up));

	glm::quat pitch = glm::angleAxis(angle_y, cam_right);

	glm::quat q = pitch * yaw;

	cameraStatus.eye = q * cameraStatus.eye;

	cameraStatus.up = q * cameraStatus.up;

	camera->setViewMatrix(glm::lookAt(
		cameraStatus.eye,
		cameraStatus.center,
		cameraStatus.up));
}


void SceneGraph::cameraZoom(float zoom) {
	const float ZOOM_SPEED = 0.05f;

	glm::vec3 direction = cameraStatus.center - cameraStatus.eye;
	cameraStatus.eye += direction * ZOOM_SPEED * zoom;

	camera->setViewMatrix(glm::lookAt(
		cameraStatus.eye,
		cameraStatus.center,
		cameraStatus.up));
}

void SceneGraph::setRoot(SceneNode* root) {
	this->root = root;
}

void SceneGraph::draw() {
	root->draw();
}

void SceneNode::add(SceneNode* child) {
	children.push_back(child);
}

void SceneNode::addMesh(std::string mesh_fullname) {
	mesh = new mgl::Mesh();
	mesh->joinIdenticalVertices();
	mesh->create(mesh_fullname);
}

void SceneNode::createSkybox(std::string prefix, std::string suffix) {
	isSkybox = true;
	texture.loadCubeMap(prefix, suffix);
}

void SceneNode::createShaderPrograms() {
	if (mesh != nullptr) {
		Shaders = new mgl::ShaderProgram();
		if (!isSkybox) {
			Shaders->addShader(GL_VERTEX_SHADER, "scene-vs.glsl");
			Shaders->addShader(GL_FRAGMENT_SHADER, "scene-fs.glsl");
		}
		else {
			Shaders->addShader(GL_VERTEX_SHADER, "skybox-vs.glsl");
			Shaders->addShader(GL_FRAGMENT_SHADER, "skybox-fs.glsl");
		}

		Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
		if (mesh->hasNormals()) {
			Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
		}
		if (mesh->hasTexcoords()) {
			Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
		}
		if (mesh->hasTangentsAndBitangents()) {
			Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
		}

		Shaders->addUniform(mgl::COLOR_ATTRIBUTE);
		Shaders->addUniform(mgl::LIGHT_POS_ATTRIBUTE);
		Shaders->addUniform(mgl::MODEL_MATRIX);
		Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
		Shaders->create();

		ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
		ColorId = Shaders->Uniforms[mgl::COLOR_ATTRIBUTE].index;
		LightPosId = Shaders->Uniforms[mgl::LIGHT_POS_ATTRIBUTE].index;
	}

	for (auto& child : children) {
		child->createShaderPrograms();
	}
}

void SceneNode::setColor(float r, float g, float b, float a) {
	color = glm::vec4(r, g, b, a);
}

void SceneNode::draw() {
	if (color[3] < 1.0f) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	if (mesh != nullptr) {
		Shaders->bind();

		if (isSkybox) {
			texture.bind();
			glDisable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
		}

		glUniform4fv(ColorId, 1, glm::value_ptr(color));
		glUniform3fv(LightPosId, 1, glm::value_ptr(glm::vec3(0.0, 2.0, 0.0)));
		glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		mesh->draw();

		if (isSkybox) {
			texture.unbind();
			glEnable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);
		}

		Shaders->unbind();
	}

	for (auto& child : children) {
		child->draw();
	}
}

void SceneNode::rotate(float angle, glm::vec3 vector) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), vector);
	for (auto& child : children) {
		child->rotate(angle, vector);
	}
}

void SceneNode::translate(glm::vec3 vector) {
	modelMatrix = glm::translate(modelMatrix, vector);
	for (auto& child : children) {
		child->translate(vector);
	}
}

void SceneNode::scale(glm::vec3 vector) {
	modelMatrix = glm::scale(modelMatrix, vector);
	for (auto& child : children) {
		child->scale(vector);
	}
}

void SceneNode::setPosition(glm::vec3 position) {
	// Update the translation part of the model matrix
	modelMatrix[3] = glm::vec4(position, 1.0f);

	for (auto& child : children) {
		child->setPosition(position);
	}
}

std::vector<SceneNode*> SceneNode::getChildren() {
	return children;
}