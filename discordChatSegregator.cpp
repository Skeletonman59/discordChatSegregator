/*
 * This code is designed to process files with a format mentioned in: https://www.youtube.com/watch?v=eoM2-s3HxPc
 * To utilize this code, place the file given from extracting the discord DM in the same directory as this.
 * 
 * If any username contains non-ascii characters, please find and replace (Ctrl+H) all instances to
 * something simple and uncommon.
 *
 * This is all very basic code of course, nothing major. Just a little side project I wanted to work on.
 * Some code snippets, like the functions, were taken from a previous assignment related to markov chaining.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool stripEmbedAttachments(string &s, bool &paragraph) {
    if(s == "{Embed}" || s == "{Attachments}"|| s == "{Reactions}") paragraph = true;
    else if (paragraph && s[0] == '[') paragraph = false; //check if the string starts with '[', aka the end of embed
    else if (paragraph) return paragraph;
    return paragraph;
}
bool stripUser(string &s, const string &user, const string &clone, bool &flip) {
    if(s == user) flip = true;
    else if (flip && s == clone) flip = false;
    else if (flip) return flip;
    return flip;
}
void stripClone(string &s, const string &clone) {
    if(s == clone) s = "";
}
void stripLinks(string &s) {
    size_t found = s.find("http:");
    if (found != string::npos) s = "";
    found = s.find("https:");
    if(found != string::npos) s = "";
}
void stripBrackets(string &s) {
    //note: this is terrible if some messages actually contain bracketed messages
    string retval;
    bool deleting = false;
    bool bracketed = false;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '[') {
            deleting = true;
            bracketed = true;
        }
        else if (s[i] == ']') deleting = false;
        else if (!deleting) retval.push_back(s[i]);
    }
    s = retval;
    if (bracketed) s.erase(0,1);
}
void stripEmotes(string &s) {
    string retval;
    bool deleting = false;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == ':') deleting = true;
        else if (s[i] == ':') deleting = false;
        else if (!deleting) retval.push_back(s[i]);
    }
    s = retval;
    
}
int main() {
    cout << "After placing the Discord DM file in the same directory as this script, please enter the file name (including file extension): ";
    string input;
    getline(cin, input);
    ifstream inputFile(input);
    if (!inputFile) {
        cout << "Bad filename\n";
        exit(EXIT_FAILURE);
    }
    vector<string> file;
    string user;
    string clone;
    cout << "Please enter your username (to exclude your own messages): ";
    getline(cin, user);
    cout << "Enter the username of the person you wish to extract messages from: ";
    getline(cin,clone);
    //Order: Embed, Attachments, Links, Brackets, Emotes
    
    string line;
    bool parDeleting = false;
    bool flip = false;
    
    while (getline(inputFile, line)) {
        if (stripEmbedAttachments(line, parDeleting)) continue;
        stripLinks(line);
        stripBrackets(line);
        stripEmotes(line);
        if (stripUser(line, user, clone, flip)) continue;
        stripClone(line, clone);
        if(!line.empty()) file.push_back(line);
    }
    
    cout << "Choose the output option: Save to a separate file (press 'y'), display in the terminal (press 'n'), or both (enter any other character): ";
    char response;
    cin >> response;
    if(response == 'y') {
        ofstream outputFile("result.txt");
        if (!outputFile) {
            cout << "Bad output\n";
            exit(EXIT_FAILURE);
        }
        for (const string &s : file) {
            outputFile << s << endl;
        }
        outputFile.close();
        cout << "Messages successfully extracted in result.txt.\n";
    }
    else if (response == 'n') {
        for (const string &s : file) {
            cout << s << endl;
        }
    }
    else {
        ofstream outputFile("result.txt");
        if (!outputFile) {
            cout << "Bad output\n";
            exit(EXIT_FAILURE);
        }
        for (const string &s : file) {
            cout << s << endl;
            outputFile << s << endl;
        }
        outputFile.close();
        cout << "Messages successfully extracted in result.txt.\n"; 
    }
    return 0;
}