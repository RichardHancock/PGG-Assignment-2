#include "Entity.h"
#include "Utility.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 initPosition, glm::vec3 rotation, glm::vec3 scale, 
	std::string modelFilename, std::string textureFilename, ResourceManager* manager) 
		: pos(initPosition), rotation(rotation), scale(scale)
{
	if (manager == nullptr)
	{
		Utility::log(Utility::E, "Resource Manager passed in to Entity was NULL");
	}
	else
	{
		model = manager->getModel(modelFilename, textureFilename);
		if (model == nullptr)
			Utility::log(Utility::E, "Model load in Entity returned NULL");
	}
}

Entity::~Entity()
{

}

void Entity::draw(glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader)
{
	model->draw(modelMatrix, viewMatrix, projMatrix, shader);
}

void Entity::update(float dt)
{
	modelMatrix = glm::translate(glm::mat4(1.0f), pos);
	modelMatrix = glm::scale(modelMatrix, scale);
	rotateMat();
}

void Entity::rotateMat()
{
	if (rotation.x != 0.00f)
	{
		modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	}
	if (rotation.y != 0.00f)
	{
		modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	}
	if (rotation.z != 0.00f)
	{
		modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	}
}