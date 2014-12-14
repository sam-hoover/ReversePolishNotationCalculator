#include "RPNCalc.h"
int main()
{
	try {
		CRPNCalc gameOn;
	} catch (runtime_error e) {
		cerr << "Error:\t" << e.what() << endl;
	} catch (exception e) {
		cerr << "Error:\t" << e.what() << endl;
	}
	cout << "Press any key to exit." << endl;
	cin.get();
	return(0);
}