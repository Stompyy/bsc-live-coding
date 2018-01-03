#pragma once

#include <vector>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//https://www.youtube.com/watch?v=F-kcaonjHf8&index=2&list=PLRIWtICgwaX2tKWCxdeB7Wv_rTET9JtWW
class Joint
{
public:
	Joint(int id, const std::string& name, const glm::mat4& orginalBindTransform);
	Joint();
	~Joint();

	void addChildJoint(Joint * joint);
	void destory();

	glm::mat4& getTransform()
	{
		return m_Transformation;
	};

	glm::mat4& getBindTransform()
	{
		return m_OriginalBindTransform;
	};


	glm::mat4& getInverseBindTransform()
	{
		return m_InverseBindTransform;
	};

	void calculateInverseBindTransform(glm::mat4& parentBindTransform);

	void displayToConsole();

private:
	int m_ID;
	std::vector<Joint*> m_Children;
	std::string m_Name;
	glm::mat4 m_Transformation;
	glm::mat4 m_OriginalBindTransform;
	glm::mat4 m_InverseBindTransform;
};


