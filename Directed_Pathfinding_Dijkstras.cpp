#include <bits/stdc++.h>

using namespace std;

class Network
{
private:
	int n;
	long long INF = LLONG_MAX;
	vector<string> cities;
	int start;

	//For storing Distance information
	vector<bool> dist_vis;
	vector<long long> dist_span;
	vector<vector<pair<int,int>>> dist_graph;
	vector<int> dist_parent;

	//For storing Time information 
	vector<bool> time_vis;
	vector<long long> time_span;
	vector<vector<pair<int,int>>> time_graph;
	vector<int> time_parent;

public:
	Network(int no_of_cities,vector<string> city_names);
	string getCity(int i);	

	void addDirectedDistanceEdge(int c1,int c2,int weight);
	void clearDistances();
	void computeDistances(int source);
	void showDistances();
	void showDistancePaths();

	void addDirectedTimeEdge(int c1,int c2,int weight);
	void clearTimes();
	void computeTimes(int source);
	void showTimes();
	void showTimePaths();

	void runDijkstras(vector<bool> &visited, vector<long long> &span, vector<vector<pair<int,int>>> &graph, vector<int> &parent);

	void showDestInfo(int dest);
	void printCities();
};

Network::Network(int no_of_cities,vector<string> city_names)
{
	n = no_of_cities;
	cities = city_names;
	dist_graph = vector<vector<pair<int,int>>>(n+1);
	time_graph = vector<vector<pair<int,int>>>(n+1);
}

string Network::getCity(int i)
{
	string s = cities[i];
	s += "("+to_string(i)+")";
	return s;
}	


//Running dijkstra's algorithm for one graph
void Network::runDijkstras(vector<bool> &visited, vector<long long> &span, vector<vector<pair<int,int>>> &graph, vector<int> &parent)
{
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;

	//Adding the source first
	pq.push({0,start});
	span[start] = 0;

	while(pq.size() > 0)
	{
		//Extracting the minimum weight Edge
		pair<int,int> temp = pq.top();
		int distance = temp.first;
		int node = temp.second;
		pq.pop();

		//Not revisiting an already visited node
		if(visited[node]) 
			continue;
		visited[node] = 1;

		for(auto child:graph[node])
		{
			int child_node = child.first;
			int child_weight = child.second;

			//Minimizing the distance if possible
			if(span[node] + child_weight < span[child_node])
			{
				span[child_node] = span[node] + child_weight;
				pq.push({span[child_node],child_node});

				//Setting the parent of the node which minimizes the dist
				parent[child_node] = node;
			}
		}
	}
}


//For Distances Graph
void Network::addDirectedDistanceEdge(int c1,int c2,int weight)
{
	if(weight<0)
		weight = INF;
	dist_graph[c1].push_back(make_pair(c2,weight));
}

//Filling everything with default values
void Network::clearDistances()
{
	dist_vis = vector<bool>(n+1,0);
	dist_span = vector<long long>(n+1,INF);
	dist_parent = vector<int>(n+1,-1);
}

void Network::computeDistances(int s)
{
	clearDistances();
	start = s;
	runDijkstras(dist_vis,dist_span,dist_graph,dist_parent);
}

void Network::showDistances()
{
	cout<<endl;
	for(int i = 1;i<=n;i++)
	{
		if(i!=start)
		{
			cout<<"Shortest distance from "<<getCity(start)<<" to "<<getCity(i)<<" is : ";
			if(dist_span[i]==LLONG_MAX)
				cout<<"Unreachable";
			else
				cout<<dist_span[i];
			cout<<endl;
		}
	}
	cout<<endl;
}

