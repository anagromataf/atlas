/*
 *  atlas_shape_impl_geometry.h
 *  atlas
 *
 *  Created by Peter Fenske on 4/13/10.
 *  Copyright 2010 Fraunhofer Institut für Software- und Systemtechnik ISST.
 *
 *  This file is part of atlas.
 *	
 *	atlas is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	atlas is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with atlas.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ATLAS_SHAPE_IMPL_GEOMETRY_H_
#define _ATLAS_SHAPE_IMPL_GEOMETRY_H_

int atlas_shape_lines_intersect(struct atlas_shape_coordinate_s * result,
								struct atlas_shape_coordinate_s * l1_s,
								struct atlas_shape_coordinate_s	* l1_e,
								struct atlas_shape_coordinate_s * l2_s,
								struct atlas_shape_coordinate_s	* l2_e);

#endif // _ATLAS_SHAPE_IMPL_GEOMETRY_H_