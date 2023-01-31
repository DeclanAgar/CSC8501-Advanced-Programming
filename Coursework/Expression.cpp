#include "Expression.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <math.h>

Expression::Expression() {}
Expression::~Expression() {}

std::vector<Expression::term> Expression::createExpression() {
	std::cout << "Enter an amount of terms from 1-4: " << "\n";
	std::cin >> noOfTerms;
	std::vector<term> terms(noOfTerms);
	int i = 0;
	for (auto t : terms) {
		if (i == noOfTerms - 1 && i != 0) {
				std::cout << "Do you want to add a constant? Y/N" << "\n";
				std::cin >> ans;
				if (ans == 'Y') {
					t.cons = addConstant();
					terms[i] = t;
					continue;
				}
		}
		terms[i] = createTerm(coefficient, degree);
		if (i < noOfTerms - OFFSET && noOfTerms != 1)
				terms[i].op = addOperator();
		i++;
	}
	return terms;
}

std::vector<Expression::term> Expression::generateExpression(int noOfTerms, int coefficient, int degree) {
	std::cout << "Generate Expression\n";
	std::vector<term> terms(noOfTerms);
	for (int i = 0; i < terms.size(); i++)
		terms[i] = generateTerm(coefficient, degree);
	return terms;
}

Expression::term Expression::createTerm(int coefficient, int degree) {
	std::cout << "Enter a coefficient from 1-9: " << "\n";
	std::cin >> coefficient;
	std::cout << "Enter a degree from 1-4: " << "\n";
	std::cin >> degree;
	return generateTerm(coefficient, degree);
}

Expression::term Expression::generateTerm(int coefficient, int degree) {
	term t;
	coefficient <= COEF_MAX ? t.coef = coefficient : t.coef = 1;
	degree <= DEGREE_MAX ? t.degree = degree : t.degree = 1;
	return t;
}

int Expression::calculateTerm(term t) {
	return (int)pow(t.variable, t.degree) * t.coef;
}

int Expression::addConstant() {
	std::cout << "Enter a constant from 1-1000: " << "\n";
	std::cin >> constant;
	return constant;
}

char Expression::addOperator() {
	std::cout << "Enter an operator, + or -:" << "\n";
	std::cin >> ans;
	return ans;
}

int Expression::calculateExpression(std::vector<term> expression, int variable) {
	sum = 0;
	int i = 0;
	for (auto t : expression) {
		t.variable = variable;
		t.cons == 0 ? t.sum = calculateTerm(t) : t.sum = t.cons;
		i == 0 ? sum += t.sum : expression[i - OFFSET].op == '+' ? sum += t.sum : sum -= t.sum;
		i++;
	}
	return sum;
}

void Expression::viewExpression(std::vector<Expression::term>& expression, std::ostream& out) {
	int i = 0;
	for (const auto& t : expression) {
		t.cons != 0 ? out << t.cons :
			t.coef == 1 && t.degree == 1 ? i == expression.size() - OFFSET ? out << "x" : out << "x" << t.op :
			t.coef == 1 ? i == expression.size() - OFFSET ? out << "x^" << t.degree : out << "x^" << t.degree << t.op :
			t.degree == 1 ? i == expression.size() - OFFSET ? out << t.coef << "x" : out << t.coef << "x" << t.op :
			i == expression.size() - OFFSET ? out << t.coef << "x^" << t.degree : out << t.coef << "x^" << t.degree << t.op;
		i++;
	}
	out << std::endl;
}

std::vector<int> Expression::createOutput(std::vector<term> expression) {
	std::cout << "Enter the starting number of the input set" << "\n";
	std::cin >> startNumber;
	std::cout << "Enter the finishing number of the input set" << "\n";
	std::cin >> finishNumber;
	std::vector<int> outputSet(finishNumber - startNumber + OFFSET);
	for (int i = 0; i < outputSet.size(); i++)
	{
		std::cout << i << "\n";
		outputSet[i] = calculateExpression(expression, startNumber);
		startNumber++;
	}
	return outputSet;
}

