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

typedef vector <vector <int> > Grafo;

Grafo g;

vector <int> padres;

void buildGraph() {
    for (int i = 0; i < trayectos.size(); ++i) {
        g[2*i][2*i+1] = 1;
        g[2*i+1][2*i] = -1;
        //nodo s
        g[g.size()-2][2*i] = 1;
        g[2*i][g.size()-2] = -1;
        //nodo t
        g[2*i+1][g.size()-1]=1;
        g[g.size()-1][2*i+1] = -1;
    }
    for (int i = 0; i < trayectos.size(); ++i) {
        for (int j = 0; j < trayectos.size(); ++j) {
            if (trayectos[i].IDdest == trayectos[j].IDor) {
                if (trayectos[j].hsal - trayectos[i].hlleg >= 15) {
                    g[2*j][2*i+1] = 1;
                    g[2*i+1][2*j] = -1;
                }
            }
        }
    }
}

void escriu(Grafo g) {
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g.size(); ++j) {
            cout << g[i][j] << " " ;
        }
        cout << endl;
    }
}

bool bfs(Grafo&  rg, int s, int t) {
    vector <bool> visited(g.size(), false);
    queue <int> q;
    q.push(s);
    visited[s] = true;
    padres[s] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < g.size(); ++i) {
            if (!visited[i] and rg[u][i] > 0) {
                q.push(i);
                padres[i] = u;
                visited[i] = true;
            }
        }
    }
    return (visited[t] == true);
}


bool bfsFinal(Grafo&  rg, int s, int t) {
    vector <bool> visited(g.size(), false);
    queue <int> q;
    q.push(s);
    visited[s] = true;
    padres[s] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < g.size(); ++i) {
            if (!visited[i]) {
                q.push(i);
                padres[i] = u;
                visited[i] = true;
            }
        }
    }
    return (visited[t] == true);
}

void escriupares() {
    for (int i = 0; i < padres.size(); ++i) {
        cout << padres[i] << " ";
    }
    cout << endl;
}

int edmonsKarp() {
    int maxFlow = 0;
    padres = vector <int> (g.size());
    Grafo rg = g;
    int s = g.size()-2;
    int t = g.size()-1;
    int path_flow = INT_MAX;
    int cont = 0;
    int k = 0;
    while(bfs(rg, g.size()-2, g.size()-1)){
        for (int i = t; i != s; i = padres[i]) {
            int u = padres[i];
            path_flow = min(path_flow, rg[u][i]);
        }
        for (int i = t; i != s; i = padres[i]) {
            int u = padres[i];
            rg[u][i] -= path_flow;
            rg[i][u] += path_flow;
        }
        maxFlow += path_flow;
    }
    // buscar s, t -> tirar bfs
    for (int i = rg.size()-1; i > 0; --i) {
        bool trobat = false;
        for (int j = rg.size()-1; j > 0; --j) {
            if (rg[i][j] == -1) trobat = true;
            else if (rg[i][j] == 1) trobat = false;
        }
        if (trobat) {
            s = i;
            t = i + 1;
            break;
        }
    }
    bfsFinal(rg, s, t);
    for (int i = t; i != s; i = padres[i]) {
        int u = padres[i];
        // cout << "aaa " << u << " " << i << endl;
        k += rg[u][i];
    }
    // escriu(rg);
    cout << "La k es: " << k << endl;
    return maxFlow;
}

int main() {
	//std::ifstream file("instance_100_2_1.air");
	std::string str;
    int max = 0;
    int orig, dest, hs, hll;
	while (cin >> orig >> dest >> hs >> hll) {
        trayecto t;
    	t.IDor = orig;
    	t.IDdest = dest;
    	t.hsal = hs;
    	t.hlleg = hll;
        trayectos.push_back(t);
        ++max;

    }
    g = Grafo(max*2+2,vector<int>(max*2+2,0));
    buildGraph();
    int maxFlow = edmonsKarp();
    cout << "El max flow es: " << maxFlow << endl;
    // escriu(g);
}