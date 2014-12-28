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

vector<trayecto> trayectos;

typedef vector <vector <int> > Grafo;

Grafo g;


void buildGraph(){
    for (int i = 0; i < trayectos.size(); ++i) {
        ++g[trayectos[i].IDor][trayectos[i].IDdest];
        //nodo S
        g[g.size()-2][trayectos[i].IDor] = 1;
        //nodo T
        g[trayectos[i].IDdest][g.size()-1] = 1;
    }

    for (int i = 0; i < trayectos.size(); ++i) {
        for (int j = 0; j < trayectos.size(); ++j) {
            if(i != j and trayectos[j].hsal - trayectos[i].hlleg >= 15) {
                //cout<<trayectos[i].hlleg<< " " << trayectos[j].hsal << " " << endl;
                //cout << "desti = " << trayectos[i].IDdest << "  orig = " << trayectos[j].IDor << endl;
                ++g[trayectos[i].IDdest][trayectos[j].IDor];
            }
        }
    }
}

void escriu(){
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g.size(); ++j) {
            cout << g[i][j] << " " ;
        }
        cout << endl;
    }
}

int edmonsKarp(){

}

int main(){
	std::ifstream file("test.txt");
	std::string str;
    int max = 0;
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
        ++max;
    }
    g = Grafo(max*2+2,vector<int>(max*2+2,0));
    buildGraph();
    int maxFlow = edmonsKarp();
    escriu();
}