#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}


class ResourceManager {
public:
	static void setExecutablePath(const std::string& path);
	static void unloadAllResources();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;


	//SHADERS------------------------------------------------------------------------
	static std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, 
														 const std::string& vertexPath, 
														 const std::string& fragmentPath);
	static std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);
	//---------------------------------------------------------------------------------


	//TEXTURES-------------------------------------------------------------------------------
	static std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);
	//-----------------------------------------------------------------------------------------


	//SPRITES---------------------------------------------------------------------------------
	static std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
												 const std::string& textureName,
												 const std::string& shaderName,
												 const unsigned int spriteWidth,
												 const unsigned int spriteHeight,
												 const std::string& subTextureName = "default");

	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);
	//---------------------------------------------------------------------------------------


	//ANIMATED SPRITES----------------------------------------------------------------------
	static std::shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(const std::string& animatedSpriteName,
																		 const std::string& textureName,
																		 const std::string& shaderName,
																		 const unsigned int spriteWidth,
																		 const unsigned int spriteHeight,
																		 const std::string& subTextureName = "default");

	static std::shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(const std::string& animatedSpriteName);
	//---------------------------------------------------------------------------------------



	//TEXTURE ATLAS-------------------------------------------------------------------------
	static std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(const std::string& textureName,
														  const std::string& texturePath,
														  const std::vector<std::string> subTexture,
														  const unsigned int subTextureWidth,
														  const unsigned int subTextureHeight);
	//-----------------------------------------------------------------------------------------

private:
	static std::string getFileString(const std::string& relativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
	static ShaderProgramMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animatedSprites;

	static std::string m_path;
};