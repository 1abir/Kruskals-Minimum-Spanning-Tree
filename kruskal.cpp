//
// Created by abirm on 11/30/2018.
//

#include <iostream>
#include <ostream>
#include <cmath>
#include <stdlib.h>
#include <algorithm>

using namespace std;

template <typename T> class Vector
{
    unsigned int _size;
    unsigned int _capacity;
    unsigned int Log;
    T* buffer;
public:
    Vector():_size(0),_capacity(0),Log(0) {}
    unsigned int size() const {
        return _size;
    }
    void reserve(unsigned int capacity) {
        T * newBuffer = (T*) malloc(capacity*8* sizeof(T));

        for (unsigned int i = 0; i < _size; i++)
            newBuffer[i] = buffer[i];

        _capacity = capacity;

        buffer = newBuffer;
    }
    void resize(unsigned int size) {
        Log = ceil(log((double) size) / log(2.0));
        reserve(1 << Log);
        _size = size;
    }
    T& operator[](unsigned int index) {
        return buffer[index];
    }
    void push_back(const T & v) {
        if (_size >= _capacity) {
            reserve(1 << Log);
            Log++;
        }
        buffer [_size++] = v;
    }
    Vector<T>& operator = (const Vector<T> & v) {
        _size = v._size;
        Log = v.Log;
        _capacity = v._capacity;
        buffer = (T*) calloc(_capacity, sizeof(T));
        for (unsigned int i = 0; i < _size; i++)
            buffer[i] = v.buffer[i];
        return *this;
    }
    T* begin() {
        return buffer;
    }

     T* end() {
        return buffer + size();
    }
};
class Edge
{
    unsigned int from,to;
    int cost;
public:
    Edge(unsigned int from, unsigned int to, int cost) : from(from), to(to), cost(cost) {}

    unsigned int getFrom() const {return from;}
    unsigned int getTo() const {return to;}
    int getCost() const {return cost;}

    bool operator<(const Edge &rhs) const {
        return cost < rhs.cost;
    }

    friend ostream &operator<<(ostream &os, const Edge &edge) {
        os << " ( " << edge.from << " , " << edge.to<<" ) ";
        return os;
    }
};

class Kruscal
{
    unsigned int nNodes;
    int mst_cost;
    Vector<int> distance,parent;
    Vector<Edge> *adList;
    Vector<Edge> edgeList,spanning_tree;

public:

    explicit Kruscal(unsigned int nodes):nNodes(nodes)
    {
        distance.resize(nodes);
        parent.resize(nodes);
        mst_cost=0;
        adList=new Vector<Edge>[nodes];
    }
    bool addEdge(unsigned int u, unsigned int v, int cost)
    {
        if(u>=nNodes||v>=nNodes)
            return false;
        Edge e(u,v,cost);
        adList[u].push_back(e);
        edgeList.push_back(e);
        //adList[v].push_back(v,u,cost);
    }
    void makeset(int u){parent[u]=u;}
    void init_disjoint_set()
    {
        for (int i = 0; i < nNodes; ++i)
            makeset(i);
    }
    int findRepresentative(int r)
    {
        if(parent[r]==r)
            return r;
        return parent[r]=findRepresentative(parent[r]);
    }
    void Union(int u,int v)
    {
        int a=findRepresentative(u);
        int b=findRepresentative(v);
        if(a!=b)
        {
            parent[a]=b;
        }
    }
    int  run()
    {
        spanning_tree = Vector<Edge>();
        mst_cost=0;
        sort(edgeList.begin(),edgeList.end());
        init_disjoint_set();
        for (int i = 0; i < edgeList.size(); ++i) {
            int u=edgeList[i].getFrom();
            int v=edgeList[i].getTo();
            if(findRepresentative(u)!=findRepresentative(v))
            {
                Union(u,v);
                spanning_tree.push_back(edgeList[i]);
                mst_cost+=edgeList[i].getCost();
            }
        }
        return mst_cost;
    }
    void print()
    {
        for (int i = 0; i < spanning_tree.size(); ++i) {
            cout<<spanning_tree[i]<<endl;
        }

    }

};

int main()
{
    freopen("mst.txt","r",stdin);
    freopen("mstOutput.txt","w",stdout);
    unsigned int u,v,nodes,edges;
    int cost;
    cin>>nodes>>edges;
    Kruscal ks(nodes);
    for (int i = 0; i < edges; ++i)
    {
        cin>>u>>v>>cost;
        ks.addEdge(u,v,cost);
    }
    int weight=ks.run();
    cout<<"Added edges: \n";
    ks.print();

    cout<<"\nMST weight: "<<weight;

    return 0;
}
