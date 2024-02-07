#include<iostream>
using namespace std;

class node {
public:
	int vertex;
	node* next;
	node(int d) {
		vertex = d;
		next = NULL;
	}
};
class Graph {
private:
	char** maze;
	node** Adj_List;
	int n_vertex;
	int end_crystal;
	int rows;
	int cols;
	int** weight;
	int** path;
	int edge_count;
public:
	Graph(int n_rows, int n_cols) {
		rows = n_rows;
		cols = n_cols;
		n_vertex = rows * cols;

		maze = new char* [rows];
		for (int i = 0; i < rows; i++) {
			maze[i] = new char[cols];
			for (int j = 0; j < cols; j++) {
				maze[i][j] = 0;
			}
		}

		Adj_List = new node * [n_vertex];
		for (int i = 0; i < n_vertex; i++) {
			Adj_List[i] = nullptr;
		}
		weight = new int* [n_vertex];
		path = new int* [n_vertex];
		for (int i = 0; i < n_vertex; i++) {
			weight[i] = new int[n_vertex];
			path[i] = new int[n_vertex];
		}
		initializeGraph();
		end_crystal = 0;
		edge_count = 0;
	}

	void make_maze() {
		srand(time(NULL));
		const int probability = 80;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				int random = rand() % 100;
				if (random > probability && random < 85) {
					if (i == 0 && j == 0) {
						maze[i][j] = 'C'; //First element
					}
					else {
						maze[i][j] = '#';
					}
				}
				else if (random <= probability && random > 75) {
					maze[i][j] = 'P';
				}
				else if (random <= 75 && random > 70) {
					maze[i][j] = 'J';
				}
				else if (random <= 70 && random > 68) {
					maze[i][j] = 'W';
				}
				else if (random <= 68 && random > 65) {
					maze[i][j] = '%';
				}
				else if (random <= 65 && random > 63) {
					maze[i][j] = '&';
				}
				else if (random <= 63 && random > 60) {
					maze[i][j] = '$';
				}
				else if (random <= 60 && random > 57) {
					maze[i][j] = '@';
				}
				else {
					maze[i][j] = 'C';
				}
			}
		}
		int Point_X = rand() % rows;
		int Point_Y = rand() % cols;
		maze[Point_X][Point_Y] = '*';
	}
	void Display_maze() {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (maze[i][j] == '*')
				{
					end_crystal = i * cols + j;
				}
				cout << maze[i][j] << " ";
			}
			cout << endl;
		}
	}
	void Add_Edge(int source, int destination) {
		node* newNode = new node(destination);
		newNode->next = Adj_List[source];
		Adj_List[source] = newNode;
		edge_count++;
	}
	void initializeGraph() {
		for (int i = 0; i < n_vertex; i++) {
			for (int j = 0; j < n_vertex; j++) {
				weight[i][j] = 100;
				path[i][j] = -1;
			}
		}
	}
	void make_adjacencyList() {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (maze[i][j] != '#') {
					int current_vertex = i * cols + j;
					if (i - 1 >= 0 && maze[i - 1][j] != '#') {
						int neighbour_vertex = (i - 1) * cols + j;
						Add_Edge(current_vertex, neighbour_vertex);
						weight[current_vertex][neighbour_vertex] = 1;
					}
					if (i + 1 < rows && maze[i + 1][j] != '#') {
						int neighbour_vertex = (i + 1) * cols + j;
						Add_Edge(current_vertex, neighbour_vertex);
						weight[current_vertex][neighbour_vertex] = 1;
					}
					if (j - 1 >= 0 && maze[i][j - 1] != '#') {
						int neighbour_vertex = i * cols + (j - 1);
						Add_Edge(current_vertex, neighbour_vertex);
						weight[current_vertex][neighbour_vertex] = 1;
					}
					if (j + 1 < cols && maze[i][j + 1] != '#') {
						int neighbour_vertex = i * cols + (j + 1);
						Add_Edge(current_vertex, neighbour_vertex);
						weight[current_vertex][neighbour_vertex] = 1;
					}
				}
			}
		}
	}
	void GraphDisplay() {
		cout << "Displaying Graph..." << endl;
		for (int i = 0; i < n_vertex; i++) {
			if (Adj_List[i] == nullptr) {
				cout << "Adj " << i << endl;
			}
			else {
				cout << "Adj " << i << "->";
				node* temp = Adj_List[i];
				while (temp != nullptr) {
					cout << temp->vertex;
					if (temp->next != nullptr) {
						cout << "->";
					}
					temp = temp->next;
				}
				cout << endl;
			}
		}
	}
	void Adjacency_print() {
		cout << "Displaying Adjacence List..... " << endl;
		for (int i = 0; i < n_vertex; i++) {
			cout << "Vertex " << i << ": ";
			node* temp = Adj_List[i];
			while (temp != nullptr) {
				cout << temp->vertex << " ";
				temp = temp->next;
			}
			cout << endl;
		}
	}
	void floyd() {
		int k = 0;
		while (k < n_vertex) {
			int i = 0;
			while (i < n_vertex) {
				int j = 0;
				while (j < n_vertex) {
					if (weight[i][k] != 100) {
						if (weight[k][j] != 100) {
							if (weight[i][k] + weight[k][j] < weight[i][j]) {
								weight[i][j] = weight[i][k] + weight[k][j];
									path[i][j] = k;
								//	cout << path[i][j] << " ";
							}
						}
					}
					j++;
				}
				i++;
			}
			k++;
		}
	}
	void Shortest_Path(int start) {
		if (path[start][end_crystal] == -1) {
			cout << "No path exists between " << start << " and " << end_crystal << endl;
			return;
		}

		cout << "Shortest path from " << start << " to " << end_crystal << ": ";
		cout << start << "->";

		print_FloydPath(start, end_crystal);

		cout << end_crystal << endl;
	}
	void print_FloydPath(int start, int end) {
		if (path[start][end] == -1) {
			return;
		}
		print_FloydPath(start, path[start][end]);
		cout << path[start][end] << "->";
		print_FloydPath(path[start][end], end);
	}

	void Dijkstra(int start) {
		bool* flag = new bool[n_vertex];
		int* distance = new int[n_vertex];

		for (int i = 0; i < n_vertex; i++) {
			flag[i] = false;
			distance[i] = 100;
		}

		distance[start] = 0;

		int count = 0;
		do {
			int d = Dijkistra_Distance(distance, flag);
			flag[d] = true;

			int j = 0;
			while (j < n_vertex) {
				if (!flag[j]){ 
					if(weight[d][j] != 100)
				    {
					if (distance[d] + weight[d][j] < distance[j])
					{
						distance[j] = distance[d] + weight[d][j];
						path[j][0] = d;  // Update the path array
					}
				}
					}
				j++;
			}
			count++;
		} while (count < n_vertex - 1);

		print_DijkstraPath(start, distance);
	}

	int Dijkistra_Distance(int* distance, bool* flag) {
		int min = 100;
		int min_index = -1;
		int i = 0;
		while (i < n_vertex) {
			if (!flag[i] && distance[i] <= min) {
				min = distance[i];
				min_index = i;
			}
			i++;
		}
		return min_index;
	}

	void print_DijkstraPath(int start, int* distance) {
		cout << "Distance from " << start << " to " << end_crystal << " is: " << distance[end_crystal] << endl;

		if (start == end_crystal) {
			cout << start << endl;
			return;
		}

		if (path[end_crystal][0] == -1) {
			cout << "No path exists from " << start << " to " << end_crystal << endl;
			return;
		}

		int* path1 = new int[n_vertex];
		int curr = end_crystal;
		int count = 0;

		do {
			path1[count++] = curr;
			curr = path[curr][0];
		} while (curr != start);

		path1[count++] = start;

		cout << "Shortest path from " << start << " to " << end_crystal << " is: ";

		int i = count - 1;
		while (i >= 0) {
			if (i != 0) {
				cout << path1[i] << " -> ";
			}
			else {
				cout << path1[i] << endl;
			}
			i--;
		}
	}


	int Find(int i, int* parent) {
		if (parent[i] == i)
			return i;
		return Find(parent[i], parent);
	}
	void Union( int x, int y, int* parent, int* rank) {
		int xroot = Find(x, parent);
		int yroot = Find(y, parent);
		if (rank[xroot] < rank[yroot])
			parent[xroot] = yroot;
		else if (rank[xroot] > rank[yroot])
			parent[yroot] = xroot;
		else {
			parent[yroot] = xroot;
			rank[xroot]++;
		}
	}
	void Kruskal() {
		int* parent = new int[n_vertex];
		int* rank = new int[n_vertex];
		for (int i = 0; i < n_vertex; i++) {
			rank[i] = 0; 
			parent[i] = i;
		}

		int** MST = new int* [n_vertex];
		for (int i = 0; i < n_vertex; i++) {
			MST[i] = new int[n_vertex];
			for (int j = 0; j < n_vertex; j++) {
				MST[i][j] = 0;
			}
		}

		int* weights = new int[n_vertex];  // New array to store edge weights in MST
		int mstIndex = 0;
		int u = 0, v = 0;

		// Apply Kruskal's algorithm until MST is formed or all edges have been processed
		int edgeIndex = 0;
		while ( edgeIndex < edge_count && mstIndex < n_vertex - 1) {
			int minWeight = INT_MAX;
			for (int i = 0; i < n_vertex; i++) {
				node* temp = Adj_List[i];
				while (temp != nullptr) {
					int j = temp->vertex;
					if (Find(i, parent) != Find(j, parent))
					{
						if (weight[i][j] < minWeight) {
							minWeight = weight[i][j];
							 v = j;
							  u = i;
						}
					}
					temp = temp->next;
				}
			}

			edgeIndex++;

			int setU = Find(u, parent);
			int setV = Find(v, parent);

			if (setU != setV) {
				MST[u][v] = minWeight;
				MST[v][u] = minWeight;
				weights[mstIndex] = minWeight;  // Store the weight of the edge
				mstIndex++;
				Union( setU, setV, parent, rank);
			}
		}

		cout << "Minimum Spanning Tree:" << endl;
		int totalWeight = 0;
		for (int i = 0; i < n_vertex; i++) {
			for (int j = i + 1; j < n_vertex; j++) {
				if (MST[i][j] != 0) {
					cout << "(" << i << "," << j << "), weight = " << MST[i][j] << endl;
					totalWeight += MST[i][j];
				}
			}
		}
		cout << "Total Weight of MST: " << totalWeight << endl;
	}


};

