#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "sanitize.hpp"
#include <stdlib.h>

using namespace std;

class TextGen {
private:
    map<string, vector<string>> markov_chain; //map usage is mandatory!

public:
	TextGen(const unsigned int seed);
	void readIn(const string & filename);
	bool isEndPunctuation(const char character);
	vector<string> getValues(const string & key);
	string generateSentence();
	map<string, vector<string>> get_markovChain();
};

//TextGen's constructor has been created for you
TextGen::TextGen(const unsigned int seed)
{
	srand(seed);//seeds our random number generator
}

//Create the rest of the functions below...
bool TextGen::isEndPunctuation(char punctuation){
    if (punctuation == '!' || punctuation == '.' || punctuation == '?') {
      return true;
    } else {
      return false;
    }
}

void TextGen::readIn(const string& input){
  ifstream file(input);
  if(!file.is_open()){
    return;
  }

  string previous = "^";
  string current = "";

  while (file >> current) {
    sanitize(current);
    if (current.length() == 0) {
      continue;
    }
    string last_letter = current.substr(current.length() - 1, current.length());

    if (isEndPunctuation(last_letter[0])) {
      current = current.erase(current.length() - 1);
      markov_chain[previous].push_back(current);
      markov_chain[current].push_back(last_letter);
      markov_chain[last_letter].push_back("$");
      previous = "^";

    } else {
      markov_chain[previous].push_back(current);
      previous = current;
    }
  }
}

vector<string> TextGen::getValues(const string& value){
  return markov_chain[value];
}

string TextGen::generateSentence(){
  string sentence = "";
  string generated = "";

  int size = rand() % markov_chain["^"].size();
  generated = markov_chain["^"].at(size);
  sentence = generated;

  while (generated != "$") {
    size = rand() % markov_chain[generated].size();
    generated = markov_chain[generated].at(size);
      if (generated == "$") {
        string punctuation(1, sentence[sentence.size() - 1]);
        sentence = sentence.substr(0, sentence.size() - 2);
        sentence += punctuation;
      } else {
        sentence += " " + generated;
      }
  }
  return sentence;
}

map<string, vector<string>> TextGen::get_markovChain(){
  return markov_chain;
}
