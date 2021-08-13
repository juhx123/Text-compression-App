#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;

struct token{
    char* self;
    int count;
    int code;
    int hashNum;
    token(char* x,int h): self(x),count(1), code(0),hashNum(h){}
    token(int x): self(NULL),count(x), code(0),hashNum(-1){}
};


int computeHash(const char* s) {
    int stringhash = 5381;
    int k = 0;
    stringhash += (static_cast<int>(s[0]) + (stringhash * 33) % 20001) % 20001;
    while(s[k] <= 'z' && s[k] >= 'A') k++;
    for(size_t i = 1; i < k; ++i){
        stringhash += (static_cast<int>(s[i]) + (stringhash * 33) % 20001) % 20001;
    }
    return stringhash % 20001;
}

int main(){
    ifstream inputText("input.txt");
    ofstream mappingFile("map.txt");
    ofstream outputText("output.txt");

    char text[1000000];
    char text2[1000000]; //store the text in two array because I cant use strtok twice.
    char c;
    int i = 0;

    vector<token*> encoder(20001); //hash table
    vector<token*> decoder(20001);

    while(inputText.get(c)){
        if(c=='.'||c=='?'||c=='!'||c=='-'||c=='\"'||c=='\''||c=='\''||c==';'||c==':'||c==','||c=='/'||c=='['||c==']'||c=='('||c==')'){
            text[i] = ' ';
            text2[i] = ' ';
            text[i+1] = c;
            text2[i+1] = c;
            text[i+2] = ' ';
            text2[i+2] = ' '; //seperate the punctations
            i+=2;
        }
        else{
            text[i] = c;
            text2[i] = c;
        }
        i++;
    }
   // cout << text;
    char* pch;

    pch = strtok(text," ");

    while(pch != NULL){
        int hashindex = computeHash(pch);
       // outputText << pch << " " << hashindex <<endl;
        if(encoder.at(hashindex) == nullptr){
            token* h = new token(pch,hashindex);
            encoder.at(hashindex) = h;
        }
        else{
            encoder.at(hashindex)->count += 1;
        }
        pch = strtok(NULL," ");
    }

    int count = 0;
    for(int i = 0; i < 20001; i++){
        if(encoder.at(i) != nullptr){
        count++;
        }
    }

    vector<token*> Sortedmap(count);

    token* newtoken = new token(-1);
    encoder.at(20000) = newtoken;
    for(int i = 0; i < count; i++){
        int maxindex = 20000;
        for(int j = 0; j < 20001; j++){
            if(encoder.at(j) != nullptr){
                if(encoder.at(maxindex)->count < encoder.at(j)->count){
                    maxindex = j;
                }
            }
        }
        if(encoder.at(maxindex) != nullptr){
            Sortedmap.at(i) = encoder.at(maxindex);
            encoder.at(maxindex) = nullptr;
        }
    }

    int m = 1;
    mappingFile << "Word - code - frequency" <<endl;
    for(int i = 0; i < count; i++){
        Sortedmap.at(i)->code = m;
        decoder.at(Sortedmap.at(i)->hashNum) = Sortedmap.at(i);
        mappingFile << Sortedmap.at(i)->self << " - " <<Sortedmap.at(i)->code << " - " << Sortedmap.at(i)->count << endl;
        m++;
    }

    char* ch;
    ch = strtok(text2," ");
    int z = 1;
    while(ch != NULL){
        int hashindex = computeHash(ch);
        outputText << decoder.at(hashindex)->code << " ";
        if(!(z%30)) outputText << endl; // doing newline to make the format nicer.
        ch = strtok(NULL," ");
        ++z;
    }


    inputText.close();
    mappingFile.close();
    outputText.close();
    return 0;
}
