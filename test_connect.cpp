/**
 * 判断连通性
 * */


#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/config.hpp>

using namespace std;
using namespace boost;

namespace qi = boost::spirit::qi;

typedef subgraph < adjacency_list<vecS, vecS, directedS,                //adjacency_list<outedgelist,vertexlist,directed,vertexproperties,edgeproperties,graphproperties,edgelist>
    property<vertex_color_t, int>, property<edge_index_t, int>>> Graph;      //模板类实例化，vecs分别表示离开顶点的outedge边和途中顶点集合所使用的数据结构，directeds为选择一个仅提供出边的有向图
typedef graph_traits<Graph>::vertex_iterator vertex_iter;                //顶点迭代器
typedef graph_traits<Graph>::edge_iterator edge_iter;

int main()
{
    std::ifstream ifs("/home/lyzlinux/Desktop/1/Building_edge.txt");             //读文件
    //std::ifstream ifs("/home/lyzlinux/Desktop/1/test.txt");
    ifs >> std::noskipws;                                             //不忽略空白字符进行读取

    boost::spirit::istream_iterator f(ifs), l;

    std::vector<std::pair<int, int>> edges;
    bool parse_ok = qi::phrase_parse(f, l, (qi::int_ >> qi::int_) % qi::eol, qi::blank, edges);//解析，四个参数，前两个为迭代器头和尾，第三个为解析器语法，第四个为忽略，>>匹配先后顺序，%对x匹配一次或多次，y分隔

    Graph g;

    for (const auto edge : edges )
    {
        add_edge(edge.first, edge.second, g);
    }

    std::vector<int> comp(num_vertices(g));
    int num = connected_components(g, comp.data());
    std::cout << std::endl;
    std::vector<int>::iterator i;

    //将结果输出到txt
    std::ofstream connect("/home/lyzlinux/Desktop/1/Building_Connect.txt");
    connect << " Total number of components : " << num << std::endl;

    // std::vector<Graph*> comps(num);

    // for (size_t i = 0; i < num; i++)
    // {
    //     comps[i] = &g.create_subgraph();    //comp[i]为子图
    // }
    // for (size_t i = 0; i < num_vertices(g); i++)
    // {
    //     add_vertex(i, *comps[comp[i]]);
    // }

    // std::pair<vertex_iter, vertex_iter> vip;

    // connect << " Vertices in g = [ ";
    // vip = vertices(g);
    // for(vertex_iter vi = vip.first; vi != vip.second; ++ vi)
    // {
    //    connect << *vi << " ";
    // }
    // connect << " ]" << std::endl;
    // for (size_t i = 0; i < (num); i++)
    // {
    //     connect << " Vertices (local) in comps [ " << i << " ] ' = [ ";
    //     std::pair<vertex_iter, vertex_iter> lvip;
    //     lvip = vertices(*comps[i]);

    //     for (vertex_iter vi = lvip.first; vi != lvip.second; ++ vi)
    //     {
    //         connect << (*comps[i]).local_to_global(*vi) << " ";
    //     }

    //     connect << " ]" << std::endl;

    // }
    
    return 0;
    
}
