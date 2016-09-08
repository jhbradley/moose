/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

// TODO: Figure out how to automatically sample the maximum refinement level.
// TODO: Error checking
// TODO: Make sure you delete trailing whitespace from this file before trying to merge it.

#include "GrainTextureVectorPostprocessor.h"
#include "EBSDMesh.h"
#include "PointSamplerBase.h"

template<>
InputParameters validParams<GrainTextureVectorPostprocessor>()
{
InputParameters params = validParams<PointSamplerBase>();

params.addRequiredParam<Real>("max_refinement_level", "The maximum refinement level in the mesh.");
MooseEnum sort_by("x-index-fastest z-index-fastest", "x-index-fastest");
params.addParam<MooseEnum >("sort_by", sort_by, "How you want to sort the points in the file.");

return params;
}

GrainTextureVectorPostprocessor::GrainTextureVectorPostprocessor(const InputParameters & parameters) :
    PointSamplerBase(parameters),
    _max_refinement_level(getParam<Real>("max_refinement_level")),
    _sort_by(getParam<MooseEnum>("sort_by"))
{
  getPoints();

  _ids.resize(_points.size());

  for (unsigned int i=0; i<_points.size(); i++)
    _ids[i] = i;
}

void
GrainTextureVectorPostprocessor::getPoints()
{
  // Fetch and check mesh
  EBSDMesh * mesh = dynamic_cast<EBSDMesh *>(&_mesh);
  if (mesh == NULL)
    mooseError("Please use an EBSDMesh in your simulation.");

  const EBSDMesh::EBSDMeshGeometry & ebsd_geometry = mesh->getEBSDGeometry();

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
  Real change_factor = std::pow(2, _max_refinement_level); // TODO: check to see if this should be an int

  _x_dim = _x_dim * change_factor;
  _y_dim = _y_dim * change_factor;
  _z_dim = _z_dim * change_factor;

  _x_step = _x_step / change_factor;
  _y_step = _y_step / change_factor;
  _z_step = _z_step / change_factor;

  // Determine the coordinates of the sampling points
  unsigned int num_points = _x_dim * _y_dim * (_z_dim == 0 ? 1 : _z_dim);
  _points.resize(num_points);

  for (unsigned int global_index = 0; global_index < num_points; ++global_index)
  {
    // Step through each point
    Point tmp_point = pointFromIndex(global_index);

    _points.push_back(tmp_point); // TODO: Is this the most efficient thing?
  }
}

Point
GrainTextureVectorPostprocessor::pointFromIndex(const unsigned int & global_index)
{
  // TODO: Be sure to add in error checking here.

  unsigned int x_index, y_index, z_index, helper_index;

  unsigned int sort_by;

  if (_sort_by.compare("x-index-fastest"))
    sort_by = 1;
  else
    sort_by = 2;

  switch (sort_by)
  {
    // x_index increasing fastest, then y, then z
    case 1:

      if (_z_dim != 0)
      {
        // Get x_index
        x_index = global_index % _x_dim;

        // Get y_index
        helper_index = global_index / _x_dim; // Make use of integer math.
        y_index = helper_index % _y_dim;

        // Get z_index
        z_index = global_index / (_x_dim * _y_dim); // Integer math once again.
      }
      else
      {
        /* WIP */
      }

      break;

    // z_index increasing fastest, then y, then x
    case 2:
      // Get z_index
      if (_z_dim != 0)
      {
        z_index = global_index % _z_dim;

        // Get y_index
        helper_index = global_index / _z_dim;
        y_index = helper_index % _y_dim;

        // Get x_index
        x_index = global_index / (_z_dim * _y_dim);
      }
      else
      {
        /* WIP */
      }
  }

  // Calculate point based on indices. We want to sample what would be the centroid
  // of each element were we to oversample the mesh at the refinement level of
  // the smallest elements.
  Real x_coord, y_coord, z_coord;

  x_coord = _x_min + _x_step / 2.0 + (_x_step * x_index);
  y_coord = _y_min + _y_step / 2.0 + (_y_step * y_index);
  z_coord = _z_min + _z_step / 2.0 + (_z_step * z_index);

  Point point(x_coord, y_coord, z_coord);

  return point;
}
