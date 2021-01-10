#include "Algorithms.h"

void BFS(const Graph& g, int start)
{
	if (!g.existVertex(start))
		return;

	queue<int> q;
	vector<bool> visited(g.getNumOfVertices());

	q.push(start);
	visited[start] = true;

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		cout << current << endl;

		vector<pair<int, int>> adjacent = g.getSuccessors(current);

		for (size_t i = 0; i < adjacent.size(); i++)
		{
			if (visited[adjacent[i].first])
				continue;

			q.push(adjacent[i].first);
			visited[adjacent[i].first] = true;
		}
	}
}
void DFS(const Graph& g, int start)
{
	if (!g.existVertex(start))
		return;

	stack<int> s;
	vector<bool> visited(g.getNumOfVertices());

	s.push(start);

	while (!s.empty())
	{
		int current = s.top();
		s.pop();

		if (visited[current])
			continue;
		visited[current] = true;

		cout << current << endl;

		vector<pair<int, int>> adjacent = g.getSuccessors(current);

		for (size_t i = 0; i < adjacent.size(); i++)
			s.push(adjacent[i].first);
	}
}

bool BFS_containsPath(const Graph& g, int start, int end)
{
	if (!g.existVertex(start))
		throw "Invalid vertex!";

	queue<int> q;
	vector<bool> visited(g.getNumOfVertices());

	q.push(start);
	visited[start] = true;

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		if (current == end)
			return true;

		vector<pair<int, int>> adjacent = g.getSuccessors(current);

		for (size_t i = 0; i < adjacent.size(); i++)
		{
			if (visited[adjacent[i].first])
				continue;

			q.push(adjacent[i].first);
			visited[adjacent[i].first] = true;
		}
	}

	return false;
}
bool containsPath(const Graph& g, int start, int end)
{
	return BFS_containsPath(g, start, end);
}

bool BFS_connected(const Graph& g)
{
	if (!g.existVertex(0))
		throw "Invalid vertex!";

	queue<int> q;
	vector<bool> visited(g.getNumOfVertices());

	int vertices = 0;

	q.push(0);
	vertices++;
	visited[0] = true;

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		vector<pair<int, int>> adjacent = g.getSuccessors(current);

		for (size_t i = 0; i < adjacent.size(); i++)
		{
			if (visited[adjacent[i].first])
				continue;

			q.push(adjacent[i].first);
			visited[adjacent[i].first] = true;

			vertices++;
		}
	}

	return vertices == g.getNumOfVertices();
}
bool connected(const Graph& g)
{
	return BFS_connected(g);
}

int Dijkstra(const Graph& g, int start, int end, vector<int>& path)
{
	vector<int> dist(g.getNumOfVertices(), INT_MAX);
	vector<int> previous(g.getNumOfVertices());

	struct temp
	{
		int vertex;
		int distFromStart;

		bool operator<(const temp& other) const
		{
			return distFromStart > other.distFromStart;
		}
	};

	priority_queue<temp> q;

	dist[start] = 0;
	q.push({ start, 0 });

	while (!q.empty())
	{
		temp currentVertex = q.top();

		if (currentVertex.vertex == end)
		{
			int current = end;
			while (current != start)
			{
				path.push_back(current);
				current = previous[current];
			}

			path.push_back(start);
			reverse(path.begin(), path.end());

			return currentVertex.distFromStart;
		}

		q.pop();

		vector<pair<int, int>> successors = g.getSuccessors(currentVertex.vertex);

		for (int i = 0; i < successors.size(); i++)
		{
			int currentSuccessor = successors[i].first;
			int currentWeight = successors[i].second;

			if (dist[currentVertex.vertex] + currentWeight < dist[currentSuccessor])
			{
				dist[currentSuccessor] = dist[currentVertex.vertex] + currentWeight;
				q.push({ currentSuccessor , dist[currentSuccessor] });
				previous[currentSuccessor] = currentVertex.vertex;
			}
		}
	}

	return INT_MAX;
}

int Prim(const Graph& g, Graph& mst)
{
	int mstWeight = 0;
	vector<bool> visited(g.getNumOfVertices(), false);

	auto comp = [](const tuple<int, int, int>& lhs, const tuple<int, int, int>& rhs) { return get<2>(lhs) > get<2>(rhs); };
	priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, decltype(comp)> q(comp);

	int addedEdges = 0;
	q.push(make_tuple(0, 0, 0)); // virtual edge for the start

	while (addedEdges < g.getNumOfVertices() - 1)
	{
		tuple<int, int, int> currentEdge = q.top();
		q.pop();

		int currentEdgeStart = get<0>(currentEdge);
		int currentEdgeEnd = get<1>(currentEdge);
		int currentEdgeWeight = get<2>(currentEdge);

		if (visited[currentEdgeEnd])
			continue;

		mstWeight += currentEdgeWeight;

		if (currentEdgeStart != currentEdgeEnd) // in order not to count the virtual edge
		{
			addedEdges++;
			cout << "Added edge: " << currentEdgeStart << "----" << currentEdgeEnd << " with weight: " << currentEdgeWeight << endl;
			mst.addEdge(currentEdgeStart, currentEdgeEnd, currentEdgeWeight);
		}
		visited[currentEdgeEnd] = true;

		vector<pair<int, int>> successors = g.getSuccessors(currentEdgeEnd);

		for (int i = 0; i < successors.size(); i++)
			q.push(make_tuple(currentEdgeEnd, successors[i].first, successors[i].second));
	}

	return mstWeight;
}