int main() {
	const int rows = 20, cols = 20;
	int end = 0;
	Graph G(rows, cols);
	cout << "Game Map :" << endl;
	G.make_maze();
	G.make_adjacencyList();
	//G.Adjacency_print();
	//G.GraphDisplay();
	while (true) {
		int choice = 0;
		G.Display_maze();
		cout << "Press 1 for Shortest path using default location (0,0) by Floyd\n"
			<< "Press 2 for Shortest path using default location (0,0) by Dijkstra\n"
			<< "Press 3 for Shortest path using custom location (coordinates entered by the user) by Floyd\n"
			<< "Press 4 for Shortest path using custom location (coordinates entered by the user) by Dijkstra\n"
			<< "Press 5 for Minimum Spanning Tree by Prim's\n"
			<< "Press 6 for Minimum Spanning Tree by Kruskal\n"
			<< "Press 7 to exit\n";
		cin >> choice;
		system("cls");
		switch (choice) {
		case 1: {
			G.Display_maze();
			G.floyd();
			G.Shortest_Path(0);
			break;
		}
		case 2: {
			G.Display_maze();
			G.Dijkstra(0);
			break;
		}
		case 3: {
			G.Display_maze();
			int start = 0;
			cout << "Enter starting point: ";
			cin >> start;
			G.Shortest_Path(start);
			break;
		}
		case 4: {
			G.Display_maze();
			int start = 0;
			cout << "Enter starting point: ";
			cin >> start;
			G.Dijkstra(start);
			break;
		}
		case 5: {
			
			G.Display_maze();
			
			
		}
		case 6: {
			G.Display_maze();
			G.Kruskal();
			break;
		}
		case 7: {
			return 0;
		}
		}
		system("pause");
		system("cls");
	}
}
	