//Showing the minimized distance paths
void Network::showDistancePaths()
{
	for(int i = 1;i<=n;i++)
	{
		if(i==start)
			continue;
		cout<<"\nShortest path from "<<getCity(start)<<" to "<<getCity(i)<<" is : \n\t"<<endl;
		if(dist_span[i] == INF)
		{
			cout<<"Unreachable"<<endl;
			continue;
		}

		//Computing the path by recursively computing the parent
		int p = i;
		vector<int> temp;
		while(p!=start)
		{
			temp.push_back(p);
			p = dist_parent[p];
		}
		cout<<"\t"<<getCity(start);
		reverse(temp.begin(), temp.end());

		for(auto x:temp)
			cout<<" --> "<<getCity(x);
		cout<<endl;

	}
}


//For Time graph
void Network::addDirectedTimeEdge(int c1,int c2,int weight)
{
	if(weight<0)
		weight = INF;
	time_graph[c1].push_back(make_pair(c2,weight));

	//time_graph[c2].push_back(make_pair(c1,weight));
}

//Filling everything with default values
void Network::clearTimes()
{
	time_vis = vector<bool>(n+1,0);
	time_span = vector<long long>(n+1,INF);
	time_parent = vector<int>(n+1,-1);
}

void Network::computeTimes(int s)
{
	clearTimes();
	start = s;
	runDijkstras(time_vis,time_span,time_graph,time_parent);
}

void Network::showTimes()
{
	cout<<endl;
	for(int i = 1;i<=n;i++)
	{
		if(i!=start)
		{
			cout<<"Minimum time taken from "<<getCity(start)<<" to "<<getCity(i)<<" is : ";
			if(time_span[i]==LLONG_MAX)
				cout<<"Unreachable";
			else
				cout<<time_span[i];
			cout<<endl;
		}
	}
	cout<<endl;
}

void Network::showTimePaths()
{
	for(int i = 1;i<=n;i++)
	{
		if(i==start)
			continue;
		cout<<"\nPath with the minimum time from "<<getCity(start)<<" to "<<getCity(i)<<" is : \n\t"<<endl;
		if(time_span[i] == INF)
		{
			cout<<"Unreachable"<<endl;
			continue;
		}

		//Computing the path by recursively computing the parent
		int p = i;
		vector<int> temp;
		while(p!=start)
		{
			temp.push_back(p);
			p = time_parent[p];
		}
		cout<<"\t"<<getCity(start);
		reverse(temp.begin(), temp.end());

		for(auto x:temp)
			cout<<" --> "<<getCity(x);
		cout<<endl;

	}
}


//Showing both the minimized distance and the time for a specific destination
void Network::showDestInfo(int i)
{
	cout<<"\n\nShortest distance from "<<getCity(start)<<" to "<<getCity(i)<<" is : ";
	if(dist_span[i]==LLONG_MAX)
		cout<<"Unreachable";
	else
		cout<<dist_span[i];
	cout<<endl;

	cout<<"\nShortest path from "<<getCity(start)<<" to "<<getCity(i)<<" is : \n\t"<<endl;

	int p = i;
	vector<int> temp;
	while(p!=start)
	{
		temp.push_back(p);
		p = dist_parent[p];
	}
	cout<<"\t"<<getCity(start);
	reverse(temp.begin(), temp.end());

	for(auto x:temp)
		cout<<" --> "<<getCity(x);
	cout<<endl;
	
	cout<<"\n\nMinimum time taken from "<<getCity(start)<<" to "<<getCity(i)<<" is : ";
	if(time_span[i]==LLONG_MAX)
		cout<<"Unreachable";
	else
		cout<<time_span[i];
	cout<<endl;

	cout<<"\nQuickest path from "<<getCity(start)<<" to "<<getCity(i)<<" is : \n\t"<<endl;

	p = i;
	temp.clear();
	while(p!=start)
	{
		temp.push_back(p);
		p = time_parent[p];
	}
	cout<<"\t"<<getCity(start);
	reverse(temp.begin(), temp.end());

	for(auto x:temp)
		cout<<" --> "<<getCity(x);
	cout<<endl;

}

