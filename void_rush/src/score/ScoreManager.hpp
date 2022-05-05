#pragma once
#include <string>
#include <fstream>
#include "generation/Position_generator.hpp"//To use difficulty

class ScoreManager {
public:
	ScoreManager();
	~ScoreManager();
	
	float GetScore()const;

	void setDamageScore();
	void Update(float dt);
	void SetPlayerSpeed(float speed);
	void SetDifficulty(Difficulity diff);
	void SetStartPlatform(Platform*& platform);
	void SetSeed(int seed);
	void SetScore(float points);
	void AddScore(float points);
	void LevelDone();
	void Reset();

	void WriteScore(std::string name, std::string file = "assets/files/highScores.txt");
private:
	void SortScores(std::string file = "assets/files/highScores.txt");
	float levelTime;
	float levelLength;
	Difficulity levelDifficulty;
	float playerSpeed;
	float score;
	int seed;

	const float constPoints = 0.1f;//Points given each update;
	const float puzzlePoints = 100.0f;//Points given when puzzle is done
	const float levelPoints = 1000.0f;//Points given when level is done (Depends on how fast the player is)
	const float deathPoints = -5.0f;//Points given when player looses a life

	const int maxScores = 5;
};