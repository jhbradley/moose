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
class GrainTextureVPP;

template<>
InputParameters validParams<GrainTextureVPP>();

class GrainTextureVPP : public PointSamplerBase
{
public:
  GrainTextureVPP(const InputParameters & parameters);

protected:
  // Mesh variables
  Real _x_step;
  Real _y_step;
  Real _z_step;

  Real _x_dim;
  Real _y_dim;
  Real _z_dim;

  Real _x_min;
  Real _y_min;
  Real _z_min;

  Real _max_refinement_level;
};

#endif //GRAINTEXTUREVECTORPOSTPROCESSOR_H
