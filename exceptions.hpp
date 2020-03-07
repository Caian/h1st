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

H1ST_MAKE_EXCEPTION(null_value_exception       )
H1ST_MAKE_EXCEPTION(empty_input_value_exception)
H1ST_MAKE_EXCEPTION(empty_output_exception     )
H1ST_MAKE_EXCEPTION(input_not_found_exception  )

}
