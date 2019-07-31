#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <cctype>

using namespace std;

void LoadFiveWordFile(vector <string> &fiveWords, ifstream &fiveWordsFile)
{

	for (string line; std::getline(fiveWordsFile, line);)
	{
		if (!line.empty())
			if(line.size() == 5)
				fiveWords.push_back(line);
	}
	fiveWordsFile.close();
}

bool ContainsWord(const vector <string> &list, const string& target)
{
	for (const string& word : list)
	{
		if (word == target)
		{
			return true;
		}
	}
	return false;
}

vector<string> FilterWords(vector<string>& fiveWords, vector<string>& commonWords)
{
	vector<string> filterWords;
	for (const string& word : fiveWords)
	{
		if (ContainsWord(commonWords, word))
		{
			filterWords.push_back(word);
		}
	}
	return filterWords;
}

const string GenerateTarget(vector <string> &fiveWords)
{
	mt19937 engine = mt19937(random_device()());
	uniform_int_distribution<int> dist(0, fiveWords.size() - 1);
	return fiveWords[dist(engine)];
}

int ScoreMatchWord(const string& target, string& guess)
{
	vector<int> matchedTarget(5, 0);
	vector<int> matchedGuess(5, 0);

	int score = 0;
	for (size_t i = 0; i < target.size(); i++)
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

	for (size_t i = 0; i < target.size(); i++)
	{
		if (matchedTarget[i] == 1)
			continue;
		for (size_t j = 0; j < guess.size(); j++)
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
	vector<string> commonWords;
	ifstream fiveWordsFile("sgb-words.txt");
	ifstream commonWordsFile("20k.txt");

	LoadFiveWordFile(fiveWords, fiveWordsFile);
	LoadFiveWordFile(commonWords, commonWordsFile);

	commonWords.resize(2000); //set difficulty

	vector<string> filterWords = FilterWords(fiveWords, commonWords);

	const string target = GenerateTarget(filterWords);
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