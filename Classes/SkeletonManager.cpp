#include "SkeletonManager.h"

SkeletonManager* SkeletonManager::instance;
Map<std::string, SkeletonRenderer *> SkeletonManager::skeleRendererCache;


SkeletonManager::SkeletonManager()
{
}

SkeletonManager::~SkeletonManager()
{
}

spSkeletonData* SkeletonManager::getSkeletonData(string filename)
{
	if (!SkeletonManager::skeleRendererCache.at(filename)) {
		return nullptr;
	}
	else {
		auto data = SkeletonManager::skeleRendererCache.at(filename)->getSkeleton()->data;
		return data;
	}
	
}

SkeletonManager * SkeletonManager::getInstance()
{
	if (SkeletonManager::instance == nullptr) {
		SkeletonManager::instance = new SkeletonManager();
	}
	return SkeletonManager::instance;
}

void SkeletonManager::cacheSkeleton(string filename, float scale)
{
	auto jsonFile = filename + ".json";
	auto atlasFile = filename + ".atlas";
	auto skeleRenderer = SkeletonRenderer::createWithFile(jsonFile.c_str(), atlasFile.c_str(), scale);
	skeleRendererCache.insert(filename, skeleRenderer);
}
