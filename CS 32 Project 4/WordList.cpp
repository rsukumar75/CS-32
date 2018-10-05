#include "provided.h"
#include <string>
#include <vector>
#include "MyHash.h"
#include <functional>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

//For loadWordList: Go through the words and create hash table with words patterns as keywords and values as a vector of strings
//Run through nested for loop to add all words of one pattern
//MyHash's find function will always return a reference to the vector of all the strings with the same word pattern.
//Use the find function to get the reference to the vector and manipulate the vector of strings within the WordList class
class WordListImpl
{
public:
    WordListImpl();
    ~WordListImpl();
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string,vector<string>>* wordlist;
    char getLower(char ch) const
    {
        if(islower(ch))
            return ch;
        else
            return (ch+32);
    }
    string getPattern(string s) const
    {
        string pattern = s;
        char ch='0';
        for(int i=0;i<pattern.size();i++)
        {
            char temp = pattern[i];
            if(isalpha(temp))
            {
                for(int j=i;j<pattern.size();j++)
                {
                    if(getLower(temp)==getLower(pattern[j]))
                        pattern[j]=ch;
                }
            }
            ch++;
        }
        return pattern;
    }
};

WordListImpl::WordListImpl()
{
    wordlist = new MyHash<string, vector<string>>;
}

WordListImpl::~WordListImpl()
{
    delete wordlist;
}
bool WordListImpl::loadWordList(string filename)
{
    delete wordlist;
    wordlist = new MyHash<string, vector<string>>;
    ifstream infile(filename);
    if(!infile)
    {
        cerr << "Error! Cannot open " << filename << endl;
        return false;
    }
    string word;
    while(getline(infile,word))
    {
        bool valid = true;
        for(int i=0;i<word.size();i++)
        {
            if(!isalpha(word[i]) && word[i] != '\'')
            {
                valid = false;
            }
        }
        if(valid)
        {
            vector<string>* t = wordlist->find(getPattern(word));
            if(t!=nullptr)
            {
                t->push_back(word);
                wordlist->associate(getPattern(word), *t);
            }
            else
            {
                vector<string> temp;
                temp.push_back(word);
                wordlist->associate(getPattern(word), temp);
            }
        }
    }
    return true;  // This compiles, but may not be correct
}

bool WordListImpl::contains(string word) const
{
    vector<string>* matches = wordlist->find(getPattern(word));
    if(matches!=nullptr)
    {
        for(int i=0;i<matches->size();i++)
        {
            bool equal = true;
            for(int j=0;j<(*matches)[i].size();j++)
            {
                if(getLower((*matches)[i][j])!=getLower(word[j]))
                {
                    equal=false;
                    break;
                }
            }
            if(equal)
                return true;
        }
    }
    return false; // This compiles, but may not be correct
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    if(cipherWord.size()!=currTranslation.size())
        return vector<string>();
    for(int i=0;i<cipherWord.size();i++)
    {
        if((!isalpha(cipherWord[i]) && cipherWord[i]!='\'' )|| (!isalpha(currTranslation[i]) && currTranslation[i]!='\'' && currTranslation[i]!='?'))
            return vector<string>();
    }
    vector<string>* matches = wordlist->find(getPattern(cipherWord));
    if(matches==nullptr) 
        return vector<string>();
    vector<string> candidates;
    for(int i=0;i<matches->size();i++)
    {
        bool valid=true;
        for(int j=0;j<(*matches)[i].size();j++)
        {
            if(isalpha(currTranslation[j]))
            {
                if(!isalpha(cipherWord[j]))
                    return vector<string>();
                if(!isalpha((*matches)[i][j]))
                {
                       valid=false;
                       break;
                }
                if(getLower(currTranslation[j])!=getLower((*matches)[i][j]))
                {
                    valid=false;
                    break;
                }
            }
            if(currTranslation[j]=='?')
            {
                if(!isalpha(cipherWord[j]))
                    return vector<string>();
                if(!isalpha((*matches)[i][j]))
                {
                    valid=false;
                    break;
                }
            }
            if(currTranslation[j]=='\'')
            {
                if(cipherWord[j]!='\'')
                    return vector<string>();
                if((*matches)[i][j]!='\'')
                {
                    valid=false;
                    break;
                }
            }
        }
        if(valid)
            candidates.push_back((*matches)[i]);
    }
    return candidates;  // This compiles, but may not be correct
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);  
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
