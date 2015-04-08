/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2015 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_GEOMETRY_TO_PATH_HPP
#define MAPNIK_GEOMETRY_TO_PATH_HPP

#include <mapnik/geometry.hpp>
#include <mapnik/path.hpp>

namespace mapnik { namespace geometry { namespace detail {

//template <typename Transformer>
struct geometry_to_path
{
    geometry_to_path(path_type & p)
        : p_(p) {}


    void operator() (geometry_collection const& collection) const
    {
        // fixme
    }

    void operator() (geometry const& geom) const
    {
        return mapnik::util::apply_visitor(*this, geom);
    }

    // point
    void operator() (point const& pt) const
    {
        //point pt_new;
        //Transformer::apply(pt, pt_new);
        p_.move_to(pt.x, pt.y);
    }

    // line_string
    void operator() (line_string const& line) const
    {
        bool first = true;
        for (auto const& pt : line)
        {
            //point pt_new;
            //Transformer::apply(pt, pt_new);
            if (first) { p_.move_to(pt.x, pt.y); first=false;}
            else p_.line_to(pt.x, pt.y);
        }
    }

    // polygon
    void operator() (polygon const& poly) const
    {
        // exterior
        bool first = true;
        for (auto const& pt : poly.exterior_ring)
        {
            //point pt_new;
            //Transformer::apply(pt, pt_new);
            if (first) { p_.move_to(pt.x, pt.y); first=false;}
            else p_.line_to(pt.x, pt.y);
        }
        // interior
        for (auto const& ring : poly.interior_rings)
        {
            first = true;
            for (auto const& pt : ring)
            {
                //point pt_new;
                //Transformer::apply(pt, pt_new);
                if (first) { p_.move_to(pt.x, pt.y); first=false;}
                else p_.line_to(pt.x, pt.y);
            }
        }
    }

    template <typename T>
    void operator() (T const& geom) const
    {
        // no-op
    }

    path_type & p_;

};
} // ns detail

template <typename T>
void to_path(T const& geom, path_type & p)
{
    detail::geometry_to_path func(p);
    func(geom);
}

}}

#endif // MAPNIK_GEOMETRY_TO_PATH_HPP
