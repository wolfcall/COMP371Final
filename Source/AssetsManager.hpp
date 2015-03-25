#pragma once

#include <memory>

class AssetsManager
{
private:
	AssetsManager()
	{}
	~AssetsManager()
	{}
public:
	static *AssetsManager getInstance()
	{
		static std::unique_ptr<AssetsManager> *ptr = nullptr;
		if (!ptr)
		{
			ptr = std::make_unique<AssetsManager>();
		}
		return ptr;
	}
};