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


}
