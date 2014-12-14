//	RPNCalc.h
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <list>
#include <string>
#include <new>
#include <sstream>
#include <stack>

#ifndef RPNCALC_H
#define RPNCALC_H

using namespace std;
const char helpMenu[] = 
	"C clear stack   | CE clear entry  | D rotate down  | F save program to file\n"
	"G0-G9 get reg n | H help on/off   | L load program | M +/- | P program on/off\n"
	"R run program   | S0-S9 set reg n | U rotate up    | X exit\n";

const char line[] =	
	"_____________________________________________________________________________\n";

const unsigned short FAIL = -1;
const unsigned short ZERO = 0;
const unsigned short ONE = 1;
const unsigned short NUMREGS = 10;
const unsigned short BUFFER_SIZE = 256;
const char P_CHAR = 'P';

enum _operator {
    InvalidOp,
    Add,
    Subtract,
    Multiply,
    Divide,
    Mod,
    Exp,
	Negate,
	Positive
};

enum _command {
	InvalidCmd,
	Clear,
	RotateDown,
	SaveToFile,
	GetReg,
	HelpToggle,
	LoadProgram,
	Neg,
	RecordProgram,
	RunProgram,
	SetReg,
	RotateUp,
	Exit
};

class CRPNCalc {
public:
	CRPNCalc(bool on = true) : m_on(on), m_helpOn(true), m_error(false) {if (m_on){run();}}
	void run();
	void print(ostream &sout);
	void input(istream &sin);

private:
	void add();
	unsigned checkRegValue();
	void clearEntry();
	void clearAll();
	void divide();
	void error();
	void exp();
	void getReg(unsigned reg);
	bool isCommand(_command &cmd);
	bool isNumber();
	bool isOperator(_operator &op);
	void loadProgram();
	void mod();
	void multiply();
	void neg();
	void negate();
	void parse();
	void performCommand(_command cmd);
	void performOperation(_operator op);
	void pos();
	void printMenu(ostream &sout);
	void processNumber(double &number);
	double popFront();
	void pushFront(double value);
	void recordProgram();
	void reportAnswer();
	void rotateDown();
	void rotateUp();
	void runProgram();
	void saveToFile();
	void setReg(unsigned reg);
	void subtract();

	double m_registers[NUMREGS];
	string m_buffer;
	deque<double> m_stack;
	list<string> m_program;
	istringstream m_instrStream;
	bool m_error;
	bool m_helpOn;
	bool m_on;
};

ostream &operator<<(ostream &sout, CRPNCalc &calc);
istream &operator>>(istream &sin, CRPNCalc &calc);
istream &skipWhiteSpace(istream &sin);


#endif