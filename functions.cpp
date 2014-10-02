/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * DistanceMap member functions add, decrease, length, minpath.
 */

#include "aimn91.h"
// add edge (i,j) with edge weight w
void DistanceMap::add_e(v_desc i, v_desc j, int w){
    if (boost::edge(i,j,g).second){
        std::cout<<"Edge from vertex "<<i<<" to vertex "<<j<<" already exists"<<std::endl;
        throw std::invalid_argument("Did not add edge");
    }
    vertex_tree x = B[std::make_pair(i,i)];
    vertex_tree root = F[std::make_pair(j,j)];
    edge_desc e = boost::add_edge(i,j,g).first; //add new edge to the graph
    boost::put(boost::edge_weight, g, e, w); //assign weight to the new edge 
    UpdateForwardBackward(x,i,j,g[j].DESC,root,w); // update desc and anc trees
// Testing code, comment out in case you want to see what happened after add_e
    std::pair<vertex_it, vertex_it> vi;
    std::pair<tree_vertex_it, tree_vertex_it> vi_tree;
    // iterate over graph vertices
    for (vi=boost::vertices(g); vi.first != vi.second; ++vi.first){
        std::cout<<"graph vertex: "<<*vi.first<<std::endl;
        // iterate in each vertices ANC and DESC
        std::cout<<"DESC "<<std::endl;
        for (vi_tree=boost::vertices(g[*vi.first].DESC); vi_tree.first != vi_tree.second; ++vi_tree.first){
            // print out vertices of the desc tree for vertex of graph ( vertices relation to g )
            std::cout<<boost::any_cast<v_desc>(g[i].DESC[*vi_tree.first].v_g)<<std::endl;
        }
        std::cout<<"ANC "<<std::endl;
        for (vi_tree=boost::vertices(g[*vi.first].ANC); vi_tree.first != vi_tree.second; ++vi_tree.first){
            // print out vertices of the anc tree for vertex of graph ( vertices relation to g )
            std::cout<<boost::any_cast<v_desc>(g[i].ANC[*vi_tree.first].v_g)<<std::endl;
        }
    }
    return;
}
// decrease edge weight of existing edge (i,j)
void DistanceMap::decrease_w(v_desc i, v_desc j, int w){
    if (!boost::edge(i,j,g).second){
        std::cout<<"Edge from vertex "<<i<<" to vertex "<<j<<" does not exist"<<std::endl;
        throw std::invalid_argument("Did not decrease weight");
    }
    edge_desc e = boost::edge(i,j,g).first;
    int existing_weight = boost::get(boost::edge_weight, g, e);
    if (existing_weight > w){
        boost::put(boost::edge_weight, g, e, w); //assign weight to the new edge
        vertex_tree x = B[std::make_pair(i,i)];
        vertex_tree root = F[std::make_pair(j,j)];
        UpdateForwardBackward(x,i,j,g[i].DESC,root,w); // update desc and anc trees
    }
    return;
}
// return length of minpath from vertex x to vertex y
int DistanceMap::length(v_desc x, v_desc y){
    int length = d[std::make_pair(x,y)];
    return length;
}
// return minpath from vertex x to vertex y
path DistanceMap::minpath(v_desc x, v_desc y){
    path minpath;
    edge_desc e;
    if (F[std::make_pair(x,y)]==0)
        return minpath;
    else{
        //Tree desc_x = DESC[x];
        //vertex_tree root = F[std::make_pair(x,x)];
        vertex_tree it = F[std::make_pair(x,y)];
        vertex_tree it_p = boost::any_cast<vertex_tree>(g[x].DESC[it].p_in_t);
        do{
            v_desc it_in_g = boost::any_cast<v_desc>(g[x].DESC[it].v_g);
            e = boost::edge(boost::any_cast<v_desc>(g[x].DESC[it_p].v_g), it_in_g,g).first;
            minpath.push(e);
            it = it_p;
            it_p = boost::any_cast<vertex_tree>(g[x].DESC[it].p_in_t);
        }while(it_p!=it);
    }
    return minpath;
}
