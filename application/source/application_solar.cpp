#include "application_solar.hpp"
#include "launcher.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include <glbinding/gl/gl.h>
#include "stars.h"
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
	:Application{ resource_path }
	, planet_object{}
{
	initializeGeometry();
	initializeShaderPrograms();
}

void ApplicationSolar::render() const {
	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);
	Planet* planetStrut = PlanetStrut();
	for (int i = 0; i < 11; ++i) {
		upload_planet_transforms(planetStrut[i]);
	}
	
	
}
Planet * ApplicationSolar::PlanetStrut() const {
	Planet sun;
	sun.scaleX = 2.0f;
	sun.scaleY = 2.0f;
	sun.scaleZ = 2.0f;
	sun.translateX = 0.0f;
	sun.translateZ = 0.0f;
	sun.timeDiff = 0.1f;
		
	
	Planet mercury;
	mercury.scaleX = 0.12f;
	mercury.scaleY = 0.12f;
	mercury.scaleZ = 0.12f;
	mercury.translateX = 22.0f;
	mercury.translateZ = -0.08f;
	mercury.timeDiff = 3.5f;
	
	
	Planet venus;
	venus.scaleX = 0.4f;
	venus.scaleY = 0.4f;
	venus.scaleZ = 0.4f;
	venus.translateX = 9.0f;
	venus.translateZ = -0.09f;
	venus.timeDiff = 1.4f;

	Planet earth;
	earth.scaleX = 0.32f;
	earth.scaleY = 0.32f;
	earth.scaleZ = 0.32f;
	earth.translateX = 16.0f;
	earth.translateZ = -0.04f;
	earth.timeDiff = 1.2f;

	
	Planet luna;
	luna.scaleX = 0.06f;
	luna.scaleY = 0.06f;
	luna.scaleZ = 0.06f;
	luna.translateX = 93.5f;
	luna.translateZ = -0.04f;
	luna.timeDiff = 1.2f;


	Planet mars;
	mars.scaleX = 0.27f;
	mars.scaleY = 0.27f;
	mars.scaleZ = 0.27f;
	mars.translateX = 28.0f;
	mars.translateZ = -0.08f;
	mars.timeDiff = 1.0f;

	
	
	Planet jupiter;
	jupiter.scaleX = 1.4f;
	jupiter.scaleY = 1.4f;
	jupiter.scaleZ = 1.4f;
	jupiter.translateX = 8.0f;
	jupiter.translateZ = -0.08f;
	jupiter.timeDiff = 0.5f;

	
	Planet saturn;
	saturn.scaleX = 1.1f;
	saturn.scaleY = 1.1f;
	saturn.scaleZ = 1.1f;
	saturn.translateX = 14.0f;
	saturn.translateZ = -0.08f;
	saturn.timeDiff = 0.3f;

	
	Planet uranus;
	uranus.scaleX = 0.75f;
	uranus.scaleY = 0.75f;
	uranus.scaleZ = 0.75f;
	uranus.translateX = 25.5f;
	uranus.translateZ = -0.08f;
	uranus.timeDiff = 0.25f;

	
	Planet neptune;
	neptune.scaleX = 0.76f;
	neptune.scaleY = 0.76f;
	neptune.scaleZ = 0.76f;
	neptune.translateX = 28.2f;
	neptune.translateZ = -0.08f;
	neptune.timeDiff = 0.19f;

	
	Planet pluto;
	pluto.scaleX = 0.09f;
	pluto.scaleY = 0.09f;
	pluto.scaleZ = 0.09f;
	pluto.translateX = 290.0f;
	pluto.translateZ = 1.3f;
	pluto.timeDiff = 0.15f;

	Planet arrayOfPlanets[11] = { sun, mercury, venus, earth, luna, mars, jupiter, saturn, uranus, neptune, pluto };
	return arrayOfPlanets;


}

Stars ApplicationSolar::StarStrut() const {
		
}


void ApplicationSolar::upload_planet_transforms(Planet planet) const {
	

	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime() * planet.timeDiff),
		glm::fvec3{ 0.0f, 1.0f, 0.0f });

	//    VERY IMPORTANT!!!! THE ORDER OF SCALING/ROTATION/TRANSFORMATION WILL AFFECT THE FINAL RESULT AND RENDER!!!!!

	//scale first from the properties of variable thePlanet
	model_matrix = glm::scale(model_matrix, glm::fvec3({ planet.scaleX, planet.scaleY, planet.scaleZ }));
	//translate the sphere according to the vector we have in the 2nd param
	model_matrix = glm::translate(model_matrix, glm::fvec3{ planet.translateX, 0.0f, planet.translateZ });

	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
		1, GL_FALSE, glm::value_ptr(model_matrix));

	// extra matrix for normal transformation to keep them orthogonal to surface
	//Used for the colors
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
		1, GL_FALSE, glm::value_ptr(normal_matrix));

	// bind the VAO to draw
	glBindVertexArray(planet_object.vertex_AO);

	// draw bound vertex array using bound shader
	glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
	
}

void ApplicationSolar::updateView() {
	// vertices are transformed in camera space, so camera transform must be inverted
	glm::fmat4 view_matrix = glm::inverse(m_view_transform);
	// upload matrix to gpu
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
		1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::updateProjection() {
	// upload matrix to gpu
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
		1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
	updateUniformLocations();

	// bind new shader
	glUseProgram(m_shaders.at("planet").handle);

	updateView();
	updateProjection();
}

// handle key input
void ApplicationSolar::keyCallback(int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, -0.1f });
		updateView();
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, 0.1f });
		updateView();
	}
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
	// mouse handling
	pos_x *= 0.1;
	pos_y *= 0.1;
	//for now make the z axis just like y axis, so up and down on the mouse is zoom in and out
	double pos_z = pos_y;

	//pass the processed variables to the translate and updateView(), and enjoy
	m_view_transform = glm::translate(m_view_transform, glm::fvec3{ pos_x, 0.0f, pos_z });
	updateView();
}

// load shader programs
void ApplicationSolar::initializeShaderPrograms() {
	// store shader program objects in container
	m_shaders.emplace("planet", shader_program{ m_resource_path + "shaders/simple.vert",
		m_resource_path + "shaders/simple.frag" });
	// request uniform locations for shader program
	m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
	m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
	m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
	m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
	model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

	// generate vertex array object
	glGenVertexArrays(1, &planet_object.vertex_AO);
	// bind the array for attaching buffers
	glBindVertexArray(planet_object.vertex_AO);

	// generate generic buffer
	glGenBuffers(1, &planet_object.vertex_BO);
	// bind this as an vertex array buffer containing all attributes
	glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
	// configure currently bound array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

	// activate first attribute on gpu
	glEnableVertexAttribArray(0);
	// first attribute is 3 floats with no offset & stride
	glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
	// activate second attribute on gpu
	glEnableVertexAttribArray(1);
	// second attribute is 3 floats with no offset & stride
	glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

	// generate generic buffer
	glGenBuffers(1, &planet_object.element_BO);
	// bind this as an vertex array buffer containing all attributes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
	// configure currently bound array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

	// store type of primitive to draw
	planet_object.draw_mode = GL_TRIANGLES;
	// transfer number of indices to model object 
	planet_object.num_elements = GLsizei(planet_model.indices.size());
}

ApplicationSolar::~ApplicationSolar() {
	glDeleteBuffers(1, &planet_object.vertex_BO);
	glDeleteBuffers(1, &planet_object.element_BO);
	glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

// exe entry point
int main(int argc, char* argv[]) {
	Launcher::run<ApplicationSolar>(argc, argv);
	
}

