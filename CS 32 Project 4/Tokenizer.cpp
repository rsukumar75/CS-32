#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
    ~TokenizerImpl();
private:
    MyHash<char, bool>* sep;
};

TokenizerImpl::~TokenizerImpl()
{
    delete sep;
}

TokenizerImpl::TokenizerImpl(string separators)
{
    sep = new MyHash<char, bool>;
    for(int i=0;i<separators.size();i++)
    {
        sep->associate(separators[i], true);
    }
    
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> words;
    string s1 = s+ " ";
    string temp="";
    bool seenSeparator=false;
    
    for(int i=0;i<s1.size();i++)
    {
        if(sep->find(s1[i])==nullptr)
        {
            temp=temp+s1[i];
            seenSeparator=false;
        }
        else
        {
            if(!seenSeparator)
            {
                words.push_back(temp);
                temp="";
                seenSeparator=true;
            }
        }
    }
    return words;  // This compiles, but may not be correct
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
