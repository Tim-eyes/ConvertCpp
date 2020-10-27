#include "source.h"

using namespace std;

source::source()
{
}

source::source(string inName, string outName)
    :inputFileName(inName), outputFileName(outName) {

    infile.open(inputFileName);
    if (infile.fail()) {
        cout << "input file open fail!" << endl;
        exit(1);
    }

    outfile.open(outputFileName);
    if (outfile.fail()) {
        cout << "output file open fail!" << endl;
        exit(1);
    }

}

source::~source()
{
    infile.close();
    outfile.close();
    mySource.clear();
}

void source::openKeywordFile()
{
    ifstream keywordFile("key.txt");
    string k;
    while (keywordFile >> k) {
        keywords.push_back(k);
    }
    keywordFile.close();
}

void source::openDirectivesFile()
{
    ifstream directivesFile("directives2.txt");
    string d;
    while (directivesFile >> d) {
        directives.push_back(d);
    }
    directivesFile.close();
}
    
void source::readFile() 
{
    string temp;
    while (!infile.eof()) {
        getline(infile, temp);
        mySource.push_back(temp);
       
    }
    fileLine = static_cast<int>(mySource.size());
}

void source::initSensitive() {
    sensitive.insert(make_pair(" ", "&nbsp;"));
    sensitive.insert(make_pair("\t", "&nbsp;&nbsp;&nbsp;&nbsp;"));
    sensitive.insert(make_pair("<", "&lt;"));
    sensitive.insert(make_pair(">", "&gt;"));
}

void source::sensitiveReplace()
{
    for (vector<string>::iterator s = mySource.begin(); s != mySource.end(); ++s) {
        for (map<string, string>::iterator sm = sensitive.begin(); sm != sensitive.end(); ++sm) {
            string::size_type pos = 0;
            while ((pos = (*s).find(sm->first, pos)) != string::npos) {
                (*s).replace(pos, (sm->first).size(), sm->second);

                pos += (sm->second).size();
            }
        }
    }
}

void source::colorComment()
{
    string quto = "\"";
    string commentLine = "//";
    pair<string, string> commentBlock("/*", "*/");
    pair<string, string> commentReplace("<span style=\"color:#00FF00\">", "</span>");
    bool isCommentBlockOver = true ;

    for (vector<string>::iterator s = mySource.begin(); s != mySource.end(); ++s) {
        string::size_type pos = 0;
        string::size_type posBlock = 0;
        string::size_type posLine = 0;
        string::size_type posQuot = 0;
        
        while (true) {
            if (isCommentBlockOver) {
                posBlock = (*s).find(commentBlock.first, pos);
                posLine = (*s).find(commentLine, pos);
                posQuot = (*s).find(quto, pos);
                
                //
                if ((posQuot < posLine) && (posQuot < posBlock) && (posQuot != string::npos)) {
                    pos = posQuot + quto.size();
                    while ((pos = (*s).find(quto, pos)) != string::npos /*&&*/)
                    {
                        pos += quto.size();
                    }
                    continue;
                }
                else if ((posLine < posBlock) && (posLine != string::npos)) {
                    pos = posLine;
                    (*s).replace(pos, commentLine.size(), commentReplace.first+commentLine);
                    (*s) += commentReplace.second;
                    //break;
                }

                else if ((posBlock < posLine) && (posBlock != string::npos)) {
                    isCommentBlockOver = false;
                    pos = posBlock;
                    (*s).replace(pos, commentBlock.first.size(), commentReplace.first + commentBlock.first);
                    //(*s).replace(pos, commentBlock.second.size(), commentReplace.second);
                    pos += (commentReplace.first + commentBlock.first).size();

                }

                else  
                    break;

            }
            if (!isCommentBlockOver) {
                if ((pos = (*s).find(commentBlock.second, pos)) != string::npos) {
                    isCommentBlockOver = true;
                    //pos = posBlock;
                    (*s).replace(pos, commentBlock.second.size(),commentBlock.second + commentReplace.second);
                    pos += (commentReplace.second + commentBlock.second).size();
                    
                }
                else {
                    (*s).insert(0, commentReplace.first);
                    (*s) += commentReplace.second;
                    break;
                }

            }

        }
    }
}

