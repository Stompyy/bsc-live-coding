#pragma once

#include <string>
#include <vector>
#include <map>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct AnimationData
{
	std::string BoneName;
	std::vector<glm::mat4> transformation;
};
struct AnimationClip
{
	std::string Name;
	std::vector<AnimationData*> keyFrames;

	void destroy()
	{
		auto iter = keyFrames.begin();
		while (iter != keyFrames.end())
		{
			if ((*iter))
			{
				delete ((*iter));
				iter = keyFrames.erase(iter);
			}
		}

		keyFrames.clear();
	};
};

class Animation
{
public:
	Animation();
	~Animation();

	void destroy()
	{
		auto iter = m_AnimationClips.begin();
		while (iter != m_AnimationClips.end())
		{
			if ((*iter).second)
			{
				(*iter).second->destroy();
				delete (*iter).second;
				iter++;
			}
		}

		m_AnimationClips.clear();
	};

	void addAnimation(const std::string& name, AnimationClip* clip);
private:
	std::map<std::string, AnimationClip*> m_AnimationClips;
};

