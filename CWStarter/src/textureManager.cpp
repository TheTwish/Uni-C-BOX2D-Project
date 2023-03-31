#include "textureManager.h"
TextureManager::~TextureManager()
{
	m_textures.clear();
}

bool TextureManager::addTexture(std::string key, std::string fileName)
{
	// has file been loaded already
	if (m_textures.find(key) != m_textures.end())
	{
		return false;
	}

	//can file be loaded
	sf::Texture temp;
	if (!temp.loadFromFile(fileName))
	{
		return false;
	}

	//add the texture to the container
	m_textures[key] = std::make_shared<sf::Texture>(temp);
	return true;
}

sf::Texture* TextureManager::getTexture(std::string key) const
{
	auto it = m_textures.find(key);//store the key
	if (it != m_textures.end())
	{
		return it->second.get();
	}
	//texture not found
	return nullptr;
}