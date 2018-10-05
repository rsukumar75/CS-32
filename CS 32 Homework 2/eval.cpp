#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.

bool isValidSyntax(string infix);
void convert(string infix, string& postfix);

/* Initialize the operand stack to empty
 For each character ch in the postfix string
 if ch is an operand
 push the value that ch represents onto the operand stack
 else // ch is a binary operator
 set operand2 to the top of the operand stack
 pop the stack
 set operand1 to the top of the operand stack
 pop the stack
 apply the operation that ch represents to operand1 and
 operand2, and push the result onto the stack
 When the loop is finished, the operand stack will contain one item,
 the result of evaluating the expression */

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if(isValidSyntax(infix))
    {
        convert(infix,postfix);
        for(int i=0;i<postfix.size();i++)
        {
            if(isalpha(postfix[i]) && !values.contains(postfix[i]))
               return 2;
        }
        stack <int> operand;
        int o1=0;
        int o2=0;
        int r=0;
        int v=0;
        for(int i=0;i<postfix.size();i++)
        {
            if(isalpha(postfix[i]))
            {
                values.get(postfix[i],v);
                operand.push(v);
            }
            else
            {
                o2=operand.top();
                operand.pop();
                o1=operand.top();
                operand.pop();
                if(postfix[i]=='+')
                    r=o1+o2;
                else if(postfix[i]=='-')
                    r=o1-o2;
                else if(postfix[i]=='*')
                    r=o1*o2;
                else
                {
                    if(o2==0)
                        return 3;
                    else
                        r=o1/o2;
                }
                operand.push(r);
            }
        }
        result=operand.top();
        return 0;
    }
    
    return 1;
}


bool balanced(string infix)
{
    stack <char> brackets;
    for(int i=0;i<infix.size();i++)
    {
        if(infix[i]=='(')
            brackets.push(infix[i]);
        else if(infix[i]==')')
        {
            if(! brackets.empty() && brackets.top()=='(')
                brackets.pop();
            else
                return false;
        }
    }
    return brackets.empty();
}
bool isValidSyntax(string infix)
{
    if(! balanced(infix))
        return false;
    stack <char> c;
    for(int i=0;i<infix.size();i++)
    {
        if(infix[i]=='-' || infix[i]=='+' || infix[i]=='*' || infix[i]=='/')
        {
            if(c.empty())
                return false;
            else if(c.top()=='-' || c.top()=='+' || c.top()=='/' || c.top()=='*' || c.top()=='(')
                return false;
            else
                c.push(infix[i]);
        }
        else if(isalpha(infix[i]))
        {
            if(c.empty())
            {
                if(isupper(infix[i]))
                    return false;
                else
                    c.push(infix[i]);
            }
            else if(isupper(infix[i]) || isalpha(c.top()))
                return false;
            else
                c.push(infix[i]);
        }
        else if(infix[i]==')')
        {
            if(c.empty())
                return false;
            else if(!isalpha(c.top()))
                return false;
        }
        else if(!isalpha(infix[i]) && infix[i]!=' ' && infix[i]!='(' && infix[i]!=')')
            return false;
    }
    if(!c.empty() && isalpha(c.top()))
        return true;
    else
        return false;
}

/* Initialize postfix to empty
 Initialize the operator stack to empty
 For each character ch in the infix string
 Switch (ch)
 case operand:
 append ch to end of postfix
 break
 case '(':
 push ch onto the operator stack
 break
 case ')':
 // pop stack until matching '('
 While stack top is not '('
 append the stack top to postfix
 pop the stack
 pop the stack  // remove the '('
 break
 case operator:
 While the stack is not empty and the stack top is not '('
 and precedence of ch <= precedence of stack top
 append the stack top to postfix
 pop the stack
 push ch onto the stack
 break
 While the stack is not empty
 append the stack top to postfix
 pop the stack */

void convert(string infix,string& postfix)
{
    postfix="";
    stack <char> op;
    for(int i=0;i<infix.size();i++)
    {
        if(infix[i]==' ')
            continue;
        else if(isalpha(infix[i]))
            postfix=postfix+infix[i];
        else if(infix[i]=='(')
            op.push(infix[i]);
        else if(infix[i]==')')
        {
            while(!op.empty() && op.top()!='(')
            {
                postfix=postfix+op.top();
                op.pop();
            }
            op.pop();
        }
        else
        {
            while(!op.empty() && op.top()!='(' && ((infix[i]=='+' && op.top()=='*') || (infix[i]=='+' && op.top()=='/') || (infix[i]=='-' && op.top()=='*') || (infix[i]=='-' && op.top()=='/') || (infix[i]=='*' && op.top()=='/') || (infix[i]==op.top()) || (infix[i]=='/' && op.top()=='*') || (infix[i]=='+' && op.top()=='-') || (infix[i]=='-' && op.top()=='+')))
            {
                    postfix=postfix+op.top();
                    op.pop();
            }
            op.push(infix[i]);
        }
    }
    while(!op.empty())
    {
        postfix=postfix+op.top();
        op.pop();
    }
}




