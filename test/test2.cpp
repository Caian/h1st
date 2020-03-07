/*
 * Copyright (C) 2019 Caian Benedicto <caianbene@gmail.com>
 *
 * This file is part of h1st.
 *
 * h1st is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * h1st is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with h1st.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <h1st/historian.hpp>

#include <gtest/gtest.h>

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace {

/**
 *
 */
class TestSingleInOut : public ::testing::Test
{
protected:

    h1st::hist_graph graph;

    void SetUp(
    )
    {
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
    }
};

/**
 *
 */
TEST_F(TestSingleInOut, HasInputs)
{
    ASSERT_TRUE(graph.has_input("out.txt"));
    ASSERT_TRUE(graph.has_input("out.A.txt"));
    ASSERT_TRUE(graph.has_input("out.B.txt"));
    ASSERT_TRUE(graph.has_input("out.C.txt"));
}

/**
 *
 */
TEST_F(TestSingleInOut, HasNotInputs)
{
    ASSERT_FALSE(graph.has_input("out.D.txt"));
    ASSERT_FALSE(graph.has_input("foobar"));
    ASSERT_FALSE(graph.has_input(""));
}

/**
 *
 */
TEST_F(TestSingleInOut, PrintGraph)
{
    std::stringstream ss;
    h1st::hist_node_print_to_stream printer(&ss);
    ASSERT_NO_THROW(graph.print(printer));
}

/**
 *
 */
TEST_F(TestSingleInOut, TrackAllInputs)
{
    std::stringstream ss;
    h1st::hist_node_print_to_stream printer(&ss);

    std::vector<std::string> files_in;
    files_in.push_back("out.txt");
    files_in.push_back("out.A.txt");
    files_in.push_back("out.B.txt");
    files_in.push_back("out.C.txt");

    std::vector<const h1st::hist_node*> nodes;

    for (size_t i = 0; i < files_in.size(); i++)
        ASSERT_TRUE(graph.has_input(files_in[i]));

    ASSERT_NO_THROW(graph.track(files_in.begin(), files_in.end(),
        std::back_inserter(nodes), false));

    ASSERT_EQ(6, nodes.size());
    ASSERT_STREQ("command 4" , nodes[0]->command().c_str());
    ASSERT_STREQ("command 5" , nodes[1]->command().c_str());
    ASSERT_STREQ("command 6" , nodes[2]->command().c_str());
    ASSERT_STREQ("command 7" , nodes[3]->command().c_str());
    ASSERT_STREQ("command 9" , nodes[4]->command().c_str());
    ASSERT_STREQ("command 10", nodes[5]->command().c_str());

    for (size_t i = 0; i < nodes.size(); i++)
        ASSERT_NO_THROW(printer(nodes[i]));
}

/**
 *
 */
TEST_F(TestSingleInOut, TrackInputB)
{
    std::stringstream ss;
    h1st::hist_node_print_to_stream printer(&ss);

    const std::string file = "out.B.txt";
    const std::string* files_in_begin = &file;
    const std::string* files_in_end = files_in_begin + 1;

    std::vector<const h1st::hist_node*> nodes;

    ASSERT_NO_THROW(graph.track(files_in_begin, files_in_end,
        std::back_inserter(nodes), false));

    ASSERT_EQ(3, nodes.size());
    ASSERT_STREQ("command 4" , nodes[0]->command().c_str());
    ASSERT_STREQ("command 9" , nodes[1]->command().c_str());
    ASSERT_STREQ("command 10", nodes[2]->command().c_str());

    for (size_t i = 0; i < nodes.size(); i++)
        ASSERT_NO_THROW(printer(nodes[i]));
}

/**
 *
 */
TEST_F(TestSingleInOut, TrackAllInputA)
{
    std::stringstream ss;
    h1st::hist_node_print_to_stream printer(&ss);

    std::vector<std::string> files_in;
    files_in.push_back("out.A.txt");
    files_in.push_back("out.D.txt");

    std::vector<const h1st::hist_node*> nodes;

    try
    {
        graph.track(files_in.begin(), files_in.end(),
            std::back_inserter(nodes), false);

        FAIL() << "graph.track did not throw any exception!";
    }
    catch (
        const h1st::input_not_found_exception& ex
    )
    {
        const std::string* iname;
        iname = boost::get_error_info<h1st::input_value>(ex);

        ASSERT_NE((void*)0, iname);
        ASSERT_STREQ("out.D.txt", iname->c_str());
    }
    catch (...)
    {
        FAIL() << "Unexpected exception thrown by graph.track!";
    }
}

/**
 *
 */
TEST_F(TestSingleInOut, TrackAnyInputA)
{
    std::stringstream ss;
    h1st::hist_node_print_to_stream printer(&ss);

    std::vector<std::string> files_in;
    files_in.push_back("out.A.txt");
    files_in.push_back("out.D.txt");

    std::vector<const h1st::hist_node*> nodes;

    ASSERT_NO_THROW(graph.track(files_in.begin(), files_in.end(),
        std::back_inserter(nodes), true));

    ASSERT_EQ(2, nodes.size());
    ASSERT_STREQ("command 4" , nodes[0]->command().c_str());
    ASSERT_STREQ("command 9" , nodes[1]->command().c_str());

    for (size_t i = 0; i < nodes.size(); i++)
        ASSERT_NO_THROW(printer(nodes[i]));
}

}
