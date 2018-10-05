#include "provided.h"
#include <string>
#include "MyHash.h"
#include <cctype>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    ~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    struct Node
    {
        MyHash<char,char> table;
        string inverted;
        Node* next;
    };
    Node* head;
    Node* top;
    string mapped;
    char getLower(char ch) const
    {
        if(islower(ch))
            return ch;
        else
            return (ch+32);
    }
};

TranslatorImpl::TranslatorImpl()
{
    head = new Node;
    head->next=nullptr;
    head->inverted="";
    mapped="";
    for(char ch='a';ch<='z';ch++)
    {
        head->table.associate(ch, '?');
    }
    top=head;
}

TranslatorImpl::~TranslatorImpl()
{
    if(head==nullptr)
        return;
    else if(head->next==nullptr)
        delete head;
    else
    {
        Node* temp=head;
        while(head!=nullptr)
        {
            head=head->next;
            delete temp;
            temp=head;
        }
    }
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    Node* temp = new Node;
    temp->next=nullptr;
    temp->inverted="";
    for(char ch='a';ch<='z';ch++)
    {
            temp->table.associate(ch, *(top->table.find(ch)));
    }
    if(ciphertext.size()!=plaintext.size())
    {
        delete temp;
        return false;
    }
    for(int i=0;i<ciphertext.size();i++)
    {
        if(!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
        {
            delete temp;
            return false;
        }
        if(temp->table.find(getLower(ciphertext[i]))!=nullptr && *(temp->table.find(getLower(ciphertext[i])))!='?')
        {
            delete temp;
            return false;
        }
        for(int j=0;j<mapped.size();j++)
        {
            if(getLower(plaintext[i])==getLower(mapped[j]))
            {
                delete temp;
                return false;
            }
        }
        
        temp->table.associate(getLower(ciphertext[i]), getLower(plaintext[i]));
        mapped=mapped+plaintext[i];
        temp->inverted=temp->inverted+plaintext[i];
    }
    top->next=temp;
    top=top->next;
    return true;  // This compiles, but may not be correct
}

bool TranslatorImpl::popMapping()
{
    if(top==head)
        return false;
    Node* temp=head;
    while(temp->next!=top)
        temp=temp->next;
    mapped = mapped.substr(0,mapped.size()-top->inverted.size());
    delete top;
    temp->next=nullptr;
    top=temp;
    return true;  // This compiles, but may not be correct
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string translation;
    for(int i=0;i<ciphertext.size();i++)
    {
        if(!isalpha(ciphertext[i]))
            translation=translation+ciphertext[i];
        else
        {
            char ch = *(top->table.find(getLower(ciphertext[i])));
            if(ch=='?')
                translation=translation+ch;
            else if(isupper(ciphertext[i]))
            {
                ch=toupper(ch);
                translation=translation+ ch;
            }
            else if(islower(ciphertext[i]))
                translation=translation+ ch;
        }
    }
    return translation;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}


