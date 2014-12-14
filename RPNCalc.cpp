//	RPNCalc.cpp
#include "RPNCalc.h"

void CRPNCalc::run()
{
	do {
		system("cls");
		print(cout);
		input(cin);
	} while (m_on);
}

void CRPNCalc::print(ostream &sout)
{
	printMenu(sout);
	if (!m_error) {
		reportAnswer();
	} else {
		error();
	}
	cout << endl;
}

void CRPNCalc::input(istream &sin)
{
	m_buffer.clear();
	m_instrStream.clear();
	getline(sin, m_buffer);
	m_instrStream.str(m_buffer);
	parse();
}

void CRPNCalc::parse()
{
	do {
		double num = NULL;
		_operator op;
		_command cmd;

		if (m_error) {
			break;
		}

		skipWhiteSpace(m_instrStream);
        if (isOperator(op)) {
 			performOperation(op);
        } else if (isCommand(cmd)) {
			performCommand(cmd);
		} else if (isNumber()) {
			processNumber(num);
		} else if (!m_instrStream.eof()) {
			m_error = true;
			break;
		}

		skipWhiteSpace(m_instrStream);
	} while (!m_instrStream.eof());
}

bool CRPNCalc::isNumber() 
{
	if (isdigit(m_instrStream.peek())) {
		return(true);
	}
	return(false);
}

inline void CRPNCalc::processNumber(double &number)
{
	m_instrStream >> number;
	m_stack.push_front(number);
}

bool CRPNCalc::isOperator(_operator &op) 
{
    op = InvalidOp;
    char token = m_instrStream.get();

    switch(token) {
		case '+':
			if (isNumber()) {
				op = Positive;
			} else {
				op = Add; 
			}
            break;
		case '-':
			if (isNumber()) {
				op = Negate;
			} else {
				op = Subtract;
			}
            break;
		case '*':
            op = Multiply;
            break;
		case '/':
            op = Divide;
            break;
		case '%':
            op = Mod;
            break;
		case '^':
            op = Exp;
            break;
    }

    if (op == InvalidOp) {
		m_instrStream.putback(token);
		return(false);
    }
    return(true);
}

void CRPNCalc::performOperation(_operator op)
{
	try {
		// check for operator with stack of one or less
		if (m_stack.size() <= ONE && op != Negate)  {
			// not enough operands for binary operator
			if (op != Positive) {
				throw exception();
			}
		} 

		switch (op) {
			case Add:
				add();
				break;
			case Subtract:
				subtract();
				break;
			case Multiply:
				multiply();
				break;
			case Divide:
				divide();
				break;
			case Mod:
				mod();
				break;
			case Exp:
				exp();
				break;
			case Negate:
				negate();
				break;
			case Positive:
				pos();
				break;
		}

	} catch (exception e) {
		m_error = true;
	}
}

bool CRPNCalc::isCommand(_command &cmd)
{
	cmd = InvalidCmd;
	char token = m_instrStream.peek();

	switch(toupper(token)) {
		case 'C':
			cmd = Clear;
			break;
		case 'D':
			cmd = RotateDown;
			break;
		case 'F':
			cmd = SaveToFile;
			break;
		case 'G':
			cmd = GetReg;
			break;
		case 'H':
			cmd = HelpToggle;
			break;
		case 'L':
			cmd = LoadProgram;
			break;
		case 'M':
			cmd = Neg;
			break;
		case 'P':
			cmd = RecordProgram;
			break;
		case 'R':
			cmd = RunProgram;
			break;
		case 'S':
			cmd = SetReg;
			break;
		case 'U':
			cmd = RotateUp;
			break;
		case 'X':
			cmd = Exit;
			break;
	}

	if (cmd != InvalidCmd) {
		m_instrStream >> token;
		return(true);
	}
	return(false);
}

void CRPNCalc::performCommand(_command cmd)
{
	switch(toupper(cmd)) {
		case Clear:
			if (toupper(m_instrStream.peek()) == 'E') {
				clearEntry();
			} else {
				clearAll();
			}
			break;
		case RotateDown:
			rotateDown();
			break;
		case SaveToFile:
			saveToFile();
			break;
		case GetReg: {
			int temp = checkRegValue();
			if (temp != FAIL) {
				getReg(temp);
			}
			break;
		}
		case HelpToggle:
			m_helpOn = !m_helpOn;
			break;
		case LoadProgram:
			loadProgram();
			break;
		case Neg:
			neg();
			break;
		case RecordProgram:
			recordProgram();
			break;
		case RunProgram:
			runProgram();
			break;
		case SetReg: {
			int temp = checkRegValue();
			if (temp != FAIL) {
				setReg(temp);
			}
			break;
		}
		case RotateUp:
			rotateUp();
			break;
		case Exit:
			m_on = false;
			break;
    }
}

inline double CRPNCalc::popFront()
{
    double value = m_stack.front();
    m_stack.pop_front();
    return(value);
}

inline void CRPNCalc::pushFront(double value)
{
		m_stack.push_front(value);
}

inline void CRPNCalc::add()
{
	double y = popFront();	// gets m_stack[0]
	double x = popFront();	// gets m_stack[1]
    pushFront(x + y);
}