void Expression::writeOutput(std::vector<int> outputSet, std::string fileName) throw (std::invalid_argument) {
	std::ofstream outputFile("outputFile.txt", std::ios::in | std::ios::out | std::ios::app);
	if (outputFile.is_open()) {
		for (int i = 0; i < outputSet.size(); i++) {
			i != outputSet.size() - OFFSET ? outputFile << outputSet[i] << "," : outputFile << outputSet[i];
		}
		outputFile << "\n";
		outputFile.close();
	} else
		throw std::invalid_argument("File " + fileName + " cannot be opened!\n");
}

void Expression::viewOutput(std::vector<int> outputSet) {
	for (int i = 0; i < outputSet.size(); i++)
		i == outputSet.size() - OFFSET ? std::cout << outputSet[i] : std::cout << outputSet[i] << ", ";
	std::cout << "\n";
}

std::vector<int> Expression::readOutput() throw (std::invalid_argument, std::runtime_error) {
	std::ifstream outputFile;
	outputFile.open(outputFileName);
	if (outputFile.fail())
		throw std::invalid_argument("File " + outputFileName + " cannot be opened!\n");

	std::cout << "Please enter the line number of the output set you want to read: \n";
	int lineNumber, currentLine = 0;
	std::cin >> lineNumber;
	std::string line;

	while (std::getline(outputFile, line, '\n')) {
		currentLine++;
		if (currentLine == lineNumber)
			break;
	}
	if (currentLine < lineNumber)
		throw std::runtime_error("Line number does not exist in file!\n");
	outputFile.close();
	return stringToInt(line);
}

std::vector<int> Expression::stringToInt(std::string line) {
	std::stringstream ss(line);
	std::vector<int> outputSet;
	int value;
	char comma;
	while (ss >> value) {
		outputSet.push_back(value);
		ss >> comma;
	}
	return outputSet;
}

