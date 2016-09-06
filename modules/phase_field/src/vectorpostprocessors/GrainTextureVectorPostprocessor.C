/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "GrainTextureVectorPostprocessor.h"
#include "EBSDMesh.h"
#include "PointSamplerBase.h"

// TODO: Figure out how to automatically sample the maximum refinement level.
// TODO: Error checking
// TODO: Make sure you delete trailing whitespace from this file before trying to merge it.

template<>
InputParameters validParams<GrainTextureVPP>()
{
InputParameters params = validParams<PointSamplerBase>();

MooseEnum sort_by("x-index-fastest z-index-fastest", "x-index-fastest");
params.addParam<MooseEnum> >("sort_by", sort_by, "How you want to sort the points in the file.");
params.addRequiredParam<Real>("max_refinement_level", "The maximum refinement level in the mesh.")

return params;
}

GrainTextureVPP::GrainTextureVPP(const InputParameters & parameters) :
  PointSamplerBase(parameters),
  _sort_by(getParam<MooseEnum>("sort_by")),
  _max_refinement_level(getParam<Real>("max_refinement_level"))
{
getPoints();

_ids.resize(_points.size());

for (unsigned int i=0; i<_points.size(); i++)
  _ids[i] = i;
}

void
GrainTextureVectorPostprocessor::getPoints()
{
// Get the EBSDMesh dimensions
const EBSDMeshGeometry & ebsd_geometry = EBSDMesh::getEBSDGeometry();         // I don't know if I need the const here.

_x_step = ebsd_geometry.d[0];
_y_step = ebsd_geometry.d[1];
_z_step = ebsd_geometry.d[2];

_x_dim = ebsd_geometry.n[0];
_y_dim = ebsd_geometry.n[1];
_z_dim = ebsd_geometry.n[2];

_x_min = ebsd_geometry.min[0];
_y_min = ebsd_geometry.min[1];
_z_min = ebsd_geometry.min[2];

// Increase the dimensions and cut the step size for any uniform refinements
Real change_factor = std::pow(2, _max_refinement_level);

_x_dim = _x_dim * change_factor;
_y_dim = _y_dim * change_factor;
_z_dim = _z_dim * change_factor;

_x_step = _x_step / change_factor;
_y_step = _y_step / change_factor;
_z_step = _z_step / change_factor;

// Determine the coordinates of the sampling points
unsigned int num_points = _x_dim * _y_dim * _z_dim;
_points.resize(num_points);

for (unsigned int global_index = 0; global_index < num_points; ++global_index)
{
  // Step through each point
  Point tmp_point = pointFromIndex(global_index);

  _points.push_back(tmp_point);                                               // You are going to need to figure out the most efficient way to do this, i.e., by reference.
}
}


Point
newGrainTextureVPP::pointFromIndex(const unsigned int & global_index)
{
// TODO: Be sure to add in error checking here.
                                                                              // Make sure to declare unsigned int _sort_by in header file.
switch (_sort_by)
{
  // x_index increasing fastest, then y, then z
  case "x-index-fastest":
    // Get x_index
    unsigned int x_index;
    x_index = global_index % _x_dim;

    // Get y_index
    unsigned int helper_index;
    helper_index = global_index / _x_dim; // Make use of integer math.

    unsigned int y_index;
    y_index = helper_index % _y_dim;

    // Get z_index
    unsigned int z_index;
    z_index = global_index / (_x_dim * _y_dim); // Integer math once again.

    break;

  // z_index increasing fastest, then y, then x
  case "z-index-fastest":
    // Get z_index
    unsigned int z_index;
    z_index = global_index % _z_dim;

    // Get y_index
    unsigned int helper_index;
    helper_index = global_index / _z_dim;

    unsigned int y_index;
    y_index = helper_index % _y_dim;

    // Get x_index
    unsigned int x_index;
    x_index = global_index / (_z_dim * _y_dim);
}

// Calculate point based on indices. We want to sample what would be the centroid
// of each element were we to oversample the mesh at the refinement level of
// the smallest elements.
x_coord = _x_min + _x_step / 2.0 + (_x_step * x_index);
y_coord = _y_min + _y_step / 2.0 + (_y_step * y_index);
z_coord = _z_min + _z_step / 2.0 + (_z_step * z_index);

Point point(x_coord, y_coord, z_coord);

return point;
}
