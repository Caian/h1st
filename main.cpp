/*
 * Copyright (C) 2019 Caian Benedicto <caianbene@gmail.com>
 *
 * This file is part of historian.
 *
 * historian is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * historian is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Make; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <map>

struct hist_node
{
    int uuid;
    const hist_node* node_in;
    std::string file_out;
    std::string args;

    hist_node(
        int uuid,
        const hist_node* node_in,
        std::string args,
        std::string file_out
    ) :
        uuid(uuid),
        node_in(node_in),
        args(args),
        file_out(file_out)
    {
    }

    void print(
    ) const
    {
        if (node_in)
        {
            std::cout
                << node_in->file_out << "("
                << node_in->uuid << ") ";
        }

        std::cout << args << " "
            << file_out << "("
            << uuid << ") "
            << std::endl;
    }

    virtual ~hist_node()
    {
    }
};

class hist_graph
{
private:

    typedef std::vector<hist_node*> node_vector;
    typedef std::map<std::string, hist_node*> input_map;

    int _uuid;
    node_vector _nodes;
    input_map _inputs;

    const hist_node* add_node(
        const hist_node* node_in,
        const std::string& args,
        const std::string& file_out
    )
    {
        hist_node* node = new hist_node(_uuid, node_in, args, file_out);
        _nodes.push_back(node);
        _uuid++;
        _inputs[file_out] = node;
        return node;
    }

    void visit(
        std::vector<int>& visited,
        const hist_node* node
    )
    {
        visited[node->uuid] = 1;

        if (node->node_in)
            visit(visited, node->node_in);
    }

public:

    hist_graph(
    ) :
        _uuid(0),
        _nodes(),
        _inputs()
    {
    }

    const hist_node* push_node(
        const std::string& file_in,
        const std::string& args,
        const std::string& file_out
    )
    {
        input_map::const_iterator it = _inputs.find(file_in);

        if (it == _inputs.end())
        {
            // TODO
            throw 0;
        }

        const hist_node* node_in = it->second;
        return add_node(node_in, args, file_out);
    }

    const hist_node* push_node(
        const std::string& args,
        const std::string& file_out
    )
    {
        return add_node(0, args, file_out);
    }

    void print(
    ) const
    {
        for (size_t i = 0; i < _nodes.size(); i++)
            _nodes[i]->print();
    }

    // TODO move to add_node
    void prune(
    )
    {
        const size_t num_nodes = _nodes.size();
        std::vector<int> visited(num_nodes, 0);

        for (input_map::const_iterator it = _inputs.begin(); it != _inputs.end(); it++)
            visit(visited, it->second);

        _uuid = 0;
        size_t j = 0;
        for (size_t i = 0; i < num_nodes; i++)
        {
            std::swap(_nodes[j], _nodes[i]);
            std::swap(visited[j], visited[i]);

            _nodes[j]->uuid = _uuid;

            if (visited[j])
            {
                j++;
                _uuid++;
            }
        }

        for (size_t i = j; i < num_nodes; i++)
            delete _nodes[i];

        _nodes.resize(j);
    }

    virtual ~hist_graph(
    )
    {
        for (size_t i = 0; i < _nodes.size(); i++)
            delete _nodes[i];
    }
};

int main(
    int argc,
    const char* argv[]
)
{
    std::cout << "Hello, world!" << std::endl;

    hist_graph graph;
    graph.push_node(             "args 1" , "out.txt"  );
    graph.push_node("out.txt"  , "args 2" , "out.A.txt");
    graph.push_node("out.txt"  , "args 3" , "out.A.txt");
    graph.push_node(             "args 4" , "out.txt"  );
    graph.push_node("out.txt"  , "args 5" , "out.A.txt");
    graph.push_node("out.A.txt", "args 6" , "out.B.txt");
    graph.push_node("out.B.txt", "args 7" , "out.C.txt");
    graph.push_node("out.txt"  , "args 8" , "out.A.txt");
    graph.push_node("out.txt"  , "args 9" , "out.A.txt");
    graph.push_node("out.A.txt", "args 10", "out.B.txt");
    std::cout << "-------- Before prunning: --------" << std::endl;
    graph.print();
    std::cout << "-------- After prunning: --------" << std::endl;
    graph.prune();
    graph.print();
    return 0;
}