std::vector<Expression::term> Expression::deriveExpression(std::vector<int> outputSet) {
	std::vector<term> expression;
	for (int startNumber = 0; startNumber <= INPUT_SET_MAX - outputSet.size(); startNumber++) {
		std::cout << "Checking " << startNumber << " as first input set value";
		for (int noOfTerms = 1; noOfTerms <= TERMS_MAX; noOfTerms++) {
			if (noOfTerms == 1) { // Number of terms is 1
				for (int degree = 1; degree <= DEGREE_MAX; degree++) {
					for (int coefficient = -COEF_MAX; coefficient <= COEF_MAX; coefficient++) {
						expression = generateExpression(noOfTerms, coefficient, degree);
						if (compareExpression(expression, outputSet, startNumber)) {
							std::cout << "Expression derived from output set.\n";
							return expression;
						}
					}
				}
			}
			else if (noOfTerms == 2) {
				expression = generateExpression(noOfTerms, coefficient, degree);
				// FOR EVERY POSSIBLE TERM 1
				for (int degree = 1; degree <= DEGREE_MAX; degree++) {
					for (int coefficient = -COEF_MAX; coefficient <= COEF_MAX; coefficient++) {
						expression[0] = generateTerm(coefficient, degree);
						// FOR EVERY POSSIBLE TERM 2
						// CHECK FOR CONSTANT
						for (int constant = 1; constant <= CONST_MAX; constant++) {
							expression[1].cons = constant;
							if (compareExpression(expression, outputSet, startNumber)) {
								return expression;
							}
							else {
								expression[0].op = '-';
								if (compareExpression(expression, outputSet, startNumber)) {
									return expression;
								}
								else {
									expression[0].op = '+';
								}
							}
						}
						expression[1].cons = 0;
						// TERM 2 IS NOT CONSTANT
						for (int degree2 = 1; degree2 <= DEGREE_MAX; degree2++) {
							for (int coefficient2 = -COEF_MAX; coefficient2 <= COEF_MAX; coefficient2++) {
								expression[1] = generateTerm(coefficient2, degree2);
								if (compareExpression(expression, outputSet, startNumber)) {
									return expression;
								}
								else {
									expression[0].op = '-';
									if (compareExpression(expression, outputSet, startNumber)) {
										return expression;
									}
									else {
										expression[0].op = '+';
									}
								}
							}
						}

					}
				}
			}
			else if (noOfTerms == 3) {
				expression = generateExpression(noOfTerms, coefficient, degree);
				// FOR EVERY POSSIBLE TERM 1
				for (int degree = 1; degree <= DEGREE_MAX; degree++) {
					for (int coefficient = -COEF_MAX; coefficient <= COEF_MAX; coefficient++) {
						expression[0] = generateTerm(coefficient, degree);
						// FOR EVERY POSSIBLE TERM 2
						for (int degree2 = 1; degree2 <= DEGREE_MAX; degree2++) {
							for (int coefficient2 = -COEF_MAX; coefficient2 <= COEF_MAX; coefficient2++) {
								expression[1] = generateTerm(coefficient2, degree2);
								// FOR EVERY POSSIBLE TERM 3
								// CHECK FOR CONSTANT
								for (int constant = 1; constant <= CONST_MAX; constant++) {
									expression[2].cons = constant;
									if (compareExpression(expression, outputSet, startNumber)) {
										return expression;
									}
									else {
										expression[0].op = '+';
										expression[1].op = '-';
										if (compareExpression(expression, outputSet, startNumber)) {
											return expression;
										}
										else {
											expression[0].op = '-';
											expression[1].op = '+';
											if (compareExpression(expression, outputSet, startNumber)) {
												return expression;
											}
											else {
												expression[0].op = '-';
												expression[1].op = '-';
												if (compareExpression(expression, outputSet, startNumber)) {
													return expression;
												}
												else {
													expression[0].op = '+';
													expression[1].op = '+';
												}
											}
										}
									}
								}
								expression[2].cons = 0;
								// TERM 3 IS NOT CONSTANT
								for (int degree3 = 1; degree3 <= DEGREE_MAX; degree3++) {
									for (int coefficient3 = -COEF_MAX; coefficient3 <= COEF_MAX; coefficient3++) {
										expression[2] = generateTerm(coefficient3, degree3);
										if (compareExpression(expression, outputSet, startNumber)) {
											return expression;
										}
										else {
											expression[0].op = '+';
											expression[1].op = '-';
											if (compareExpression(expression, outputSet, startNumber)) {
												return expression;
											}
											else {
												expression[0].op = '-';
												expression[1].op = '+';
												if (compareExpression(expression, outputSet, startNumber)) {
													return expression;
												}
												else {
													expression[0].op = '-';
													expression[1].op = '-';
													if (compareExpression(expression, outputSet, startNumber)) {
														return expression;
													}
													else {
														expression[0].op = '+';
														expression[1].op = '+';
													}
												}
											}
										}
									}
								}
							}
						}

					}
				}
			}
			else { // Terms is 4
				expression = generateExpression(noOfTerms, coefficient, degree);
				// FOR EVERY POSSIBLE TERM 1
				for (int degree = 1; degree <= DEGREE_MAX; degree++) {
					for (int coefficient = -COEF_MAX; coefficient <= COEF_MAX; coefficient++) {
						expression[0] = generateTerm(coefficient, degree);
						// FOR EVERY POSSIBLE TERM 2
						for (int degree2 = 1; degree2 <= DEGREE_MAX; degree2++) {
							for (int coefficient2 = -COEF_MAX; coefficient2 <= COEF_MAX; coefficient2++) {
								expression[1] = generateTerm(coefficient2, degree2);
								// FOR EVERY POSSIBLE TERM 3
								for (int degree3 = 1; degree3 <= DEGREE_MAX; degree3++) {
									for (int coefficient3 = -COEF_MAX; coefficient3 <= COEF_MAX; coefficient3++) {
										expression[2] = generateTerm(coefficient3, degree3);
										// FOR EVERY POSSIBLE TERM 4
										// CHECK FOR CONSTANT
										for (int constant = 1; constant <= CONST_MAX; constant++) {
											expression[3].cons = constant;
											if (compareExpression(expression, outputSet, startNumber)) {
												return expression;
											}
											else {
												expression[0].op = '+';
												expression[1].op = '+';
												expression[2].op = '-';
												if (compareExpression(expression, outputSet, startNumber)) {
													return expression;
												}
												else {
													expression[0].op = '+';
													expression[1].op = '-';
													expression[2].op = '+';
													if (compareExpression(expression, outputSet, startNumber)) {
														return expression;
													}
													else {
														expression[0].op = '+';
														expression[1].op = '-';
														expression[2].op = '-';
														if (compareExpression(expression, outputSet, startNumber)) {
															return expression;
														}
														else {
															expression[0].op = '-';
															expression[1].op = '+';
															expression[2].op = '+';
															if (compareExpression(expression, outputSet, startNumber)) {
																return expression;
															}
															else {
																expression[0].op = '-';
																expression[1].op = '+';
																expression[2].op = '-';
																if (compareExpression(expression, outputSet, startNumber)) {
																	return expression;
																}
																else {
																	expression[0].op = '-';
																	expression[1].op = '-';
																	expression[2].op = '+';
																	if (compareExpression(expression, outputSet, startNumber)) {
																		return expression;
																	}
																	else {
																		expression[0].op = '-';
																		expression[1].op = '-';
																		expression[2].op = '-';
																		if (compareExpression(expression, outputSet, startNumber)) {
																			return expression;
																		}
																		else {
																			expression[0].op = '+';
																			expression[1].op = '+';
																			expression[2].op = '+';
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
										expression[3].cons = 0;
										// TERM 4 IS NOT CONSTANT
										for (int degree4 = 1; degree4 <= DEGREE_MAX; degree4++) {
											for (int coefficient4 = -COEF_MAX; coefficient4 <= COEF_MAX; coefficient4++) {
												expression[3] = generateTerm(coefficient4, degree4);
												if (compareExpression(expression, outputSet, startNumber)) {
													return expression;
												}
												else {
													expression[0].op = '+';
													expression[1].op = '+';
													expression[2].op = '-';
													if (compareExpression(expression, outputSet, startNumber)) {
														return expression;
													}
													else {
														expression[0].op = '+';
														expression[1].op = '-';
														expression[2].op = '+';
														if (compareExpression(expression, outputSet, startNumber)) {
															return expression;
														}
														else {
															expression[0].op = '+';
															expression[1].op = '-';
															expression[2].op = '-';
															if (compareExpression(expression, outputSet, startNumber)) {
																return expression;
															}
															else {
																expression[0].op = '-';
																expression[1].op = '+';
																expression[2].op = '+';
																if (compareExpression(expression, outputSet, startNumber)) {
																	return expression;
																}
																else {
																	expression[0].op = '-';
																	expression[1].op = '+';
																	expression[2].op = '-';
																	if (compareExpression(expression, outputSet, startNumber)) {
																		return expression;
																	}
																	else {
																		expression[0].op = '-';
																		expression[1].op = '-';
																		expression[2].op = '+';
																		if (compareExpression(expression, outputSet, startNumber)) {
																			return expression;
																		}
																		else {
																			expression[0].op = '-';
																			expression[1].op = '-';
																			expression[2].op = '-';
																			if (compareExpression(expression, outputSet, startNumber)) {
																				return expression;
																			}
																			else {
																				expression[0].op = '+';
																				expression[1].op = '+';
																				expression[2].op = '+';
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}

							}
						}

					}
				}
			}
		}
	}
	std::cout << "Failed to derive an expression.\n";
	return expression;
}

bool Expression::compareExpression(std::vector<term> expression, std::vector<int> outputSet, int startNumber) {
	for (int i = startNumber; i < startNumber + outputSet.size(); i++) { // Assume that the starting number is 0
		if (outputSet[i] != calculateExpression(expression, i))
			return false;
		viewExpression(expression, std::cout);
	}
	return true;
}

//void Expression::writeOutput(std::vector<int> outputSet, std::string fileName) throw (std::invalid_argument) {
//	std::ofstream outputFile("outputFile.txt", std::ios::in | std::ios::out | std::ios::app);
//	if (outputFile.is_open()) {
//		for (int i = 0; i < outputSet.size(); i++) {
//			i != outputSet.size() - OFFSET ? outputFile << outputSet[i] << "," : outputFile << outputSet[i];
//		}
//		outputFile << "\n";
//		outputFile.close();
//	}
//	else
//		throw std::invalid_argument("File " + fileName + " cannot be opened!\n");
//}

void Expression::writeExpressions(std::vector<term> expression, std::string fileName) throw (std::invalid_argument) {
		std::ofstream expressionFile(fileName, std::ios::in | std::ios::out | std::ios::app);
		if (expressionFile.is_open()) {
			int i = 0;
			for (term t : expression) {
				t.cons != 0 ? expressionFile << t.cons :
						t.coef == 1 && t.degree == 1 ? i == expression.size() - OFFSET ? expressionFile << "x" : expressionFile << "x" << t.op :
						t.coef == 1 ? i == expression.size() - OFFSET ? expressionFile << "x^" << t.degree : expressionFile << "x^" << t.degree << t.op :
						t.degree == 1 ? i == expression.size() - OFFSET ? expressionFile << t.coef << "x" : expressionFile << t.coef << "x" << t.op :
						i == expression.size() - OFFSET ? expressionFile << t.coef << "x^" << t.degree : expressionFile << t.coef << "x^" << t.degree << t.op;
					i++;
			}
			expressionFile << "\n";
			expressionFile.close();
		}
		else
			throw std::invalid_argument("File " + fileName + " cannot be opened!\n");
}

std::string Expression::getOutputFileName() { return outputFileName; }
std::string Expression::getExpressionFileName() { return expressionFileName; }

int main()
{
	Expression e;
	std::vector<Expression::term> expression;
	std::vector<int> outputSet;

	bool exit = false;
	while (!exit) {
		int command = 0;
		std::cout << "Enter a number to perform the following commands:\n1. Create an algebraic expression.\n2. Display the current expression.\n3. Create an output set.\n4. Display an output set.\n5. Save an output set to a file.\n6. Read an output set from file.\n7. Derive an expression from an output set.\n8. Save all expressions derived from output sets to a file.\n9. Exit.\n";
		std::cin >> command;

		switch (command)
		{
		case 1: // Create algebraic expresison
			expression = e.createExpression();
			std::cout << "Expression made.\n";
			break;
		case 2: // Display expression
			e.viewExpression(expression, std::cout);
			std::cout << "Expression displayed.\n";
			break;
		case 3: // Create output set from algebraic expression
			outputSet = e.createOutput(expression);
			std::cout << "Output set made.\n";
			break;
		case 4: // Display output set
			e.viewOutput(outputSet);
			std::cout << "Displayed output set.\n";
			break;
		case 5: // Save output set to file
			try {
				e.writeOutput(outputSet, e.getOutputFileName()); // TODO GETTER FOR FILE NAMES
				std::cout << "Output set saved to file\n";
			}
			catch (const std::invalid_argument& iae) {
				std::cout << "Unable to write data to file: " << iae.what() << "\n";
			}
			break;
		case 6: // Read output set from file
			try {
				outputSet = e.readOutput();
				std::cout << "Output set read in.\n";
			}
			catch (const std::invalid_argument& iae) {
				std::cout << "Unable to read data from file: " << iae.what() << "\n";
			}
			catch (const std::runtime_error& rte) {
				std::cout << "Unable to read line from file: " << rte.what() << "\n";
			}
			break;
		case 7: // Derive an expression from an output set
			expression = e.deriveExpression(outputSet);
			break;
		case 8: // Save all expressions derived from output sets to file
		{
			int noOfExpressions = 0;
			std::cout << "Enter the amount of output sets to be derived and written to in the file: ";
			std::cin >> noOfExpressions;
			for (int i = 0; i <= noOfExpressions; i++) {
				e.writeExpressions(e.deriveExpression(e.readOutput()), e.getExpressionFileName());
			}
			break;
		}	
		case 9: // Exit interface
			std::cout << "Exiting...";
			return 0;
		default:
			std::cout << "Invalid input provided. Please enter a valid number.\n";
			break;
		}
	}
	return 0;
}