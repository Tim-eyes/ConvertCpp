#ifndef _SOURCE_H_
#define _SOURCE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class source
{
public:
	source();
	source(string inName,string outName);
	~source();
	
	void openKeywordFile();
	void openDirectivesFile();
	void readFile();
	void initSensitive();
	void sensitiveReplace();
	void colorComment();
	void colorConstantString();
	void colorDirectives();
	void colorKeywordsAndDirectives();
	void output();

private:

	string inputFileName;
	string outputFileName;
	fstream infile;
	ofstream outfile;

	vector<string> keywords;
	vector<string> directives;
	vector<string> mySource;
	map<string, string> sensitive;
	int fileLine;


	
};



#endif // !_SOURCE_H_

