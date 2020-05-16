#include  <iostream>
using namespace std;

int currentlevel(int x){
	int level = 0;
	if ((x > 0) && (x < 400)) { level = 0; }
	if ((x > 400) && (x < 800)) { level = 1; }
	if ((x > 800)) { level = 2; }
	return level;


}

string textlevel(int currentlevel) {
	string leveltext = "";
	switch (currentlevel)
	{
	case 0: leveltext = "Hello!\nWelcome\nto my\ngame!"; break;

	case 1: leveltext = "First\nlevel,\neasy\none!"; break;

	case 2: leveltext = "Haven't\nadded\nyet :c"; break;
	}
	return leveltext;
}