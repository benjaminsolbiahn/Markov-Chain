#include <iostream>
#include <string>
#include <cmath>    // abs()
#include <stdlib.h>

#include "TextGen.hpp"
using namespace std;

// Global static variables
static int testCount = 0;
static const int testTotal = 11;
static const string GREEN = "\033[32m";
static const string RED = "\033[31m";
static const string RESET = "\033[0m";

// Prototypes
bool assertTrue(const string& nameOfTest, bool actual);
bool assertValidSentence(const string & nameOfTest, const string & actual);

template <typename T, typename U>
bool assertVectorEquals(const string& nameOfTest, const vector<T>& expected, const vector<U>& actual);

map<string, vector<string>> open_markov_chain(const string&);
float markov_chain_equivalence(map<string, vector<string>>, map<string, vector<string>>);

// Main
int main(int argc, char const *argv[]) {
	TextGen gen(92870);//seed = 92870
	assertTrue("Punctuation 01", gen.isEndPunctuation('.'));
	assertTrue("Punctuation 02", gen.isEndPunctuation('?'));
	assertTrue("Punctuation 03", gen.isEndPunctuation('!'));

	TextGen Frankenstein(31);//seed = 31
	Frankenstein.readIn("Frankenstein or The Modern Prometheus by Mary Shelley.txt");
	TextGen SleepyHollow(11712);//seed = 11712
	SleepyHollow.readIn("The Legend of Sleep Hollow by Washington Irving.txt");

	{
		vector<string> expected = { "lovers,", "days", "lovers" };
		assertVectorEquals("Values : Frankenstein 'youthful'", expected, Frankenstein.getValues("youthful"));
		assertTrue("Value Count : Frankenstein '^'", Frankenstein.getValues("^").size() > 10);
	}
	{
		vector<string> expected = { "hill,","crests", "dell," };
		assertVectorEquals("Values : SleepyHollow", expected, SleepyHollow.getValues("woody"));
		assertTrue("Value Count : SleepyHollow '^'", Frankenstein.getValues("^").size() > 10);
	}

	//Is your generateSentence() function creating syntactically correct sentences?
	assertValidSentence("GenerateSentence : Frankenstein", Frankenstein.generateSentence());
	assertValidSentence("GenerateSentence : Sleepy Hollow", SleepyHollow.generateSentence());

	/*
		The following "Percentage of Equivalence" tests aim to measure how accurately
		you are forming your Markov Chain. Carefully read the project description to
		be sure you are doing so properly.
	*/
	{//Frankenstein Percentage of Equivalence test
		map<string, vector<string>> ans_key = open_markov_chain("markov_chain_answer_key_frankenstein.txt");
		cout << "\nFrankenstein PERCENTAGE OF EQUIVALENCE: " <<
			markov_chain_equivalence(ans_key, Frankenstein.get_markovChain()) << endl;
	}
	{//Sleepy Hollow Percentage of Equivalence test
		map<string, vector<string>> ans_key = open_markov_chain("markov_chain_answer_key_sleepyhollow.txt");
		cout << "\nSleepyHollow PERCENTAGE OF EQUIVALENCE: " <<
			markov_chain_equivalence(ans_key, SleepyHollow.get_markovChain()) << endl;
	}

	cout << endl << testCount << " tests passed out of " << testTotal << " total tests" << endl
		<< 110.0 * (float)testCount / (float)testTotal << "/110" << endl;

	cin.get();
	return 0;
}

// Helper Functions
bool assertTrue(const string& nameOfTest, bool actual) {
	if (actual == true) {
		// Green colored text
		cout << GREEN << "PASSED "
			<< RESET << nameOfTest << ": test successfully resulted in 'true'" << endl;
		testCount++;
		return true;
	}
	// Red colored text
	cout << RED << "FAILED "
		<< RESET << nameOfTest << ": test unsuccessfully resulted in 'false'" << endl;
	return false;
}

bool assertValidSentence(const string & nameOfTest, const string & actual) {
	char lastChar = actual[actual.size() - 1];
	if (isupper(actual[0]) && (lastChar == '.' || lastChar == '?' || lastChar == '!')) {
		for (int i = 1; i < actual.size() - 1; ++i) {
			if (actual[i] == ' ') {
				// Green colored text
				cout << GREEN << "PASSED "
					<< RESET << nameOfTest << ": valid sentence '" << actual << "'" << endl;
				testCount++;
				return true;
			}
		}
	}

	// Red colored text
	cout << RED << "FAILED "
		<< RESET << nameOfTest << ": invalid sentence '" << actual << "'" << endl;
	return false;
}

template <typename T, typename U>
bool assertVectorEquals(const string& nameOfTest, const vector<T>& expected, const vector<U>& actual) {
	if (expected.size() == actual.size()) {
		for (size_t i = 0; i < expected.size(); i++) {
			if (expected[i] != actual[i]) {
				// Red colored text
				cout << RED << "FAILED "
					<< RESET << nameOfTest << ": expected '" << expected[i] << "' but actually '" << actual[i] << "'" << endl;
				return false;
			}
		}

		// Green colored text
		cout << GREEN << "PASSED "
			<< RESET << nameOfTest << ": expected and actual lists match: {";
		for (int i = 0; i < expected.size(); i++) {
			cout << " " << expected[i];
		}
		cout << " }" << endl;

		testCount++;
		return true;
	}

	// Red colored text
	cout << RED << "FAILED "
		<< RESET << nameOfTest << ": expected size '" << expected.size() << "' but actually size is '" << actual.size() << "'" << endl;
	return false;
}


map<string, vector<string>> open_markov_chain(const string& filename)
{
	map<string, vector<string>> ret;
	ifstream _in;
	_in.open(filename);
	string k = "", v = "";
	while (_in)
	{
		if (k.empty()) _in >> k >> v;
		else
		{
			_in >> v;
			if (v == "ENDL") _in >> k >> v;
		}
		if (v != "ENDL") ret[k].push_back(v);
	}
	return ret;
}

float markov_chain_equivalence(map<string, vector<string>> ans, map<string, vector<string>> supplied)
{
	int size = ans.size();
	int running_total = size;
	int diff = ans.size() - supplied.size(); running_total -= abs(diff);//take some accuracy off if the sizes are different
	for (auto it = ans.begin(); it != ans.end(); ++it)
		if (supplied[it->first] != ans[it->first]) running_total--;

	float percentage = (float)running_total / (float)size;
	if (percentage > .6)
	{
		cout << GREEN << "PASSED "
			<< RESET << "Markov Chain is at least 60% equivalent.";
		testCount++;
	}
	else
		cout << RED << "FAILED "
		<< RESET << "Markov Chain is NOT at least 60% equivalent.";
	return percentage;
}
