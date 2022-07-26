#include "UniLink.h"
const string SERVER = "";
const string NAME = "";
const string PASSWORD = "";
int main()
{
	UniLink U(SERVER, NAME, PASSWORD);
	U.run();
	return 0;
}