void source::colorConstantString()
{   
    string quto = "\"";
    string head = "<span";
    string tail = "</span>";
    pair<string, string> constantStringRepalce("<span style=\"color:#FF0000\">", "</span>");
    bool isComment = true;

    for (vector<string>::iterator s = mySource.begin(); s != mySource.end(); ++s) {
        string::size_type pos1 = 0;
        string::size_type pos2 = 0;
        string::size_type pos = 0;
        
        while (true) {
            if ((pos=(*s).find(head,pos1))!=string::npos) {
                pos2 = pos;
            }
            else{
                isComment = false;
                pos2 = static_cast<string::size_type>((*s).size());
            }    
            
          
            bool isOver = true;
            string::size_type posTemp = pos1; 
            while (true) {
           
                if (isOver) {
                    if (((posTemp = (*s).find(quto, posTemp)) != string::npos) && (posTemp < pos2)) {
                        isOver = false;
                        (*s).replace(posTemp, quto.size(), constantStringRepalce.first + quto);
                        posTemp += (constantStringRepalce.first + quto).size();
                        pos2 += (constantStringRepalce.first + quto).size();
                    }
                    else 
                        break;
                   
                }
                if (!isOver) {
                    while (((posTemp = (*s).find(quto, posTemp)) != string::npos) && (posTemp < pos2)) {
                       
                        isOver = true;
                        (*s).replace(posTemp, quto.size(), quto + constantStringRepalce.second);
                        posTemp += (constantStringRepalce.second + quto).size();
                        pos2 += (constantStringRepalce.second + quto).size();
                        
                    }
                }

            }

            if (isComment) {
                pos1 = (*s).find(tail, pos1);
                pos1 = static_cast<string::size_type>(tail.size());
            }
            else
                break;
        }
    }
}


void source::colorKeywordsAndDirectives()
{   
    string head = "<span";
    string tail = "</span>";
    pair<string, string> keywordsReplace("<span style=\"color:#0000FF\">", "</span>");
    pair<string, string> directivesReplace("<span style=\"color:#FF0000\">", "</span>");
    bool isComment = true;

    for (vector<string>::iterator s = mySource.begin(); s != mySource.end(); ++s) {
        string::size_type pos1 = 0;
        string::size_type pos2 = 0;
        string::size_type pos = 0;

        while (true) {
            if ((pos = (*s).find(head, pos1)) != string::npos) {
                pos2 = pos;
            }
            else {
                isComment = false;
                pos2 = static_cast<string::size_type>((*s).size());
            }

            for (vector<string>::iterator k = keywords.begin(); k != keywords.end(); ++k) {
                string::size_type posK = pos1;

                 while(((posK=(*s).find(*k,posK))!=string::npos) && (posK<pos2)) {
                     if (posK != 0) {

                     }
                     (*s).replace(posK, (*k).size(), keywordsReplace.first + (*k) + keywordsReplace.second);
                     posK += (keywordsReplace.first + (*k) + keywordsReplace.second).size();
                     pos2 += (keywordsReplace.first + (*k) + keywordsReplace.second).size();

                 }
            }

            for (vector<string>::iterator d = directives.begin(); d != directives.end(); ++d) {
                string::size_type posD = pos1;
                while (((posD = (*s).find((*d), posD)) != string::npos) && (posD < pos2)) {
                    (*s).replace(posD, (*d).size(), directivesReplace.first + (*d) + directivesReplace.second);
                    posD += (directivesReplace.first + (*d) + directivesReplace.second).size();
                    pos2 += (directivesReplace.first + (*d) + directivesReplace.second).size();
                }


            }


            if (isComment) {
                pos1 = (*s).find(tail, pos1);
                pos1 = static_cast<string::size_type>(tail.size());
            }
            else
                break;
        }
    }
}

void source::output() {
    outfile << "<div style=\"padding:0;font-size:20px;\">";

        outfile << "<div style=\"padding:0;margin:0;float:left;color:rgba(128, 128, 128, 1);"
            << "border-right:1px solid #888000;background-color:rgba(224, 224, 224, 1);\">";
        for (int i = 0; i != fileLine; i++) {
            outfile << "&nbsp;" << i + 1 << "&nbsp;<br />";
        }
        outfile << "</div>";

        outfile << "<div style=\"padding:0;float:left;padding-left:5px;\">";
        for (vector<string>::iterator s = mySource.begin(); s != mySource.end(); ++s) {
            outfile << (*s) << "<br />";
        }
        outfile << "</div>";
        outfile << "<div style=\"clear:both;font-size:0px;line-height:0px;\">  </div>";

    outfile << "</div>";
    outfile << "<div style=\"clear:left;\">&nbsp;</div>";
   
}