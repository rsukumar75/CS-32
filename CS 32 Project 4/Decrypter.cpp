#include "provided.h"
#include <string>
#include <vector>
#include "MyHash.h"
#include <iostream>
using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
    DecrypterImpl();
    ~DecrypterImpl();
private:
    WordList* words;
    Translator translate;
    Tokenizer tokens;
    MyHash<int, bool> used;
    int countQuestionMarks(string s)
    {
        int cnt=0;
        for(int i=0;i<s.size();i++)
        {
            if(s[i]=='?')
                cnt++;
        }
        return cnt;
    }
    
    string chooseEncryptedWord(vector<string> words)
    {
        bool reset=true;
        for(int i=0;i<words.size();i++)
        {
            if(used.find(i)==nullptr)
            {
                reset=false;
                break;
            }
        }
        if(reset)
            used.reset();
        vector<string> translatedWords;
        for(int i=0;i<words.size();i++)
        {
            translatedWords.push_back(translate.getTranslation(words[i]));
        }
        string max="";
        int m = 0;
        int x = 0;
        for(int i=0;i<translatedWords.size();i++)
        {
            if(countQuestionMarks(translatedWords[i])>m && used.find(i)==nullptr)
            {
                m = countQuestionMarks(translatedWords[i]);
                max = words[i];
                x=i;
            }
            else if(countQuestionMarks(translatedWords[i])==m && used.find(i)==nullptr && translatedWords[i].size()>max.size())
            {
                m = countQuestionMarks(translatedWords[i]);
                max = words[i];
                x=i;
            }
        }
        used.associate(x, true);
        return max;
    }
    char getLower(char ch) const
    {
        if(!isalpha(ch))
            return ch;
        if(islower(ch))
            return ch;
        else
            return (ch+32);
    }
    string toLower(string s)
    {
        string t="";
        for(int i=0;i<s.size();i++)
        {
            t+=getLower(s[i]);
        }
        return t;
    }
    
    void sort(vector<string>& s)
    {
        int i,j;
        string temp = s[0];
        for(i=1;i<s.size();i++)
        {
            temp=s[i];
            for(j=i-1;j>=0 && temp<s[j];j--)
            {
                s[j+1]=s[j];
            }
            s[j+1]=temp;
        }
    }
    
};

DecrypterImpl::DecrypterImpl()
:tokens(" 0123456789,;:.!()[]{}-\"#$%^&")
{
    words = new WordList;
}

DecrypterImpl::~DecrypterImpl()
{
    delete words;
}

bool DecrypterImpl::load(string filename)
{
    delete words;
    words= new WordList;
    return words->loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    //cerr<<endl<<"each run ==============================="<<endl;
    vector<string> cipherWords;
    vector<string> validTrans;
    cipherWords=tokens.tokenize(ciphertext);
    string w = chooseEncryptedWord(cipherWords);
    //cerr << "Chosen Word: " << w << endl;
    string partialTrans = translate.getTranslation(w);
    //cerr << "Partial translation: " << partialTrans << endl;
    vector<string> candidates = words->findCandidates(w, partialTrans);
    if(candidates.empty())
    {
        translate.popMapping();
        return validTrans;
    }
    for(int i=0;i<candidates.size();i++)
    {
        //cerr << "Candidate: " << candidates[i] << endl;
        bool possiblyValid=true;
        Translator temp;
        for(char ch='a';ch<='z';ch++)
        {
            string t = "";
            t+= ch;
            temp.pushMapping(t, translate.getTranslation(t));
        }
        for(int j=0;j<w.size();j++)
        {
            string check="";
            string mapped="";
            check+=w[j];
            mapped+=getLower(candidates[i][j]);
            //cerr << "Char in word: " << check << endl;
            //cerr << "Char in candidate: " << mapped << endl;
            if(temp.getTranslation(check)=="?")
            {
                //cerr << "Translated to a ?" << endl;
                if(!temp.pushMapping(check, mapped))
                {
                    possiblyValid=false;
                    break;
                }
                //if(!possiblyValid)
                    //cerr << "Did not map " << check << " to " << mapped << endl;
            }
            else if(toLower(temp.getTranslation(check))==mapped)
            {
                //cerr << "Mapping exists so continue." << endl;
                continue;
            }
            else
            {
                if(!temp.pushMapping(check,mapped))
                {
                    possiblyValid=false;
                    break;
                }
                //if(!possiblyValid)
                    //cerr << "Did not map " << check << " to " << mapped << endl;
            }
        }
        if(possiblyValid)
        {
            string possibleTrans = temp.getTranslation(ciphertext);
            //cerr << "Possible translation: " << possibleTrans << endl;
            vector<string> transCipherWords = tokens.tokenize(possibleTrans);
            bool wordsInList = true;
            if(!transCipherWords.empty())
            {
                for(int k=0;k<transCipherWords.size();k++)
                {
                    if(countQuestionMarks(transCipherWords[k])==0 && !words->contains(transCipherWords[k]))
                    {
                        wordsInList = false;
                        break;
                    }
                }
                if(wordsInList)
                {
                    if(countQuestionMarks(possibleTrans)>0)
                    {
                        string newtrans="";
                        string keys="";
                        for(char ch='a';ch<='z';ch++)
                        {
                            string check2="";
                            check2+=ch;
                            //cerr << "Character: " << check2 << endl;
                            //cerr << "Temporary mapped to: " << temp.getTranslation(check2) << endl;
                            //cerr << "Currently mapped to: " << translate.getTranslation(check2) << endl;
                            if(translate.getTranslation(check2)!=temp.getTranslation(check2))
                            {
                                newtrans+=temp.getTranslation(check2);
                                keys+=ch;
                            }
                        }
                       // cerr << "Keys: " << keys << endl;
                       // cerr << "New translation of keys: " << newtrans << endl;
                        translate.pushMapping(keys, newtrans);
                        vector<string> tempWords=crack(ciphertext);
                        for(int i=0;i<tempWords.size();i++)
                        {
                            validTrans.push_back(tempWords[i]);
                        }
                    }
                    if(countQuestionMarks(possibleTrans)==0)
                    {
                        validTrans.push_back(possibleTrans);
                        //cerr << "Pushed valid translation." << endl;
                    }
                }
            }
        }
    }
    translate.popMapping();
    used.reset();
    if(validTrans.size()>=1)
        sort(validTrans);
    return validTrans;  // This compiles, but may not be correct
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
