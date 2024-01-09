#include <map>
#include <list>
#include <string>
#include<cmath>
#include<ctime>
#include<numeric>
#include<time.h>
#include <set>
#include <stack>
#include <iostream>
#include <algorithm>
#include <parameters.hpp>

extern const int gridSize;

class WaveFunction {

private:
	int size;
	std::set<std::string> coefficients[parameters::gridSize][parameters::gridSize]; //Donne pour chaque case les briques possibles
	std::set<std::string> tiles; //La liste des briques �l�mentaires
	std::map<std::string, int> numbers;
	std::map<const std::string, int> weights;  //Pour chaque brique, donne son poids

public:
	WaveFunction() {}

	WaveFunction(int initSize, std::map<const std::string, int> weightsList, std::set<std::string> tileList) {
		size = initSize;
		tiles = tileList;
		weights = weightsList;
		initNumbers();
		initCoefficients();
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}

	void print() {
		for (int y = size - 1; y >= 0; y--) {
			for (int x = 0; x < size; x++) {
				for (std::string tile : coefficients[x][y]) {
					std::cout << tile;
				}

			}
			std::cout << std::endl;
		}
	}

	void initNumbers() {
		int i = 0;
		for (std::string tile : tiles) {
			numbers.insert(std::pair<std::string, int>(tile, i));
			i++;
		}
	}

	void initCoefficients() { // 
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				coefficients[i][j] = tiles;  // Au d�but, toutes les briques sont possibles
			}
		}
	}

	std::set<std::string> get(int x, int y) {
		return coefficients[x][y];
	}

	int getNumber(std::string tile) {
		return numbers[tile];
	}

	float shannonEntropy(int x, int y) {
		int sumWeights = 0;
		float sumWeightLogWeight = 0;
		for (std::string option : coefficients[x][y]) {
			int weight = weights.at(option);
			//std::cout << x << " , " << y << " : ";
			//std::cout << weight << std::endl;
			sumWeights += weight;
			sumWeightLogWeight += weight * log(weight);
		}
		return log(sumWeights) - (sumWeightLogWeight / sumWeights);
	}

	bool isFullyCollapsed() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (coefficients[i][j].size() > 1) { return false; }
			}
		}
		return true;
	}

	std::map<const std::pair<int, int>, std::string> getAllCollapsed() {
		std::map<const std::pair<int, int>, std::string> result;
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				std::string tile = *coefficients[x][y].begin();
				//std::cout << "Only tile left : " << tile << std::endl;
				std::pair<int, int> coords = std::make_pair(x, y);
				result.insert(std::pair<std::pair<int, int>, std::string>(coords, tile));
			}
		}
		return result;
	}

	void collapse(int x, int y) {
		//std::cout << x << " , " << y << std::endl;
		std::set<std::string> options = coefficients[x][y];
		std::map<const std::string, int> validWeights;
		for (std::string tile : options) {
			validWeights.insert(std::pair<std::string, int>(tile, weights.at(tile)));
		}
		//std::cout << "validWeights OK" << std::endl;
		int totalWeights = std::accumulate(validWeights.begin(), validWeights.end(), 0, [](const int prev_sum, const std::pair<std::string, int>& entry) {
			return prev_sum + entry.second;
			});
		//std::cout << "totalWeights OK" << std::endl;
		//std::srand(std::time(nullptr));
		float rnd = (float)rand() / RAND_MAX;
		//std::cout << "rnd = " << rnd << std::endl;
		float randTotalWeight = rnd * totalWeights;
		std::string chosen;
		std::map<const std::string, int>::iterator it = validWeights.begin();
		for (std::pair<const std::string, int> element : validWeights) {
			if (randTotalWeight > 0) {
				std::string tile = element.first;
				int weight = element.second;
				randTotalWeight -= weight;
				if (randTotalWeight < 0) {
					chosen = tile;
				}
				//std::cout << "Remaining weight : " << randTotalWeight << std::endl;
			}
		}
		//std::cout << "Chosen tile is : " << chosen << std::endl;
		coefficients[x][y] = { chosen };
	}

	void remove(int x, int y, std::string forbiddenTile) {
		std::set<std::string>::iterator it = coefficients[x][y].find(forbiddenTile);
		coefficients[x][y].erase(it);
	}
};

