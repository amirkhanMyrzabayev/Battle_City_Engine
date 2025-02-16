#pragma once


#include <glm/vec2.hpp>
#include <memory>
#include <array>

class Tank;
class Level;
class StartScreen;

class Game {
public:
	Game(const glm::vec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
	unsigned int getCurrentWidth() const;
	unsigned int getCurrentHeight() const;

private:
	std::array<bool, 349> m_keys;

	enum class EGameState {
		StartScreen,
		LevelStart,
		Level,
		Pause,
		Scores,
		GameOver
	};

	std::shared_ptr<StartScreen> m_pStartScreen;

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::shared_ptr<Tank> m_pTank;
	std::shared_ptr<Level> m_pLevel;
};