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

#include <ex3/exceptions.hpp>

#define H1ST_MAKE_EINFO(Name, Type) \
    typedef boost::error_info< \
        struct tag_##Name##_name, \
        Type \
        > Name;

#define H1ST_MAKE_DERIVED(Name, Base) \
    class Name : \
        public Base \
    { \
    };

#define H1ST_MAKE_EXCEPTION(Name) \
    H1ST_MAKE_DERIVED(Name, h1st_exception)

namespace h1st {

class h1st_exception :
    public ex3::exception_base
{
};

H1ST_MAKE_EINFO(argument_name, std::string)
H1ST_MAKE_EINFO(input_value  , std::string)

H1ST_MAKE_EXCEPTION(null_value_exception      )
H1ST_MAKE_EXCEPTION(empty_input_list_exception)
H1ST_MAKE_EXCEPTION(input_not_found_exception )

}
