#include "ScoreManager.hpp"

ScoreManager::ScoreManager()
{
	Reset();
}

ScoreManager::~ScoreManager()
{
}

float ScoreManager::GetScore() const
{
	return score;
}

void ScoreManager::setDamageScore()
{
	//score += deathPoints;
	AddScore(deathPoints);
}

void ScoreManager::Update(float dt)
{
	levelTime += dt;
	//score += constPoints;
	AddScore(constPoints*dt);
}

void ScoreManager::SetPlayerSpeed(float speed)
{
	this->playerSpeed = speed;
}

void ScoreManager::SetDifficulty(Difficulity diff)
{
	this->levelDifficulty = diff;
}

void ScoreManager::SetStartPlatform(Platform*& platform)
{
	Platform* plat = platform;
	levelLength = 0.0f;
	while (plat->next != nullptr) {
		levelLength += (*plat->next->getPos() - *plat->getPos()).length();
		plat = plat->next;
	}
	levelLength;
}

void ScoreManager::SetSeed(int seed)
{
	this->seed = seed;
}

void ScoreManager::SetScore(float points)
{
	score = points;
}

void ScoreManager::AddScore(float points)
{
	score += points;
	HUD->UpdateScore(score);
}

void ScoreManager::LevelDone()
{
	//Add points
	float scoreMultiplyer = 1.0f;
	float puzzleTime = 10.0f;
	switch (levelDifficulty)
	{
	case Difficulity::medium:
		scoreMultiplyer = 1.5f;
		puzzleTime = 15.0f;
		break;
	case Difficulity::hard:
		scoreMultiplyer = 2.0f;
		puzzleTime = 20.0f;
		break;
	}
	float optimalTime = puzzleTime + (levelLength / playerSpeed);//Time it would take to go in a straight line (* some multiplyer)
	float scoreToGive = levelPoints * (optimalTime / levelTime);
	//score += (scoreToGive + puzzlePoints) * scoreMultiplyer;
	AddScore((scoreToGive + puzzlePoints) * scoreMultiplyer);
	//Reset timer
	levelTime = 0.0f;

	//return score;
}

void ScoreManager::Reset()
{
	this->levelDifficulty = Difficulity::easy;
	this->levelLength = 0.0f;
	this->levelTime = 0.0f;
	this->score = 0.0f;
	this->playerSpeed = 0.0f;
}

void ScoreManager::WriteScore(std::string name, std::string file)
{
	std::ifstream scoreFile;
	std::ofstream scoreFileWrite;
	std::string numScoresS;
	std::string tempScore;
	std::string tempName;
	std::string tempSeed;
	std::string newFile;
	int numScoresI;
	bool scoreInserted = false;
	bool nameExists = false;
	bool reWrite = false;

	std::vector<std::string>scores;
	std::vector<std::string>names;
	std::vector<std::string>seeds;
	scoreFile.open(file, std::ifstream::in);
	scoreFile >> numScoresS;
	if (numScoresS != "" && numScoresS != "0") {
		numScoresI = std::stoi(numScoresS);
		for (int i = 0; i < numScoresI; i++) {
			scoreFile >> tempScore;
			scoreFile >> tempName;
			scoreFile >> tempSeed;
			if (tempName == name) {
				nameExists = true;
				if (std::stoi(tempScore) < (int)score) {
					reWrite = true;
					tempScore = std::to_string((int)score);
				}
			}
			scores.push_back(tempScore);
			names.push_back(tempName);
			seeds.push_back(tempSeed);
		}
		if (nameExists) {
			if (reWrite) {
				newFile += std::to_string(numScoresI) + "\n";
				for (int i = 0; i < scores.size(); i++) {
					newFile += scores[i] + " " + names[i] + " " + seeds[i] + "\n";
				}
			}
		}
		else if (numScoresI < maxScores) {
			reWrite = true;
			newFile += std::to_string(numScoresI + 1) + "\n";
			std::string toInsert;
			for (int i = 0; i < scores.size(); i++) {
				if (std::stoi(scores[i]) < score && !scoreInserted) {
					toInsert = std::to_string((int)score) + " " + name + " " + std::to_string(seed) + "\n";
					toInsert += scores[i] + " " + names[i] + " " + seeds[i] + "\n";
					scoreInserted = true;
				}
				else {
					toInsert = scores[i] + " " + names[i] + " " + seeds[i] + "\n";
				}
				newFile += toInsert;
			}
			if (!scoreInserted) {
				newFile += std::to_string((int)score) + " " + name + " " + std::to_string(seed) + "\n";
			}
		}
		else {
			reWrite = true;
			newFile += numScoresS + "\n";
			for (int i = 0; i < scores.size(); i++) {
				if (std::stoi(scores[i]) < score && !scoreInserted) {
					for (int j = scores.size() - 1; j > i; j--) {
						scores[j] = scores[j - 1];
						names[j] = names[j - 1];
						seeds[j] = seeds[j - 1];
					}
					scores[i] = std::to_string((int)score);
					names[i] = name;
					seeds[i] = std::to_string(seed);
					scoreInserted = true;
				}
				newFile += scores[i] + " " + names[i] + " " + seeds[i] + "\n";
			}
		}
	}
	else {
		reWrite = true;
		newFile = "1\n" + std::to_string((int)score) + " " + name + " " + std::to_string(seed) +"\n";
	}
	scoreFile.close();
	if (reWrite) {
		scoreFileWrite.open(file, std::ofstream::out);
		scoreFileWrite << newFile;
		scoreFileWrite.close();
	}
	SortScores(file);
	Reset();
}

void ScoreManager::SortScores(std::string file)
{
	std::ifstream scoreFile;
	std::ofstream scoreFileWrite;
	std::string temp;
	std::string newFile;
	std::vector<std::string>scores;
	scoreFile.open(file, std::ifstream::in);
	scoreFile >> newFile;
	newFile += "\n";
	while (std::getline(scoreFile, temp)) {
		if(temp != "")
			scores.push_back(temp);
	}
	for (int i = 0; i < scores.size()-1; i++){
		for (int j = i+1; j < scores.size(); j++) {
			int n1 = std::stoi(scores[i].substr(0, scores[i].find(" ")));
			int n2 = std::stoi(scores[j].substr(0, scores[j].find(" ")));
			if (n1 < n2) {
				temp = scores[i];
				scores[i] = scores[j];
				scores[j] = temp;
			}
		}
	}
	for (int i = 0; i < scores.size(); i++) {
		newFile += scores[i]+"\n";
	}
	scoreFileWrite.open(file, std::ofstream::out);
	scoreFileWrite << newFile;
	scoreFileWrite.close();

}

void ScoreManager::SetHUD(Hud*& hud)
{
	this->HUD = hud;
}
