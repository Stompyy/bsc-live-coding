#include "Joint.h"


Joint::Joint(int id, const std::string & name, const glm::mat4 & orginalBindTransform)
{
	m_ID = id;
	m_Name = name;
	m_OriginalBindTransform = orginalBindTransform;
	m_InverseBindTransform = glm::mat4(1.0f);
	m_Transformation = glm::mat4(1.0f);

}

Joint::Joint()
{
}

Joint::~Joint()
{
	destroy();
}

void Joint::addChildJoint(Joint * joint)
{
	m_Children.push_back(joint);
}

void Joint::destroy()
{
	auto iter = m_Children.begin();
	while (iter != m_Children.end())
	{
		if ((*iter))
		{
			delete (*iter);
			(*iter) = nullptr;
			iter = m_Children.erase(iter);
		}
	}
}

void Joint::calculateInverseBindTransform(glm::mat4 & parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform*m_OriginalBindTransform;
	m_InverseBindTransform = glm::inverse(bindTransform);
	for (Joint * child : m_Children)
	{
		child->calculateInverseBindTransform(bindTransform);
	}
}

void Joint::displayToConsole()
{
	printf("Joint %d - %s\n", m_ID, m_Name.c_str());
	for (Joint * child : m_Children)
	{
		child->displayToConsole();
	}
}