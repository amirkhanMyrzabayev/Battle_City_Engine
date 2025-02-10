#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"



#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"



ResourceManager::ShaderProgramMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;

void ResourceManager::setExecutablePath(const std::string& path)
{
	size_t found = path.find_last_of("/\\");
	m_path = path.substr(0, found);
}

void ResourceManager::unloadAllResources()
{
	m_shaderPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cout << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}


//SHADERS------------------------------------------------------------------------
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,
																	  const std::string& vertexPath,
																	  const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, 
														std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;

	if (newShader->isCompiled())
	{
		return newShader;
	}
	std::cerr << "Can't load shader program:\n" 
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program " << shaderName << std::endl;
	return nullptr;
}
//SHADERS------------------------------------------------------------------------


//TEXTURES-------------------------------------------------------------------------------
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(
																		width, height, pixels, channels, 
																		GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);
	return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the texture " << textureName << std::endl;
	return nullptr;
}
//TEXTURES-------------------------------------------------------------------------------


//SPRITES---------------------------------------------------------------------------------
std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
															  const std::string& textureName,
															  const std::string& shaderName,
															  const unsigned int spriteWidth,
															  const unsigned int spriteHeight,
															  const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture " << textureName << "for the sprite " << spriteName << std::endl;
	}

	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program " << shaderName << "for the sprite" << spriteName << std::endl;
	}
		
	std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite>(pTexture, 
																												subTextureName, 
																												pShader, 
																												glm::vec2(0.f, 0.f), 
																												glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the sprite " << spriteName << std::endl;
	return nullptr;
}
//SPRITES-----------------------------------------------------------------------------------------------





//ANIMATED SPRITES----------------------------------------------------------------------
std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& animatedSpriteName,
																			  const std::string& textureName,
																		 	  const std::string& shaderName,
																			  const unsigned int spriteWidth,
																		 	  const unsigned int spriteHeight,
																			  const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture " << textureName << "for the animated sprite " << animatedSpriteName << std::endl;
	}

	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program " << shaderName << "for the animated sprite" << animatedSpriteName << std::endl;
	}

	std::shared_ptr<RenderEngine::AnimatedSprite> newAnimatedSprite = m_animatedSprites.emplace(animatedSpriteName, 
																							std::make_shared<RenderEngine::AnimatedSprite>(pTexture,
																															subTextureName,
																															pShader,
																															glm::vec2(0.f, 0.f),
																															glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newAnimatedSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& animatedSpriteName)
{
	auto it = m_animatedSprites.find(animatedSpriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the animated sprite " << animatedSpriteName << std::endl;
	return nullptr;
}
//ANIMATED SPRITES----------------------------------------------------------------------


//TEXTURE ATLAS-------------------------------------------------------------------------
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(const std::string& textureName,
																	   const std::string& texturePath,
																	   const std::vector<std::string> subTextures,
																	   const unsigned int subTextureWidth,
																	   const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textutreHeight = pTexture->height();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textutreHeight;
		for (const auto& currentSubTexureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
									static_cast<float>(currentTextureOffsetY - subTextureHeight) / textutreHeight);

			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
									static_cast<float>(currentTextureOffsetY) / textutreHeight);

			pTexture->addSubTexture(std::move(currentSubTexureName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}
	return pTexture;
}
//TEXTURE ATLAS-------------------------------------------------------------------------


//JSON PARSING-------------------------------------------------------------------------------
bool ResourceManager::loadJSONResources(const std::string& JSONpath)
{
	const std::string JSONString = getFileString(JSONpath);
	if (JSONpath.empty())
	{
		std::cerr << "No JSON resources file" << std::endl;
		return false;
	}
	
	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << " (" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "In JSON file: " << JSONpath << std::endl;
		return false;
	}

	auto shadersIt = document.FindMember("shaders");
	if (shadersIt != document.MemberEnd())
	{
		for (const auto& currentShader : shadersIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();
			loadShaders(name, filePath_v, filePath_f);
		}
	}

	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		for (const auto& currentTextureAtlas : textureAtlasesIt->value.GetArray())
		{
			const std::string name = currentTextureAtlas["name"].GetString();
			const std::string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetUint();
			
			const auto subTexturesArray = currentTextureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());
			for (const auto& currentSubTexture : subTexturesArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}
			loadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
		}
	}

	auto animatedSpritesIt = document.FindMember("animatedSprites");
	if (animatedSpritesIt != document.MemberEnd())
	{
		for (const auto& currentAnimatedSprite : animatedSpritesIt->value.GetArray())
		{
			const std::string name = currentAnimatedSprite["name"].GetString();
			const std::string textureAtlas = currentAnimatedSprite["textureAtlas"].GetString();
			const std::string shader = currentAnimatedSprite["shader"].GetString();
			const std::string initialSubTexture = currentAnimatedSprite["initialSubTexture"].GetString();
			const unsigned int initialHeight = currentAnimatedSprite["initialHeight"].GetUint();
			const unsigned int initialWidth = currentAnimatedSprite["initialWidth"].GetUint();
			auto pAnimatedSprite = loadAnimatedSprite(name, textureAtlas, shader, initialWidth, initialHeight, initialSubTexture);
			if (!pAnimatedSprite)
			{
				continue;
			}

			const auto statesArray = currentAnimatedSprite["states"].GetArray();
			for (const auto& currentState : statesArray)
			{
				const std::string stateName = currentState["stateName"].GetString();
				std::vector<std::pair<std::string, uint64_t>> frames;
				const auto framesArray = currentState["frames"].GetArray();
				frames.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTexture = currentFrame["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint64();
					frames.emplace_back(std::pair<std::string, uint64_t>(subTexture, duration));
				}
				pAnimatedSprite->insertState(stateName, std::move(frames));
			}
		}
	}
	return true;
}
//JSON PARSING-------------------------------------------------------------------------------