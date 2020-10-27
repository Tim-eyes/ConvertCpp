#include "source.h"

int main() {
    //char ch;
  
    //do
    //{
    //    string infileName;
    //    string outfileName;
    //    cout << "Please input the name of input file" << endl;
    //    cin >> infileName;
    //    cout << "Please input the name of output file" << endl;
    //    cin >> outfileName;
    //    
    //    
    //    source myS(infileName,outfileName);
    //    //myS.
    //    
    //    cout << "If you want to try again, please input \'Y\',otherwise input any character else" << endl;
    //    cin >> ch;
    //} while (ch == 'Y');
	
    source s("c.cpp", "h1.html");
   
    s.openKeywordFile();
    s.openDirectivesFile();
    s.readFile();
   
    s.initSensitive();
    s.sensitiveReplace();
    s.colorComment();
    
     
    s.colorConstantString();
    //s.colorDirectives();
    s.colorKeywords(); 
   
    s.output();

	return 0;
}