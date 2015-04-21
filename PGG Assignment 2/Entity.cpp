#include "Entity.h"
#include "Utility.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 initPosition, std::string modelFilename, std::string textureFilename,
	ResourceManager* manager) 
		: pos(initPosition)
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
	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2));
}