#include <iostream>
#include <stack>
#include <string>
#include <math.h>
#include <cctype>
#include <unordered_map>
using namespace std;

struct ExceptionSyntaxError : public exception {
    const char* what() const throw () {
        return "Systax Error \n";
    }
};

struct ExceptionNoMatchingVariableName : public exception {
    const char* what() const throw () {
        return "No Matching Variable Name \n";
    }
};

struct ExceptionEmptyString : public exception {
    const char* what() const throw () {
        return "Empty String \n";
    }
};

bool isOperator(char c) {
    return c == '|' || c == '&' || c == '>' || c == '+';
}

bool postfixCalculation(string& str, unordered_map < string, bool >& m) {
    int size = str.size();
    stack <bool> value;

    string variableName ="";
    for (int i = 0; i < size; i++) {
        
        char c = str[i];
        if (isalpha(c)||isdigit(c)) {//check it is a continuos variable name
            variableName += c;
            continue;
        }

        if (variableName.size() > 0) {//replace variable with its initialized value
            if (m.find(variableName) == m.end()) throw ExceptionNoMatchingVariableName();
            value.push(m[variableName]);
            variableName = "";
        }

        if (isOperator(c)) {
            //pop variable from stack
            if (value.size() < 2) throw ExceptionSyntaxError();
            bool val2 = value.top();
            value.pop();
            bool val1 = value.top();
            value.pop();

            //choose an operand to do
            switch (c)
            {
            case '&':
                value.push(val1 && val2);
                break;
            case '|':
                value.push(val1 || val2);
                break;
            case '>':
                if (val1 == 1 && val2 == 0) value.push(0);
                else value.push(1);
                break;
            case '+':
                if (val1 == val2) value.push(0);
                else value.push(1);
                break;
            }
        }
        else if(c == '!') {//invert by pop and push
            if (value.size() < 1) throw ExceptionSyntaxError();
            bool newTop = 1 - value.top();
            value.pop();
            value.push(newTop);
        }
    }
    if (value.size() != 1) throw ExceptionSyntaxError();
    return value.top();
}

bool prefixCalculation(string& str, unordered_map < string, bool >& m) {
    int size = str.size();
    stack <bool> value;

    string variableName = "";
    for (int i = size-1; i > -1 ; i--) {

        char c = str[i];

        if (isalpha(c) || isdigit(c)) {//check it is a continuos variable name
            variableName += c;
            continue;
        }

        if (variableName.size() > 0) {//replace variable with its initialized value
            //because we are check backward so the variableName need to be reversed
            string reverseVariableName;
            for (int i = variableName.size()-1; i >-1; i--) {
                reverseVariableName += variableName[i];
            }
            variableName = reverseVariableName;

            if (m.find(variableName) == m.end()) throw ExceptionNoMatchingVariableName();
            value.push(m[variableName]);
            variableName = "";
        }

        if (isOperator(c)) {
            //pop variable from stack
            if (value.size() < 2) throw ExceptionSyntaxError();
            bool val1 = value.top();
            value.pop();
            bool val2 = value.top();
            value.pop();

            //choose an operand to do
            switch (c)
            {
            case '&':
                value.push(val1 && val2);
                break;
            case '|':
                value.push(val1 || val2);
                break;
            case '>':
                if (val1 == 1 && val2 == 0) value.push(0);
                else value.push(1);
                break;
            case '+':
                if (val1 == val2) value.push(0);
                else value.push(1);
                break;
            }
        }
        else if (c == '!') {//invert by pop and push
            if (value.size() < 1) throw ExceptionSyntaxError();
            bool newTop = 1 - value.top();
            value.pop();
            value.push(newTop);
        }
    }
    if (value.size() != 1) throw ExceptionSyntaxError();
    return value.top();
}

bool checkIsPostfix(string str) {
    bool isPostfix = 0;
    for (int i =0; i < str.size(); i++) {
        if (str[i] != ' ') {
            if (isalpha(str[i])) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    throw ExceptionEmptyString();
}

bool solve(string str, unordered_map < string, bool > m) {
    
    bool isPostfix = checkIsPostfix(str); // 1 = postfix , 0 = prefix
    
    if (isPostfix) {
        return postfixCalculation(str, m);
    }
    else {
        return prefixCalculation(str, m);
    }
}

int main() {
    try
    {
        // Example Tesecase
        string str1 = "+ | > & p ! abc r s q";
        unordered_map <string, bool> m1 = { make_pair("p" , 1), make_pair("abc", 1) , make_pair("r", 0), make_pair("s", 0),  make_pair("q", 0) };
        cout << str1 << " = " << solve(str1, m1) << endl;

        string str2 = " p abc ! & r > s | q + ";
        cout << str2 << " = " << solve(str2, m1) << endl;

        string str3 = " p !  ! ";
        cout << str3 << " = " << solve(str3, m1) << endl;

        string str4 = " !!  p ";
        cout << str4 << " = " << solve(str4, m1) << endl;

        //Input Custom Testcase here
        string str5 = ""; //input string
        //Input custom variable and theirs value
        unordered_map <string, bool> m2 = { make_pair("p" , 1), make_pair("q", 1) , make_pair("r", 0), make_pair("s", 0),  make_pair("t", 0) };
        cout << str5 << " = " << solve(str5, m2) << endl;
    }
    catch (ExceptionSyntaxError &e)
    {
        cout << e.what();
    }
    catch (ExceptionNoMatchingVariableName& e)
    {
        cout << e.what();
    }
    catch (ExceptionEmptyString& e)
    {
        cout << e.what();
    }
    return 0;
}