inline void CRPNCalc::subtract()
{
	double y = popFront();	// gets m_stack[0]
	double x = popFront();	// gets m_stack[1]
    pushFront(x - y);
}

inline void CRPNCalc::multiply()
{
	double y = popFront();	// gets m_stack[0]
	double x = popFront();	// gets m_stack[1]
    pushFront(x * y);
}

void CRPNCalc::divide()
{
	double y = popFront();	// gets m_stack[0]
	double x = popFront();	// gets m_stack[1]
	if (y == ZERO) {
		m_error = true;
	} else {
	    pushFront(x / y);
	}
}
	
inline void CRPNCalc::mod()
{
	double y = popFront();	// gets m_stack[0]
	double x = popFront();	// gets m_stack[1]
	pushFront(fmod(x, y));
}

inline void CRPNCalc::exp()
{
	double y = popFront();	// gets m_stack[0]
	double x = popFront();	// gets m_stack[1]
	pushFront(pow(x, y));
}

void CRPNCalc::negate()
{
	double x = NULL;
	m_instrStream >> x;
	x *= -1;
	m_stack.push_front(x);
}

inline void CRPNCalc::pos()
{
	double x = NULL;
	m_instrStream >> x;
	m_stack.push_front(x);
}

inline void CRPNCalc::clearEntry()
{
	m_instrStream.get();
	if (m_stack.size() > ZERO) {
		m_stack.pop_front();
	}
}

inline void CRPNCalc::clearAll()
{
	m_stack.clear();
	m_instrStream.str("");
}

void CRPNCalc::rotateDown()
{
	if (m_stack.size() < ONE) {
		m_error = true;
	} else {
		double temp = m_stack.front();
		m_stack.pop_front();
		m_stack.push_back(temp);
	}
}

void CRPNCalc::rotateUp()
{
	if (m_stack.size() < ONE) {
		m_error = true;
	} else {
		double temp = m_stack.back();
		m_stack.pop_back();
		m_stack.push_front(temp);
	}
}

void CRPNCalc::saveToFile()
{
	list<string> temp = m_program;
	string filename;
	cout << "Enter filename (no extension): ";
	cin >> filename;
	filename += ".txt";
	ofstream fileOut(filename);
	if (fileOut.is_open()) {
		if (temp.size() > ZERO) {
			unsigned j = temp.size();
			for (unsigned i = 0; i < j; i++) {
				fileOut << temp.back() << endl;
				temp.pop_back();
			}
		}
	} else {
		m_error = true;
	}
	fileOut.close();
}

void CRPNCalc::reportAnswer() 
{
	if (m_stack.size() == ZERO) {
		cout << endl;
	} else {
		m_stack.front();
		cout << setprecision(10) << m_stack.front() << endl;
	}
}

inline void CRPNCalc::error()
{
	cout << "<< ERROR >>" << endl;
	m_error = false;
}

void CRPNCalc::printMenu(ostream &sout)
{
	sout << " Reverse Polish Notation Calculator\t By: MeIsVox" << endl;
	sout << line;
	if (m_helpOn) {
		sout << helpMenu;
	} else {
		sout << endl << endl << endl;
	}
	sout << line;
}

void CRPNCalc::loadProgram()
{
	ifstream fileIn;
	string filename;
	string contents;
	cout << "Enter the filename: ";
	cin >> filename;
	fileIn.open(filename);
	if (fileIn.is_open()) {
		while (fileIn.good()) {
			getline(fileIn, contents);
			m_program.push_front(contents);
		}
	} else {
		m_error = true;
	}
	fileIn.close();
}

void CRPNCalc::neg()
{
	if (m_stack.size() > ZERO) {
		m_stack.front() *= -1;
	} else {
		m_error = true;
	}
}

void CRPNCalc::recordProgram()
{
	m_instrStream.str("");
	m_program.clear();
	int i = 0;
	char ch = NULL;
	string::iterator sit;
	string record;
	string temp;
	do {
		cout << i << "> ";
		getline(cin, record);
		for (sit = record.begin(); sit != record.end(); sit++) {
			if (toupper(*sit) == P_CHAR) {
				ch = *sit;
				break;
			}
			temp += *sit;
		}
		temp += " ";
		i++;
	} while (toupper(ch) != P_CHAR);
	m_program.push_front(temp);
}

inline void CRPNCalc::runProgram() 
{
	istringstream progIn;
	progIn.str(m_program.front());
	input(progIn);
}

istream &skipWhiteSpace(istream &sin)
{
	char ch;
	while (isspace(ch = sin.peek()) && ch != '\n') {
		ch = sin.get();
	}
	return(sin);
}

unsigned CRPNCalc::checkRegValue()
{
	unsigned num = NULL;

	if (isNumber()) {
		m_instrStream >> num;
		if (num >= 0 && num < 10) { 
			return(num);
		}
	}
	m_error = true;
	return(FAIL);
}

void CRPNCalc::setReg(unsigned reg) 
{
	if (m_stack.size() > ZERO) {
		m_registers[reg] = m_stack.front();
	} else {
		m_error = true;
	}
}

inline void CRPNCalc::getReg(unsigned reg)
{
	pushFront(m_registers[reg]);
}

ostream &operator<<(ostream &sout, CRPNCalc &calc)
{
	calc.print(sout);
	return(sout);
}
