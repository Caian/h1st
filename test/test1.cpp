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
TEST(TestInvalidGraph, EmptyOutput)
{
    h1st::hist_graph graph;

    std::vector<std::string> files_out;

    ASSERT_THROW(graph.push_node("command 1", files_out.begin(),
        files_out.end()), h1st::empty_output_exception);
}

/**
 *
 */
TEST(TestInvalidGraph, MissingInput)
{
    h1st::hist_graph graph;

    std::vector<std::string> files_in;
    files_in.push_back("out.txt");

    std::vector<std::string> files_out;
    files_out.push_back("out.A.txt");

    ASSERT_THROW(graph.push_node(files_in.begin(), files_in.end(),
        "command 1", files_out.begin(), files_out.end()),
        h1st::input_not_found_exception);
}

/**
 *
 */
TEST(TestInvalidPrinter, NullStream)
{
    ASSERT_THROW(h1st::hist_node_print_to_stream printer(0),
        h1st::null_value_exception);
}

/**
 *
 */
TEST(TestInvalidNodeInput, NullHist)
{
    try
    {
        h1st::node_input ni(0, "test");

        FAIL() << "node_input constructor did not throw any exception!";
    }
    catch (
        const h1st::null_value_exception& ex
    )
    {
        const std::string* aname;
        aname = boost::get_error_info<h1st::argument_name>(ex);

        ASSERT_NE((void*)0, aname);
        ASSERT_STREQ("node", aname->c_str());
    }
    catch (...)
    {
        FAIL() << "Unexpected exception thrown by node_input constructor!";
    }
}

/**
 *
 */
TEST(TestInvalidNodeInput, EmptyInput)
{
    const std::string file = "out.txt";
    const std::string* files_out_begin = &file;
    const std::string* files_out_end = files_out_begin + 1;

    try
    {
        h1st::hist_node node(0, "", files_out_begin, files_out_end);

        try
        {
            h1st::node_input ni(&node, "");

            FAIL() << "node_input constructor did not throw any exception!";
        }
        catch (
            const h1st::empty_input_value_exception& ex
        )
        {
            const std::string* aname;
            aname = boost::get_error_info<h1st::argument_name>(ex);

            ASSERT_NE((void*)0, aname);
            ASSERT_STREQ("file", aname->c_str());
        }
        catch (...)
        {
            FAIL() << "Unexpected exception thrown by node_input constructor!";
        }
    }
    catch(
        ...
    )
    {
        FAIL() << "hist_node constructor threw an exception!";
    }
}

}
