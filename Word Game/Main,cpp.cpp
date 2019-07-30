#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <cctype>

using namespace std;

void LoadFile(vector <string> &fiveWords, ifstream &fiveWordsFile)
{

	for (string line; std::getline(fiveWordsFile, line);)
	{
		if (!line.empty())
			fiveWords.push_back(line);
	}
	fiveWordsFile.close();
}

const string GenerateTarget(vector <string> &fiveWords)
{
	mt19937 engine = mt19937(random_device()());
	uniform_int_distribution<int> dist(0, fiveWords.size() - 1);
	return fiveWords[dist(engine)];
}

bool ContainsWord(vector <string> &fiveWords, string& guess)
{
	for (const string& word : fiveWords)
	{
		if (word == guess)
		{
			return true;
		}
	}
	return false;
}

int ScoreMatchWord(const string& target, string& guess)
{
	vector<int> matchedTarget(5, 0);
	vector<int> matchedGuess(5, 0);

	int score = 0;
	for (int i = 0; i < target.size(); i++)
	{
		if (target[i] == guess[i])
		{
			score += 2;
			matchedTarget[i] = 1;
			matchedGuess[i] = 1;
		}
	}

	if (score == 10)
		return score;

	for (int i = 0; i < target.size(); i++)
	{
		if (matchedTarget[i] == 1)
			continue;
		for (int j = 0; j < guess.size(); j++)
		{
			if (matchedGuess[j] == 1)
				continue;
			if (target[i] == guess[j])
			{
				score += 1;
				matchedTarget[i] = 1;
				matchedGuess[j] = 1;
				break;
			}
		}
	}
	return score;
}

int main()
{
	vector<string> fiveWords;
	ifstream fiveWordsFile("sgb-words.txt");

	LoadFile(fiveWords, fiveWordsFile);
	const string target = GenerateTarget(fiveWords);
	target.size();
	while (true)
	{
		cout << "Guess a five letter word: ";
		string guess;
		getline(cin, guess);

		for (char& c : guess)
		{
			c = tolower(c);
		}

		if (guess.size() != 5)
		{
			cout << "Enter a FIVE letter word!" << endl;
			continue;
		}

		if (!ContainsWord(fiveWords, guess))
		{
			cout << "That isn't a word!" << endl;
			continue;
		}

		int score = ScoreMatchWord(target, guess);

		if (score == 10)
		{
			cout << "You did it. Wow!" << endl;
			break;
		}
		else
		{
			cout << "You get " << score << " points." << endl;
		}
	}

	cin.clear();
	cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
	cin.get();

	return 0;
}