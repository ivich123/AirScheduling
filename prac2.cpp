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

struct edge {
	int lb;
	int cap;

};


typedef vector <vector <edge> > Grafo;
typedef vector <vector <int> > gflow;
typedef vector <pair<int, int> > viaje;
typedef vector <int> vec;
vector <viaje> viajes;
vector <int> demands;

void writDemands(vector <int> t) {
	for (int i = 0; i < t.size(); ++i) {
		cout << t[i] << " ";
	}
	cout << endl;
}

void writegraph(gflow g2) {
	for (int i = 0; i < g2.size(); ++i) {
		for (int j = 0; j < g2.size(); ++j) {
			cout << g2[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


void escribe (Grafo g) {
	for (int i = 0; i < g.size(); ++i) {
		for (int j = 0; j < g.size(); ++j) {
			cout << "[" << g[i][j].lb << "," << g[i][j].cap << "]" << " ";
		}
		cout << endl;
	}
	cout << endl;
}


void buildGraph(Grafo& g, vector<trayecto> trayectos) {
	// vamos a crear el grafo con demandas para despues transformarlo en uno de flujos.
	for (int i = 0; i < trayectos.size(); ++i) {
		g[2*i][2*i+1].lb = 1;
		g[2*i][2*i+1].cap = 1;
        //g[2*i+1][2*i].lb = -1;
        //g[2*i+1][2*i].cap = -1;
        //nodo s;
        g[g.size()-4][2*i].lb = 0;
        g[g.size()-4][2*i].cap = 1;
        //g[2*i][g.size()-2] = -1;
        //nodo t;
        g[2*i+1][g.size()-3].lb=0;
        g[2*i+1][g.size()-3].cap=1;
        //g[g.size()-1][2*i+1] = -1;
        //arista s t con capacidad k y lb 0;
        g[g.size()-4][g.size()-3].lb = 0;
        g[g.size()-4][g.size()-3].lb = trayectos.size();


        //arista de s' a s
        g[g.size()-4][g.size()-2].lb = trayectos.size();
        //arista de t a t'
        g[g.size()-3][g.size()-1].lb = trayectos.size();
	}
	//vamos a conectar los vuelos que cumplan nuestras restricciones y les pondremos lb = 0, y cap = 1;
	for (int i = 0; i < trayectos.size(); ++i) {
		for (int j = 0; j < trayectos.size(); ++j) {
			if (trayectos[i].IDdest == trayectos[j].IDor) {
                if (trayectos[j].hsal - trayectos[i].hlleg >= 15) {
                    g[2*j][2*i+1].lb = 0;
                    g[2*j][2*i+1].cap = 1;
                    //g[2*i+1][2*j] = -1;
                }
            }
		}
	}
}


gflow residualGraph(Grafo g, vector<trayecto> t, vector <int>& demands) {
	gflow g2 = vector<vector <int> > (g.size(), vector<int>(g.size()));
	int j = 0;
	for (int i = 0; i < t.size(); ++i) {
		demands[j] -=  g[2*i][2*i+1].lb;
		demands[j+1] +=  g[2*i][2*i+1].lb;
		j+=2;
	}
	for (int i = 0; i < g.size(); ++i) {
		for (int j = 0; j < g.size(); ++j) {
			if (g[i][j].cap != 0) {
                g2[i][j] = g[i][j].cap - g[i][j].lb;
            }
		}
	}
	return g2;
}

bool bfs(gflow&  rg, int s, int t, vector <int>& padres) {
    vector <bool> visited(rg.size(), false);
    queue <int> q;
    q.push(s);
    visited[s] = true;
    padres[s] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < rg.size(); ++i) {
            if (!visited[i] and rg[u][i] > 0) {
                q.push(i);
                padres[i] = u;
                visited[i] = true;
            }
        }
    }
    return (visited[t] == true);
}

int edmonsKarp(gflow g, vector<int>& padres) {
    int maxFlow = 0;
    gflow rg = g;
    int s = g.size()-4;
    int t = g.size()-3;
    int path_flow = INT_MAX;
    int cont = 0;
    int k = 0;
    while (bfs(rg, g.size()-4, g.size()-3, padres)) {
        for (int i = t; i != s; i = padres[i]) {
            int u = padres[i];
            path_flow = min(path_flow, rg[u][i]);
            rg[u][i] -= path_flow;
            rg[i][u] += path_flow;
        }
        bfs(rg, g.size()-4, g.size()-3, padres);
        for (int i = t; i != s; i = padres[i]) {
            int u = padres[i];
            int orig = (i / 2) + 1;
            int dest = (u / 2) + 1;
            int numViajes = (rg.size() - 4) / 2;
            if (rg[u][i] > 0 and orig <= numViajes and dest <= numViajes) {
                if (viajes.size() > 0) {
                    viaje v = viajes[viajes.size()-1];
                    if (orig == v[v.size()-1].second) {
                        pair<int, int> p;
                        p.first = orig;
                        p.second = dest;
                        viajes[viajes.size()-1].push_back(p);
                    }
                    else {
                        pair<int, int> p;
                        p.first = orig;
                        p.second = dest;
                        viaje v;
                        v.push_back(p);
                        viajes.push_back(v);
                    }
                }
                else {
                    pair<int, int> p;
                    p.first = orig;
                    p.second = dest;
                    viaje v;
                    v.push_back(p);
                    viajes.push_back(v);
                }
                // viajes.push_back(tr);
            }
        }
        maxFlow += path_flow;
    }

    return maxFlow;
}

int calcularMinPilotos(int n) {
    return (demands[demands.size()-3] - n) - 1;
}

bool existeViaje(vec& tra, int ori, int des) {
    for (int i = 0; i < tra.size(); ++i) {
        if (tra[i] == ori or tra[i] == des) return true;
    }
    return false;
}

void calcularTrayectos(int minPilotos) {
    vector <vec> tray;
    for (int i = 0; i < minPilotos; ++i) {
        viaje v = viajes[i];
        vec tra;
        for (int j = v.size() - 1; j >= 0; --j) {
            if (not existeViaje(tra, v[j].first, v[j].second)) {
                tra.push_back(v[j].first);
                tra.push_back(v[j].second);
                cout << v[j].first << " " << v[j].second << " ";
            }
        }
        tray.push_back(tra);
        cout << endl;
    }
}

int main(){
    int max = 0;
    int orig, dest, hs, hll;
    Grafo g;
    vector<trayecto> trayectos;
    vector <int> padres;
    while (cin >> orig >> dest >> hs >> hll) {
        trayecto t;
        t.IDor = orig;
        t.IDdest = dest;
        t.hsal = hs;
        t.hlleg = hll;
        trayectos.push_back(t);
        ++max;
    }
    g = Grafo(max*2+4,vector<edge>(max*2+4));
    demands = vector <int> (max*2+2,0);
    padres = vector <int> (g.size(),0);
    //demanda de s = -k
    demands[demands.size()-4] = -trayectos.size();
    //demanda de t = k;
    demands[demands.size()-3] = trayectos.size();
    buildGraph(g, trayectos);
    //g2 sera el grafo resultante de aplicar las tranformaciones para quitar las cotas.
    //demands quedara modificado con la tranformacion del nuevo grafo de flujos.
    gflow g2;
    g2 = residualGraph(g, trayectos, demands);
    int n = edmonsKarp(g2, padres);
    int minPilotos = calcularMinPilotos(n);
    calcularTrayectos(minPilotos);
    // writDemands(demands);
    cout << "resultat " << minPilotos  << endl;


    //writegraph(g2);
    //escribe(g);
    //escribe(g);
    //int maxFlow = edmonsKarp();
    // escribe(g);
    //cout << "FLOOOOOW  " << maxFlow << endl;
}
