#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

int main()
{
	/*
	TO DO

	Does not read in the files as command line arguments: reads them in via ifsteam objects

	Does not account for words not found in the 'dictionaries': current implemention appends too many
	question marks when faced with unknown words
	*/

	// Opening all of the files to be used
	ifstream english_to_spanish;
	english_to_spanish.open("english_spanish.txt");
	ifstream spanish_to_polish;
	spanish_to_polish.open("spanish_polish.txt");
	ifstream translation_file;
	translation_file.open("translation_file.txt");
	// Buffer for when we want to start reading these files
	string buffer;

	// Vectors where the english->spanish diction and spanish->polish dictionary will be kept
	// plus the final translation
	vector<string> english_spanish;
	vector<string> spanish_polish;
	vector<string> finished_translation;

	// Make sure the files are opened
	if (english_to_spanish.is_open())
	{
		cout << "File 'english_to_spanish.txt' opened." << endl;
	}
	else { cout << "File not opened."; exit(1); }
	if (spanish_to_polish.is_open())
	{
		cout << "File 'spanish_to_polish.txt' opened." << endl;
	}
	else { cout << "File not opened."; exit(1); }
	if (translation_file.is_open())
	{
		cout << "File 'translation_file.txt' opened." << endl;
	}
	else { cout << "File not opened."; exit(1); }

	// Read the first file and store the contents in a vector
	while (getline(english_to_spanish, buffer))
	{
		stringstream line_stream(buffer);
		string word;
		// Reading the lines and delimiting on the =
		while (getline(line_stream, word, '='))
		{
			english_spanish.push_back(word);
		}
	}

	while (getline(spanish_to_polish, buffer))
	{
		stringstream line_stream(buffer);
		string word;
		while (getline(line_stream, word, '='))
		{
			spanish_polish.push_back(word);
		}
	}

	// Reads in the file to be translated
	while (getline(translation_file, buffer))
	{
		stringstream line_stream(buffer);
		string word;
		string translated_word;
		// Delimiting on spaces - does not account for punctuation
		while (getline(line_stream, word, ' '))
		{
			for (int i = 0; i < english_spanish.size() - 1; i++)
			{
				// if the word in the translation file is found in the english to spanish vector,
				// the ith word is the english word and the ith + 1 is the spanish equivalent
				if (word == english_spanish[i])
				{
					word = english_spanish[i + 1];
					// the same logic applies here for the spanish to polish translations as above
					for (int j = 0; j < spanish_polish.size() - 1; j++)
					{
						if (word == spanish_polish[j])
						{
							// if we find the word we want to translated, the j + 1 word is the finished translation
							// we can then append that word to our finished translation
							translated_word = spanish_polish[j + 1];
							finished_translation.push_back(translated_word);
						}
					}
				}
			}
		}
	}
	
	// Prints the final translation
	for (int i = 0; i < finished_translation.size(); i++)
	{
		cout << finished_translation[i] << " ";
	}

}