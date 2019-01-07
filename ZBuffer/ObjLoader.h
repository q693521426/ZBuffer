#pragma once

#include "Common.h"
#include <string>

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	bool loadFromFile(std::string);
};

