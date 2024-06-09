#include <bits/stdc++.h>

using namespace std;

void leituraGrafo(vector<vector<int>> &, int);
void escritaGrafo(vector<vector<int>> &);
bool grafobipartido(vector<vector<int>> &, vector<int> &);
bool bfs(vector<vector<int>> &, int, int, vector<int> &);
int fordFulkerson(vector<vector<int>> &, int, int);
int emparelhamentoMaximo(vector<vector<int>> &, int, vector<int> &);

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> Grafo(n);
    leituraGrafo(Grafo, m);

    escritaGrafo(Grafo);

    cout << "Total Vértices: " << n << endl;
    cout << "Total Arestas: " << m << endl;

    vector<int> color(n, -1);
    if (grafobipartido(Grafo, color)) {
        cout << "O grafo é bipartido." << endl;
        int EmparelhamentoMaximo = emparelhamentoMaximo(Grafo, n, color);
        cout << "O emparelhamento máximo é: " << EmparelhamentoMaximo << endl;
    } else {
        cout << "O grafo não é bipartido." << endl;
    }

    return 0;
}

void leituraGrafo(vector<vector<int>> &G, int m)
{
    int a, b;
    while (m--)
    {
        cin >> a >> b;
        G[a].push_back(b);
        G[b].push_back(a);
    }
}

void escritaGrafo(vector<vector<int>> &G)
{
    int n = G.size();
    cout << "Lista de Adjacência:" << endl;
    for (int u = 0; u < n; u++)
    {
        cout << u << ": ";
        for (auto v : G[u])
        {
            cout << v << " ";
        }
        cout << endl;
    }
}

bool grafobipartido(vector<vector<int>> &Grafo, vector<int> &color) {
    int n = Grafo.size();
    for (int i = 0; i < n; ++i) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v : Grafo[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool bfs(vector<vector<int>> &residualGraph, int s, int t, vector<int> &parent) {
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == t) {
                    return true;
                }
            }
        }
    }
    return false;
}

int fordFulkerson(vector<vector<int>> &residualGraph, int s, int t) {
    int u, v;
    int n = residualGraph.size();
    vector<int> parent(n);
    int max_flow = 0;

    while (bfs(residualGraph, s, t, parent)) {
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, residualGraph[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= path_flow;
            residualGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }

    return max_flow;
}

int emparelhamentoMaximo(vector<vector<int>> &Grafo, int n, vector<int> &color) {
    int source = n;
    int sink = n + 1;
    
    vector<vector<int>> residualGraph(n + 2, vector<int>(n + 2, 0));

    for (int u = 0; u < n; ++u) {
        if (color[u] == 0) {
            residualGraph[source][u] = 1;
            for (int v : Grafo[u]) {
                residualGraph[u][v] = 1;
            }
        } else {
            residualGraph[u][sink] = 1;
        }
    }

    return fordFulkerson(residualGraph, source, sink);
}