class CompatibilityTester {

private:
	std::set<std::string> data[parameters::nTiles][parameters::nTiles]; //Un tableau qui � chaque couple ordonn� de briques �l�mentaires associe un set des directions possibles (si d est dans data[i][j] alors j peut �tre place dans la directiond par rapport a i)
	std::map<std::string, int> numbers;

public:
	CompatibilityTester(std::set<std::string> dataInit[parameters::nTiles][parameters::nTiles], std::map<std::string, int> numbersInit) {
		for (int i = 0; i < parameters::nTiles; i++) {
			for (int j = 0; j < parameters::nTiles; j++) {
				data[i][j] = dataInit[i][j];
			}
		}
		numbers = numbersInit;

	}

	int getNumber(std::string tile) {
		return numbers[tile];
	}

	bool check(std::string tile1, std::string tile2, std::string direction) {
		int numTile1 = getNumber(tile1);
		int numTile2 = getNumber(tile2);
		std::set<std::string> directions = data[numTile1][numTile2];
		/*for (std::string dir : directions) {
			std::cout << dir << std::endl;
		}
		std::cout << "check done" << std::endl; */
		return directions.count(direction) != 0;
	}
};

class Propagator {

private:
	int outputSize;
	WaveFunction waveFunction;
	CompatibilityTester compatibilityTester;
	std::set<std::string> directions = { "up", "down", "left", "right" };
	std::map<const std::string, std::pair<int, int>> directionCoord;

public:
	Propagator(int outputSizeInit, std::map<const std::string, int> weights, std::set<std::string> tiles, CompatibilityTester compatibilityTesterInit) : compatibilityTester(compatibilityTesterInit) {
		outputSize = outputSizeInit;
		waveFunction = WaveFunction(outputSize, weights, tiles);
		//waveFunction.print();
		std::pair<int, int> up = std::make_pair(0, 1);
		std::pair<int, int> down = std::make_pair(0, -1);
		std::pair<int, int> left = std::make_pair(-1, 0);
		std::pair<int, int> right = std::make_pair(1, 0);
		directionCoord.insert(std::pair<std::string, std::pair<int, int>>("up", up));
		directionCoord.insert(std::pair<std::string, std::pair<int, int>>("down", down));
		directionCoord.insert(std::pair<std::string, std::pair<int, int>>("left", left));
		directionCoord.insert(std::pair<std::string, std::pair<int, int>>("right", right));
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}

	void printResult(std::map<const std::pair<int, int>, std::string> result) {
		std::map<const std::pair<int, int>, std::string>::iterator it = result.begin();
		int resultSize = result.size();
		//std::cout << resultSize << std::endl;
		//if (resultSize < 100) {
		std::for_each(result.begin(), result.end(), [](std::pair<const std::pair<int, int>, std::string> element) {
			std::pair<int, int> coords = element.first;
			std::string tile = element.second;
			std::cout << coords.first << ", " << coords.second << " : " << tile << std::endl;
			});
		//}
	}

	std::map<const std::pair<int, int>, std::string> run() {
		int i = 0;
		while (!(waveFunction.isFullyCollapsed()) && i < outputSize * outputSize) {
			//std::cout << "I'm iterating !" << std::endl;
			iterate();
			i++;
		}
		std::cout << "Result : " << std::endl;
		waveFunction.print();
		return waveFunction.getAllCollapsed();
	}



