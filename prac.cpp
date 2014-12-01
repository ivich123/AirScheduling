#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace std;


struct trayecto{
	int IDor;
	int IDdest;
	int hsal;
	int hlleg;
};

vector <trayecto> trayectos;

/*
void escribeTRayectos(){
	for (int i = 0; i < trayectos.size(); ++i)
	{
		cout << trayectos[i].IDor << " " << trayectos[i].IDdest << " " << trayectos[i].hsal << " " << trayectos[i].hlleg << endl;
	}

}*/

int main(){
	std::ifstream file("instance_100_2_1.air");
	std::string str;
	while (std::getline(file, str))
    {
    	istringstream iss;
    	iss.str (str);
    	trayecto t;
    	int val;
    	iss >> val;
    	t.IDor = val;
    	iss >> val;
    	t.IDdest = val;
    	iss >> val;
    	t.hsal = val;
    	iss >> val;
    	t.hlleg = val;
    	trayectos.push_back(t);
    }
    //escribeTRayectos();
}