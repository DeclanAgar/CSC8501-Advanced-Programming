#pragma once
#include <vector>
#include <string>
#include <iostream>

class Expression {
public:
    struct term {
        int coef = 1;
        int variable = 0;
        int degree = 1;
        int cons = 0;
        char op = '+';
        int sum = 0;
    };
    Expression();
    ~Expression();
    std::vector<term> createExpression();
    std::vector<term> generateExpression(int, int, int);
    term createTerm(int, int);
    term generateTerm(int, int);
    int calculateTerm(struct term);
    int addConstant();
    char addOperator();
    int calculateExpression(std::vector<term>, int);
    void viewExpression(std::vector<term>&, std::ostream& out);
    std::vector<int> createOutput(std::vector<term>);
    void writeOutput(std::vector<int>, std::string);
    void viewOutput(std::vector<int>);
    std::vector<int> readOutput();
    std::vector<int> stringToInt(std::string);
    std::vector<term> deriveExpression(std::vector<int>);
    bool compareExpression(std::vector<term>, std::vector<int>, int);
    void writeExpressions(std::vector<term>, std::string);
    std::string getOutputFileName();
    std::string getExpressionFileName();
protected:
    const int INPUT_SET_MAX = 200;
    const int TERMS_MAX = 4;
    const int DEGREE_MAX = 4;
    const int COEF_MAX = 9;
    const int CONST_MAX = 1000;
    const int OFFSET = 1;
    const std::string  outputFileName = "outputFile.txt";
    const std::string expressionFileName = "testexpressionFile.txt";

    int noOfTerms = 0;
    int degree = 1;
    int coefficient = 1;
    char ans = ' ';
    int constant = 0;
    
    int startNumber = 0;
    int finishNumber = 0;
    int sum = 0;
};
