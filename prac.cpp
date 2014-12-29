#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <queue>


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

vector <int> padres;


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

bool bfs(Grafo&  rg, int s, int t){
    vector <bool> visited(g.size(),false);
    queue <int> q;
    q.push(s);
    visited[s] = true;
    padres[s] = -1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for (int i = 0; i < g.size(); ++i)
        {
            if(!visited[i] and rg[u][i] > 0){
                q.push(i);
                padres[i] = u;
                visited[i] = true;
            }
        }
    }
    return (visited[t] == true);
}

int edmonsKarp(){
    int maxFlow = 0;
    padres = vector <int> (g.size());
    Grafo rg = g;
    int s = g.size()-2;
    int t = g.size()-1;
    int path_flow = 99999;
    int cont = 0;
    while(bfs(rg, g.size()-2, g.size()-1)){
        for (int i = t; i != s; i = padres[i])
        {
            int u = padres[i];
            path_flow = min(path_flow,rg[u][i]);            
        }
        for (int i = t; i != s; i = padres[i])
        {
            int u = padres[i];
            rg[u][i] -= path_flow;
            rg[i][u] += path_flow;
        }
        maxFlow += path_flow;
        cout << path_flow << endl;
    }
    return maxFlow;
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
    cout << "FLOOOOOW  " << maxFlow << endl;  
}