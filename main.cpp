#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace std;

int main()
{

	cout << "Working fine" << endl;

	fstream file;
	file.open("test.txt", ios::out); // open file
	if (file.is_open())
	{
		file << "This is a sample content for the file \n";
		file.close();
	}

	file.open("test.txt", ios::in);
	if (file.is_open())
	{
		string content;
		while (getline(file, content))
		{
			cout << content << endl;
		}
		file.close();
	}
	return 0;
}