	void iterate() {
		//std::cout << "Start iteration !" << std::endl;
		std::pair<int, int> coords = minEntropyCoordinates();
		//std::cout << "Min entropy coords calculated" << std::endl;
		//std::cout << coords.first << " , " << coords.second << std::endl;
		waveFunction.collapse(coords.first, coords.second);
		//std::cout << "It's collapsed !" << std::endl;
		propagate(coords.first, coords.second);
		//std::cout << "It's propagated !" << std::endl;
	}

	void propagate(int x, int y) {
		std::stack<std::pair<int, int>> stack;
		std::pair<int, int> coords = std::make_pair(x, y);
		stack.push(coords);
		while (stack.size() > 0) {
			std::pair<int, int> currentCoord = stack.top();
			stack.pop();
			//std::cout << "Current coord : " << currentCoord.first << " , " << currentCoord.second << std::endl;
			std::set<std::string> currentPossTiles = waveFunction.get(currentCoord.first, currentCoord.second);
			std::map<const std::string, bool> validDir = validDirections(currentCoord, outputSize);
			//printValidDirs(validDir);
			for (std::string d : directions) {
				if (validDir[d]) {
					std::pair<int, int> otherCoord = { (currentCoord.first + directionCoord[d].first), (currentCoord.second + directionCoord[d].second) };
					//std::cout << "Other coord : " << otherCoord.first << " , " << otherCoord.second << std::endl;
					for (std::string otherTile : waveFunction.get(otherCoord.first, otherCoord.second)) {
						bool otherTilePossible = false;
						for (std::string currentTile : currentPossTiles) {
							//std::cout << currentTile << " , ";
							otherTilePossible = otherTilePossible || compatibilityTester.check(currentTile, otherTile, d);
						}
						if (!otherTilePossible) {
							//std::cout << otherTile << "not possible" << std::endl;
							waveFunction.remove(otherCoord.first, otherCoord.second, otherTile);
							stack.push(otherCoord);
						}
					}
				}
			}
		}
		//waveFunction.print();
	}

	std::pair<int, int> minEntropyCoordinates() {
		float minEntropy = 100;
		std::pair<int, int> minEntropyCoords;
		for (int x = 0; x < outputSize; x++) {
			for (int y = 0; y < outputSize; y++) {
				if ((waveFunction.get(x, y)).size() == 1) {
					continue;
				}
				float entropy = waveFunction.shannonEntropy(x, y);
				//std::srand(static_cast<unsigned int>(std::time(nullptr)));
				float rnd = (float)rand() / RAND_MAX;
				float entropyWithNoise = entropy + rnd / 1000;
				//std::cout << x << " , " << y << " : ";
				//std::cout << entropyWithNoise << std::endl;
				if (entropyWithNoise < minEntropy) {
					minEntropy = entropyWithNoise;
					//std::cout << x << " , " << y << " : " << "Min entropy so far" << minEntropy << std::endl;
					minEntropyCoords = std::make_pair(x, y);
				}
			}
		}
		return minEntropyCoords;
	}

	std::map<const std::string, bool> validDirections(std::pair<int, int> coords, int size) {
		int x = coords.first;
		int y = coords.second;
		std::map<const std::string, bool> dir;
		if (y > 0) { dir.emplace("down", true); }
		else { dir.emplace("down", false); }
		if (y < outputSize - 1) { dir.emplace("up", true); }
		else { dir.emplace("up", false); }
		if (x > 0) { dir.emplace("left", true); }
		else { dir.emplace("left", false); }
		if (x < outputSize - 1) { dir.emplace("right", true); }
		else { dir.emplace("right", false); }

		return dir;
	}
	void printValidDirs(std::map<const std::string, bool> validDirs) {
		std::cout << "The valid directions are : ";
		std::map<const std::string, bool>::iterator it = validDirs.begin();
		for (std::pair<const std::string, bool> element : validDirs) {
			if (element.second) {
				std::cout << element.first << ", ";
			}
		}
		std::cout << std::endl;
	}

};