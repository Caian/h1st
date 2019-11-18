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

#pragma once

#include <utility>
#include <ostream>
#include <string>
#include <vector>
#include <map>

namespace h1st {

class node_input
{
private:

    const struct hist_node* _node;
    std::string _file;

public:

    node_input(
        const hist_node* node,
        const std::string& file
    ) :
        _node(node),
        _file(file)
    {
        if (!_node)
        {
            // TODO
            throw 2;
        }

        if (_file.size() == 0)
        {
            // TODO
            throw 3;
        }
    }

    const struct hist_node* node(
    ) const
    {
        return _node;
    }

    const std::string& file(
    ) const
    {
        return _file;
    }
};

class hist_node
{
public:

    typedef std::vector<node_input> nodes_in_vector;
    typedef std::vector<std::string> files_out_vector;

private:

    int _uuid;
    nodes_in_vector _nodes_in;
    files_out_vector _files_out;
    std::string _command;

public:

    template <typename ITO>
    hist_node(
        int uuid,
        const std::string& command,
        ITO files_out_begin,
        ITO files_out_end
    ) :
        _uuid(uuid),
        _nodes_in(),
        _files_out(files_out_begin, files_out_end),
        _command(command)
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
        _uuid(uuid),
        _nodes_in(nodes_in_begin, nodes_in_end),
        _files_out(files_out_begin, files_out_end),
        _command(command)
    {
    }

    const int& uuid(
    ) const
    {
        return _uuid;
    }

    int& uuid(
    )
    {
        return _uuid;
    }

    const nodes_in_vector& nodes_in(
    ) const
    {
        return _nodes_in;
    }

    const files_out_vector& files_out(
    ) const
    {
        return _files_out;
    }

    const std::string& command(
    ) const
    {
        return _command;
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

        for (size_t i = 0; i < node->files_out().size(); i++)
            _inputs[node->files_out()[i]] = node;

        prune();

        return node;
    }

    void visit(
        std::vector<int>& visited,
        const hist_node* node
    ) const
    {
        visited[node->uuid()] = 1;

        for (size_t i = 0; i < node->nodes_in().size(); i++)
            visit(visited, node->nodes_in()[i].node());
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

            _nodes[j]->uuid() = _uuid;

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

    template <typename Printer>
    void print(
        Printer& printer
    ) const
    {
        for (size_t i = 0; i < _nodes.size(); i++)
            printer(_nodes[i]);
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

class hist_node_print_to_stream
{
private:

    std::ostream* _p_stream;

public:

    hist_node_print_to_stream(
        std::ostream* p_stream
    ) :
        _p_stream(p_stream)
    {
        if (_p_stream == NULL)
        {
            // TODO
            throw 5;
        }
    }

    void operator ()(
        const hist_node* node
    )
    {
        for (size_t i = 0; i < node->nodes_in().size(); i++)
        {
            const node_input& input = node->nodes_in()[i];

            (*_p_stream)
                << input.file() << "("
                << input.node()->uuid() << ") ";
        }

        (*_p_stream) << node->command() << " ";

        for (size_t i = 0; i < node->files_out().size(); i++)
        {
            (*_p_stream)
                << node->files_out()[i] << "("
                << node->uuid() << ") ";
        }

        (*_p_stream) << std::endl;
    }
};

}
