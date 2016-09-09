/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef GRAINTEXTUREVECTORPOSTPROCESSOR
#define GRAINTEXTUREVECTORPOSTPROCESSOR

// MOOSE includes
#include "PointSamplerBase.h"

// Forward Declarations
class GrainTextureVectorPostprocessor;

template<>
InputParameters validParams<GrainTextureVectorPostprocessor>();

class GrainTextureVectorPostprocessor : public PointSamplerBase
{
public:
  GrainTextureVectorPostprocessor(const InputParameters & parameters);

protected:
  void getPoints();
  Point pointFromIndex(const unsigned int & global_index);

  // Mesh variables
  Real _x_step;
  Real _y_step;
  Real _z_step;

  unsigned int _x_dim;
  unsigned int _y_dim;
  unsigned int _z_dim;

  Real _x_min;
  Real _y_min;
  Real _z_min;

  Real _max_refinement_level;

  std::string _index_by;
};

#endif //GRAINTEXTUREVECTORPOSTPROCESSOR_H
