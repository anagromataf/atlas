# Atlas

Atlas is a library for storing information about moving objects based on spatio-temporal situation descriptions.

## Conventions for Operations and Geometric Objects

### Lines vs. Arcs
The terms lines and arcs are used. In the implementation there are only arcs. The connecting edge between to coordinates is a line. Therefore an arc consists of lines (= line segments).

### Arcs
An arc must not have two intersecting line segments. Two line segments may be on top of each other.

### Polygons
A polygon must be simple. That means, that no two edges intersect. Of course the vertexes of two neighboring edges share the same coordinate. Two edges may be on top of each other but must not intersect.

When defining a polygon, the coordinates must be listed in a counter-clockwise way.

Polygons can be nested. To exclude the area of a polygon from the surrounding polygon, it must be located inside. When further checks are applied, i.e. if a point belongs to the area of the (outside) polygon, the convention is, that edges always belong to the inside part.
Example: One polygon that contains a second polygon inside, thus, excluding the area. If the point is on top of an edge of the outside polygon, it is *inside*. If the point is on top of an edge of the second inside polygon, it is *not inside*.

### Precision of Doubles
In the geometry file (atlas_shape_impl_geometry) doubles need to be compared. Doubles appear to have a limited precision.

The equality of two doubles is checked by calculating the difference, taking the absolute value and then comparing the difference by larger-than 1.0E-15.

Example: Given the lines ((-9,-9),(10,10)) and ((-6,4),(2,0)) the equations are f(x)=x and g(x)=-x/2+1. Setting those equal x is 2/3 and y is - x substituted in f(x) and g(x) - 2/3. The computer does not work with fractions. Having used this test case the maximum precision to compare the absolute value of the difference with is 1.0E-15.



## atlas_shape_impl

### atlas_shape_data_s
This struct holds the information about a shape. It contains the number of coordinates, the number of parts and additionally a data part. Within the data part first a list of atlas_shape_part_info_s exists. The length of that list depends on how many parts a shape has, but there has to be at least one. Following the part information all coordinates are located in the data part.

### atlas_shape_part_info_s
A part info has information of the type of a part (point, arc, polygon) and the index (starting at 0) at which the coordinates start in the last section of the data part.

### atlas_shape_create
This creates a new shape and returns a lazy object.

#### TODO
Still todo are checks for useful inputs and consistency. Those include:
1. (Complete) arcs must not intersect
2. Polygons must be simple
3. If two points are equal (exactly on top of each other) one can be ignored
4. If two arcs are exactly equal (same starting and ending coordinates) one can be ignored. This shall not apply to line segments. For example two arcs might share a segment, but this segment is necessary for the definition of both. Therefore this line segment cannot be ignored.

### atlas_shape_get_number_of_parts and atlas_shape_get_number_of_coords
Simply returns the information stored in atlas_shape_data_s.

### atlas_shape_get_type_of_part and atlas_shape_get_start_of_part
A shape might consist of multiple parts. When either the type of part or the number of coordinates in a part is to be read, the index (starting at 0) is passed to this function and will return the information.

### atlas_shape_create_intersection

### atlas_shape_union
Currently union only creates a new shape and adds all parts (including coordinates) from both input shapes.

#### TODO
The function needs to be changed, such that all points form a list unless on top of each other. 

Arcs also form a list, but if two arcs are exactly the same, only one is kept.

Polygons must either form a list also, but if they pairwise intersect one new polygon needs to be formed. There is also the case, when a polygon fully contains another polygon, such that the inside polygon is not part of the resulting area.

### atlas_shape_create_difference

### atlas_shape_is_equal
To compare two shapes for equality, the parts within each shape have to be compared. If one equal part exists in both shapes, it might not be at the same index.
Another possibility is, that two actually equal polygons might have a different first coordinate.
Two arcs might also be equal, but one arc (straight line) is only defined by two coordinates and the other of a list of coordinates.
Points however are equal, when their coordinates are simply equal.

### atlas_shape_intersects

### atlas_shape_is_inside and atlas_shape_is_outside
This function takes two shapes as parameters. The convention shall be, that the first shape is checked if it is inside of the seconds shape, as in reading "is shape 1 inside of shape 2".

To check, whether a shape is inside another shape, there must be a check, if parts are inside or not. The following cases may occur:
1. Point/Arc: A point must be on top of a line segment of the arc
2. Point/Polygon: The point must be located inside the polygon or on top of an edge.
3. Arc/Arc: The first arc must be on top of the second arc and be equal in length or shorter.
4. Arc/Polygon: This reduces to Point/Polygon. No vertex of an arc can be located outside of the polygon.
5. Polygon/Polygon: Also reduces to Point/Polygon.

There might be the case, when within a single shape multiple polygons are defined, that are inside of each other. This means, that the second polygon is not part of the resulting area. If a point of either an arc or polygon from the second shape is located within this "inside polygon" it will not be inside.

### atlas_shape_is_adjacent
The cases are analog to the check for inside.

1. Point/Arc: A point is defined as adjacent to an arc of the point is on top of a line segment.
2. Point/Polygon: Same as Point/Line, but not inside.
3. Arc/Arc: Arcs are adjacent to each other, when they are either inside **or** share a line segment.
4. Arc/Polygon: Same as Arc/Arc.
5. Polygon/Polygon: Same as Arc/Polygon.




## atlas_shape_impl_geometry

This file contains helper functions for geometry, for example checking if lines intersect and so on.

### atlas_shape_lines_intersect
To check if two lines (line segments, from point to point) intersect, this function is used. The following cases for two lines can occur:
1. parallel and not on top of each other
2. parallel and on top of each other
3. intersecting

Depending on the case, the returned value (`return`) differs. `coordinate` defines a variable, which address is passed as a parameter.
1. `return` = 0
2. `return` = 1
3. `return` = 1 and `coordinate` is set to the point of intersection