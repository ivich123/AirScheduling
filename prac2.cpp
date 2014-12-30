#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits.h>

using namespace std;

struct trayecto{
	int IDor;
	int IDdest;
	int hsal;
	int hlleg;
};


vector<trayecto> trayectos;

//first flow, second capacity
typedef pair <int, int> fc;


typedef vector <vector <fc> > Grafo;

Grafo g;

vector <int> padres;


void buildGraph(){
	for (int i = 0; i < trayectos.size(); ++i)
	{
		++g[trayectos[i].IDor][trayectos[i].IDdest].first;
		++g[trayectos[i].IDor][trayectos[i].IDdest].second;

		//size-2 = nodo s;
		g[g.size()-2][trayectos[i].IDor].first = 0;
		g[g.size()-2][trayectos[i].IDor].second = 1;

		//size -1 = nodo t;
		g[trayectos[i].IDdest][g.size()-1].first = 0;
		g[trayectos[i].IDdest][g.size()-1].second = 1;
	}
	for (int i = 0; i < trayectos.size(); ++i)
	{
		for (int j = 0; j < trayectos.size(); ++j)
		{
			if((trayectos[i].IDdest == trayectos[j].IDor) and trayectos[j].hsal - trayectos[i].hlleg >= 15){
				 g[trayectos[i].IDdest][trayectos[j].IDor].first= 0;
				 g[trayectos[i].IDdest][trayectos[j].IDor].second= 1;
			}
		}
	}
}

void escriu(Grafo g){
	for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g.size(); ++j) {
            cout << "["<< g[i][j].first << "," << g[i][j].second << "]";
        }
        cout << endl;
    }
}


bool bfs(Grafo rg, int s, int t){
	vector <bool> visited(g.size(),false);
	queue <int> q;
	q.push(s);
	visited[s] = true;
	padres[s] = -1;
	while(!q.empty()){
		int u = q.front();
		q.pop();
		for (int i = 0; i < rg.size(); ++i)
		{
			if((visited[i] == false) and (rg[u][i].first <= rg[u][i].second)){
				q.push(i);
				padres[i] = u;
				visited[i] = true;

			}
		}
	}
	return(visited[t] == true);
}

void escriupares(){
	for (int i = 0; i < padres.size(); ++i)
	{
		cout << padres[i] << " ";
	}
	cout << endl;
}

int edmonsKarp(){
	int maxFlow = 0;
	Grafo rg = g;
	int t = g.size()-1;
	int s = g.size()-2;
	while(bfs(rg,g.size()-2,g.size()-1)){
		int path_flow = INT_MAX;
		for (int i = t; i != s; i = padres[i])
		{
			int u = padres[i];
			path_flow = min(path_flow, rg[u][i].first);
		}
		for (int i = t; i != s; i = padres[i])
		{
			int u = padres[i];
			rg[u][i].first -= path_flow;
            rg[i][u].first += path_flow;
		}
		//escriupares();
		maxFlow += path_flow;
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
    
    g = Grafo(max*2+2,vector<fc>(max*2+2));
    padres = vector <int> (g.size());
    buildGraph();
    //escriu(g);
    int maxFlow = edmonsKarp();
    //escriu(g);
    //cout << "FLOOOOOW  " << maxFlow << endl;  
}


/**/
/**/
/**/
/*        for (int i = t; i != s; i = padres[i])
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
        //escriu(rg);
        maxFlow += path_flow;
        cout << path_flow << endl;
    }*/