void Network::printCities()
{
	for(int i = 1;i<=n;i++)
		cout<<"\t"<<i<<"."<<cities[i]<<endl;
}



int main()
{
	int n,m;
	cout<<"Enter the number of cities (numbered 1 to n) : ";
	cin>>n;
	vector<string> cities(n+1);
	cout<<"Enter names of all the cities in order :-"<<endl;
	for(int i = 1;i<=n;i++)
	{
		getline(cin >> ws, cities[i]);
	}
	cout<<"\nEnter the number of paths : ";
	cin>>m;
	cout<<"\nEnter all the directed paths (source,destination,distance,time) :-\n";
	Network net(n,cities);
	for(int i = 1;i<=m;i++)
	{
		int a,b,w1,w2;
		cin>>a>>b>>w1>>w2;

		//Creating the directed graph
		net.addDirectedDistanceEdge(a,b,w1);
		net.addDirectedTimeEdge(a,b,w2);
	}

	cout<<"\n\n\nTRAVEL MAP COMPLETE\n\n\n";

	int ch = 0;
	int source = -1;
	
	while(ch != 8)
	{
		cout<<"\nPlease select an option :-\n\n\t1.Enter a new source city\n\t2.Print minimum distances to all cities\n\t3.Print minimum time-taken to all cities \n\t4.Print Shortest paths to all cities\n\t5.Print Quickest paths to all cities \n\t6.Print information about a specific destination\n\t7.Display all cities in order\n\t8.Exit \n";
		cout<<"\nEnter Choice : ";
		
		cin>>ch;
		cout<<endl;

		if(ch == 1)
		{
			cout<<"Enter a new source city : ";
			cin>>source;

			//Making sure a valid source is selected first
			if(!(source>0 && source<=n))
			{
				cout<<"Invalid source city, please enter a valid source city!!!"<<endl;
				continue;
			}

			//Compute the minimized paths if we have a valid source
			net.computeDistances(source);
			net.computeTimes(source);
			cout<<"\nAll optimized paths computed from source : "<<net.getCity(source)<<endl;
			continue;
		}
		else
		{
			if(!(source>0 && source<=n))
			{
				cout<<"Invalid source city, please enter a valid source city!!!"<<endl;
				continue;
			}
		}

		switch(ch)
		{
			case 2:
			net.showDistances();
			break;

			case 3:
			net.showTimes();
			break;

			case 4:
			net.showDistancePaths();
			break;

			case 5:
			net.showTimePaths();
			break;

			case 6:
			cout<<"\nEnter the destination : ";
			int dest;
			cin>>dest;
			//net.showDestInfo(dest);
			cout<<"This feature is not working currently!\n";
			break;

			case 7:
			net.printCities();
			break;

			case 8:
			cout<<"Exiting!!!";
			break;

			default:
				cout<<"Invalid Choice, please try again!"<<endl;
		}

	}

}

/*
6
Mumbai
Kolkata
Delhi
Ahmedabad
New York
Los Angeles
6
1 3 3 9
5 1 10 1
2 1 2 3
5 2 3 10
6 5 4 5
5 4 9 2
*/

/*
15
C1
C2
C3
C4
C5
C6
C7
C8
C9
C10
C11
C12
C13
C14
C15
32
1 2 42 100
1 2 56 8
2 1 80 95
1 3 77 231
2 3 106 32
2 3 99 86
3 4 7 63
3 4 24 150
4 2 38 96
4 13 36 97
2 5 203 47
2 5 54 540
2 5 1 65
4 5 90 1000
5 4 75 25
5 6 97 27
6 5 400 13
11 6 67 80
6 7 342 6
6 7 55 77
7 14 38 183
14 7 381 381
14 15 81 200
7 8 24 420
7 8 537 37
8 6 9999 999
8 6 642 26
8 9 4 750
9 8 333 2
9 10 52 5000
10 9 321 60
6 9 352 72
*/