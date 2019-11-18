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

#include <h1st/historian.hpp>

#include <gtest/gtest.h>

#include <iterator>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <map>

/**
 *
 */
TEST(TestHistorian, SingleInOut)
{
    using namespace historian;

    std::stringstream ss;

    hist_node_print_to_stream printer(&ss);
    hist_graph graph;

    {
        std::vector<std::string> files_out;
        files_out.push_back("out.txt");

        ASSERT_NO_THROW(graph.push_node(
            "command 1", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.A.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 2", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.A.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 3", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_out;
        files_out.push_back("out.txt");

        ASSERT_NO_THROW(graph.push_node(
            "command 4", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.A.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 5", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.A.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.B.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 6", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.B.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.C.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 7", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.A.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 8", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.A.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 9", files_out.begin(), files_out.end()));
    }
    {
        std::vector<std::string> files_in;
        files_in.push_back("out.A.txt");

        std::vector<std::string> files_out;
        files_out.push_back("out.B.txt");

        ASSERT_NO_THROW(graph.push_node(files_in.begin(), files_in.end(),
            "command 10", files_out.begin(), files_out.end()));
    }

    ASSERT_NO_THROW(graph.print(printer));

    {
        const std::string file = "out.B.txt";
        const std::string* files_in_begin = &file;
        const std::string* files_in_end = files_in_begin + 1;

        std::vector<const hist_node*> nodes;

        ASSERT_NO_THROW(graph.track(files_in_begin, files_in_end,
            std::back_inserter(nodes)));

        for (size_t i = 0; i < nodes.size(); i++)
            ASSERT_NO_THROW(printer(nodes[i]));
    }
}
