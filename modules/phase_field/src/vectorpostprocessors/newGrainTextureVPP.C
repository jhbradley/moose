                                                                                // TODO: Error checking
                                                                                // TODO: Make sure you delete trailing whitespace from this file before trying to merge it.
#include "newGrainTextureVPP.h"
#include "EBSDMesh.h"

void
GrainTextureVectorPostprocessor::execute()
{
  // Get the EBSDMesh dimensions
  const EBSDMeshGeometry & ebsd_geometry = EBSDMesh::getEBSDGeometry();         // I don't know if I need the const here.

  _x_step = ebsd_geometry.d[0];                                                 // Declare these member variables in header (Reals)
  _y_step = ebsd_geometry.d[1];
  _z_step = ebsd_geometry.d[2];

  _x_dim = ebsd_geometry.n[0];
  _y_dim = ebsd_geometry.n[1];
  _z_dim = ebsd_geometry.n[2];

  _x_min = ebsd_geometry.min[0];
  _y_min = ebsd_geometry.min[1];
  _z_min = ebsd_geometry.min[2];

  // Increase the dimensions and cut the step size for any uniform refinements
  Real ur_level = uniform_refine;
  Real change_factor = std::pow(2, ur_level);

  _x_dim = _x_dim * change_factor;
  _y_dim = _y_dim * change_factor;
  _z_dim = _z_dim * change_factor;

  _x_step = _x_step / change_factor;
  _y_step = _y_step / change_factor;
  _z_step = _z_step / change_factor;

  // Determine the coordinates of the sampling points
  _num_points = _x_dim * _y_dim * _z_dim;                                       // Make sure to declare the member variable (Real _num_points) in the header file.
  _points.resize(_num_points);                                                  // Make sure to declare the member variable (std::vector<Point> _points) in the header file.

  for (_global_index = 0; _global_index < _num_points; ++_global_index)         // Make sure to declare the member variable (unsigned int _global_index) in the header file.
  {
    // Step through each point                                                  // I think you can do this so that you don't need an "indexFromPoint" function.
    Point tmp_point = pointFromIndex();

    _points.push_back(tmp_point);                                               // You are going to need to figure out the most efficient way to do this, i.e., by reference.
  }
}

const Point
newGrainTextureVPP::pointFromIndex() // We just need the global index
{
  // TODO: Be sure to add in error checking here.
                                                                                // Make sure to declare unsigned int _sort_by in header file.
  switch (_sort_by)
  {
    // x_index increasing fastest, then y, then z
    case 1:
      // Get x_index
      unsigned int x_index;
      x_index = _global_index % _x_dim;

      // Get y_index
      unsigned int helper_index;
      helper_index = _global_index / _x_dim; // Make use of integer math.

      unsigned int y_index;
      y_index = helper_index % _y_dim;

      // Get z_index
      unsigned int z_index;
      z_index = _global_index / (_x_dim * _y_dim); // Integer math once again.

    // z_index increasing fastest, then y, then x
    case 2:
      // Get z_index
      unsigned int z_index;
      z_index = _global_index % _z_dim;

      // Get y_index
      unsigned int helper_index;
      helper_index = _global_index / _z_dim;

      unsigned int y_index;
      y_index = helper_index % _y_dim;

      // Get x_index
      unsigned int x_index;
      x_index = _global_index / (_z_dim * _y_dim);
  }

  // Calculate point based on indices. We want to sample what would be the centroid
  // of each element were we to oversample the mesh at the refinement level of
  // the smallest elements.
  x_coord = _x_min + _x_step / 2.0 + (_x_step * x_index);
  y_coord = _y_min + _y_step / 2.0 + (_y_step * y_index);
  z_coord = _z_min + _z_step / 2.0 + (_z_step * z_index);

  // Make sure that you are

  Point point(x_coord, y_coord, z_coord);
}
