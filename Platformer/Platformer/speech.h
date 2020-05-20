#include  <iostream>
using namespace std;

int currentlevel(int x){
	int level = 0;
	if ((x > 0) && (x < 800)) { level = 0; }
	if ((x > 800) && (x <1600)) { level = 1; }
	if ((x > 1600)&& (x<2400)) { level = 2; }
	if ((x > 2400) && (x < 6450)) { level = 3; }
	return level;


}

string textlevel(int currentlevel) {
	string leveltext = "";
	switch (currentlevel)
	{
	case 0: leveltext = "Hello!\nWelcome\nto my\ngame!"; break;

	case 1: leveltext = "Do not\n touch \nwater!"; break;

	case 2: leveltext = "Collect\ncoins!"; break;
	
	case 3: leveltext = "Reach\nfinish\nLook af-\nter\nhealth!"; break;
	}

	return leveltext;
}