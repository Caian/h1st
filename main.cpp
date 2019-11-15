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
#include <iterator>
#include <utility>
#include <string>
#include <vector>
#include <map>

struct node_input
{
    const struct hist_node* const node;
    const std::string file;

    node_input(
        const hist_node* node,
        const std::string& file
    ) :
        node(node),
        file(file)
    {
        if (!node)
        {
            // TODO
            throw 2;
        }

        if (file.size() == 0)
        {
            // TODO
            throw 3;
        }
    }
};

struct hist_node
{
    typedef std::vector<node_input> nodes_in_vector;
    typedef std::vector<std::string> files_out_vector;

    int uuid;
    const nodes_in_vector nodes_in;
    const files_out_vector files_out;
    const std::string command;

    template <typename ITO>
    hist_node(
        int uuid,
        const std::string& command,
        ITO files_out_begin,
        ITO files_out_end
    ) :
        uuid(uuid),
        nodes_in(),
        files_out(files_out_begin, files_out_end),
        command(command)
    {
    }

    template <typename ITN, typename ITO>
    hist_node(
        int uuid,
        ITN nodes_in_begin,
        ITN nodes_in_end,
        const std::string& command,
        ITO files_out_begin,
        ITO files_out_end
    ) :
        uuid(uuid),
        nodes_in(nodes_in_begin, nodes_in_end),
        files_out(files_out_begin, files_out_end),
        command(command)
    {
    }

    void print(
    ) const
    {
        for (size_t i = 0; i < nodes_in.size(); i++)
        {
            const node_input& input = nodes_in[i];

            std::cout
                << input.file << "("
                << input.node->uuid << ") ";
        }

        std::cout << command << " ";

        for (size_t i = 0; i < files_out.size(); i++)
        {
            std::cout
                << files_out[i] << "("
                << uuid << ") ";
        }

        std::cout << std::endl;
    }

    virtual ~hist_node()
    {
    }
};

class hist_graph
{
private:

    typedef std::vector<hist_node*> node_vector;
    typedef std::map<std::string, hist_node*> file_map;

    int _uuid;
    node_vector _nodes;
    file_map _inputs;

    const hist_node* add_node(
        hist_node* node
    )
    {
        _nodes.push_back(node);
        _uuid++;

        for (size_t i = 0; i < node->files_out.size(); i++)
            _inputs[node->files_out[i]] = node;

        prune();

        return node;
    }

    void visit(
        std::vector<int>& visited,
        const hist_node* node
    ) const
    {
        visited[node->uuid] = 1;

        for (size_t i = 0; i < node->nodes_in.size(); i++)
            visit(visited, node->nodes_in[i].node);
    }

    void prune(
    )
    {
        const size_t num_nodes = _nodes.size();
        std::vector<int> visited(num_nodes, 0);

        for (file_map::const_iterator it = _inputs.begin(); it != _inputs.end(); it++)
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

    const hist_node* get_hist_node(
        const std::string& file
    ) const
    {
        file_map::const_iterator it = _inputs.find(file);

        if (it == _inputs.end())
        {
            // TODO
            throw 0;
        }

        return it->second;
    }

public:

    hist_graph(
    ) :
        _uuid(0),
        _nodes(),
        _inputs()
    {
    }

    template <typename ITF, typename ITO>
    const hist_node* push_node(
        ITF files_in_begin,
        ITF files_in_end,
        const std::string& command,
        ITO files_out_begin,
        ITO files_out_end
    )
    {
        std::vector<node_input> nodes_in;

        for (ITF file = files_in_begin; file != files_in_end; file++)
        {
            file_map::const_iterator it = _inputs.find(*file);

            if (it == _inputs.end())
            {
                // TODO
                throw 0;
            }

            node_input node_in(it->second, it->first);
            nodes_in.push_back(node_in);
        }

        hist_node* node = new hist_node(_uuid, nodes_in.begin(),
            nodes_in.end(), command, files_out_begin, files_out_end);

        return add_node(node);
    }

    template <typename ITO>
    const hist_node* push_node(
        const std::string& command,
        ITO files_out_begin,
        ITO files_out_end
    )
    {
        hist_node* node = new hist_node(_uuid, command,
            files_out_begin, files_out_end);

        return add_node(node);
    }

    void print(
    ) const
    {
        for (size_t i = 0; i < _nodes.size(); i++)
            _nodes[i]->print();
    }

    template <typename ITF, typename ITN>
    void track(
        ITF files_begin,
        ITF files_end,
        ITN nodes_out
    ) const
    {
        const size_t num_nodes = _nodes.size();
        std::vector<int> visited(num_nodes, 0);

        for (ITF file = files_begin; file != files_end; file++)
        {
            const hist_node* node_in = get_hist_node(*file);
            visit(visited, node_in);
        }

        for(size_t i = 0; i < num_nodes; i++)
        {
            if (visited[i])
            {
                const hist_node* node = _nodes[i];
                *nodes_out = node;
                nodes_out++;
            }
        }
    }

    virtual ~hist_graph(
    )
    {
        for (size_t i = 0; i < _nodes.size(); i++)
            delete _nodes[i];
    }
};

void push_one(
    hist_graph& graph,
    const std::string& file_in,
    const std::string& command,
    const std::string& file_out
)
{
    const std::string* files_in_begin = &file_in;
    const std::string* files_in_end = files_in_begin + 1;

    const std::string* files_out_begin = &file_out;
    const std::string* files_out_end = files_out_begin + 1;

    graph.push_node(files_in_begin, files_in_end, command,
        files_out_begin, files_out_end);
}

void push_one(
    hist_graph& graph,
    const std::string& command,
    const std::string& file_out
)
{
    const std::string* files_out_begin = &file_out;
    const std::string* files_out_end = files_out_begin + 1;

    graph.push_node(command, files_out_begin, files_out_end);
}

void track_one(
    const hist_graph& graph,
    const std::string& file
)
{
    const std::string* files_in_begin = &file;
    const std::string* files_in_end = files_in_begin + 1;

    std::vector<const hist_node*> nodes;

    graph.track(files_in_begin, files_in_end,
        std::back_inserter(nodes));

    for (size_t i = 0; i < nodes.size(); i++)
    {
        nodes[i]->print();
    }
}

int main(
    int argc,
    const char* argv[]
)
{
    std::cout << "Hello, world!" << std::endl;

    hist_graph graph;

    push_one(graph,              "command 1" , "out.txt"  );
    push_one(graph, "out.txt"  , "command 2" , "out.A.txt");
    push_one(graph, "out.txt"  , "command 3" , "out.A.txt");
    push_one(graph,              "command 4" , "out.txt"  );
    push_one(graph, "out.txt"  , "command 5" , "out.A.txt");
    push_one(graph, "out.A.txt", "command 6" , "out.B.txt");
    push_one(graph, "out.B.txt", "command 7" , "out.C.txt");
    push_one(graph, "out.txt"  , "command 8" , "out.A.txt");
    push_one(graph, "out.txt"  , "command 9" , "out.A.txt");
    push_one(graph, "out.A.txt", "command 10", "out.B.txt");
    graph.print();
    std::cout << "-------- Tracking out.B.txt: --------" << std::endl;
    track_one(graph, "out.B.txt");
    return 0;
}
