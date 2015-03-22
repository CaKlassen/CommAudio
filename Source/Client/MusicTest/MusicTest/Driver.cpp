#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "Music.h"

using std::cout;
using std::cerr;
using std::endl;

int main(void)
{
	if (!MCIPlayLocalFile("test.wav"))
	{
		cerr << "Failed to play file" << endl;
	}

	getchar();

	return 0;
}