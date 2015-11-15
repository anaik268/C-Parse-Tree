#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>

#include "program2.h"
using namespace std;


string getName(istream *);
string getParents(istream *);
void nestedClass(istream *);
int checkClass(istream *);

int lBrace = 0;
int parseError = 0;
string saveClass = "";
TokenType saveToken = T_NONE;
int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		cout << "You entered to many arguments" << endl;
		exit(0);
	}

	if (argc == 1)
	{
		cout << "you entered no command line arguments" << endl;
		exit(0);
	}
	string recognized;

	ifstream myFile;
	myFile.open(argv[1]);
	if (!myFile.is_open())									//check if it opened, if not then
	{
		cout << "File does not exist" << endl;				//file doesnt exist and exit

		return 0;
	}
	TokenType cur = T_NONE;
	string parseString = ""; //cout at end the whole parse string

	while (cur != T_EOF)
	{
		parseString = "";
		cur = getToken(&myFile, recognized);
		if (cur == T_CLASS)
		{
			::parseError++;
			checkClass(&myFile); //look into the class details
		}

		if (getToken(&myFile, recognized) == T_LEFTBRACE)
			nestedClass(&myFile);
		//cout << cur << " --> " << recognized << endl;
	}

	if (::parseError == 0)
		cout << "0 parse error." << endl;

	system("pause");

	return 0;

}

string getName(istream* in)
{
	string name = "";
	TokenType temp = getToken(in, name);
	return name;
}

string getParents(istream* in)
{
	string parentName = "";
	string classType = " public";
	string name = "";
	TokenType parent = getToken(in, name);
	if (parent == T_PRIVATE || parent == T_PROTECTED)
	{
		classType = " " + name;
		parent = getToken(in, name);
		parentName += "Parent " + name + classType;
		//if (getToken(in, name) == T_C)
	}
	else
		parentName += "Parent " + name + classType;


	return parentName;
}

void nestedClass(istream* in)
{
	string recognized;
	while (::lBrace >= 1 || !in->eof())
	{
		TokenType lookForClass = getToken(in, recognized);
		if (lookForClass == T_RIGHTBRACE)
		{
			//cout << ::lBrace << endl;
			::lBrace -= 1;
			if (::lBrace == 0)
				break;
		}

		else if (lookForClass == T_LEFTBRACE)
		{
			//cout << ::lBrace << endl;
			::lBrace += 1;
		}
		else if (lookForClass == T_CLASS)
		{
			string nameOfClass = getName(in);
			cout << "class " << nameOfClass << " nested-in " << saveClass << endl;
			saveClass = nameOfClass;
		}


	}
}

int checkClass(istream* in)
{
	string parseString = "";
	string recognized = "";
	string nameOfClass = getName(in);
	saveClass = nameOfClass;
	parseString += "class " + nameOfClass;
	TokenType nextToken = getToken(in, recognized);
	if (nextToken == T_SEMICOLON)
	{
		return 0;
	}

	if (nextToken == T_COLON)
	{
		parseString += " " + getParents(in);
		nextToken = getToken(in, recognized);
		while (nextToken == T_COMMA)
		{
			parseString += " " + getParents(in);
			nextToken = getToken(in, recognized);
		}
	}
	cout << parseString << endl;

	if (nextToken == T_LEFTBRACE)
	{
		::lBrace = 1;
		nestedClass(in);
	}
	return 0;
}
