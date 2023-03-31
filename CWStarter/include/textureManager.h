#pragma once

#include <map>
#include<SFML/Graphics.hpp>
#include <string.h>
#include <memory.h>

class TextureManager
{
private:
	std::map<std::string,std::shared_ptr<sf::Texture>>m_textures;
public:
	~TextureManager();
	bool addTexture(std::string key, std::string fileName);//!< Add a texture to the texture manager, return bool for error checking
	sf::Texture* getTexture(std::string key) const; //!<Return texture as an index
};
