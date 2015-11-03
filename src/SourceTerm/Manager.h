/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 * @section LICENSE
 * Copyright (c) 2015, SeisSol Group
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 **/

#ifndef SOURCETERM_MANAGER_H_
#define SOURCETERM_MANAGER_H_

#include "typedefs.hpp"
#include "NRF.h"

#include <Solver/time_stepping/TimeManager.h>
#include <Geometry/MeshReader.h>
#include <inttypes.h>

namespace seissol {
  namespace sourceterm {    
    /** Finds the tetrahedrons that contain the point sources acting at centres.
     *  In "contained" we save if the point source is contained in the mesh.
     *  We use uint8_t here as bool and MPI has a smell of trouble (see cleanDoubles).
     */
    void findMeshIds( Vector3 const*    centres,
                      MeshReader const& mesh,
                      unsigned          numSources,
                      uint8_t*          contained,
                      unsigned*         meshId );
#ifdef USE_MPI
    void cleanDoubles(uint8_t* contained, unsigned numSources);
#endif

    void transformNRFSourceToInternalSource( Vector3 const&           centre,
                                             unsigned                 element,
                                             Subfault const&          subfault,
                                             Offsets const&           offsets,
                                             Offsets const&           nextOffsets,
                                             double *const            sliprates[3],
                                             seissol::model::Material material,
                                             PointSources&            pointSources,
                                             unsigned                 index );
    class Manager;
  }
}

class seissol::sourceterm::Manager {  
private:
  ClusterMapping* cmps;
  PointSources* sources;
  
  void freeSources();

public:
  Manager() : cmps(NULL), sources(NULL) {}
  ~Manager() { freeSources(); }
  
  void mapPointSourcesToClusters( unsigned const*               meshIds,
                                  unsigned                      numberOfSources,
                                  unsigned const              (*meshToClusters)[2],
                                  unsigned const*               meshToCopyInterior,
                                  unsigned const*               copyInteriorToMesh,
                                  MeshStructure const*          meshStructure,
                                  unsigned                      numberOfClusters );
  
  /// \todo Throw this out
  void loadSourcesFromFSRM( double const*                 momentTensor,
                            int                           numberOfSources,
                            double const*                 centres,
                            double const*                 strikes,
                            double const*                 dips,
                            double const*                 rakes,
                            double const*                 onsets,
                            double const*                 areas,
                            double                        timestep,
                            int                           numberOfSamples,
                            double const*                 timeHistories,
                            MeshReader const&             mesh,
                            CellMaterialData const*       materials,
                            unsigned const              (*meshToClusters)[2],
                            unsigned const*               meshToCopyInterior,
                            unsigned const*               copyInteriorToMesh,
                            MeshStructure const*          meshStructure,
                            unsigned                      numberOfClusters,
                            time_stepping::TimeManager&   timeManager);

#ifdef USE_NETCDF
  void loadSourcesFromNRF( char const*                   fileName,
                           MeshReader const&             mesh,
                           CellMaterialData const*       materials,
                           unsigned const              (*meshToClusters)[2],
                           unsigned const*               meshToCopyInterior,
                           unsigned const*               copyInteriorToMesh,
                           MeshStructure const*          meshStructure,
                           unsigned                      numberOfClusters,
                           time_stepping::TimeManager&   timeManager );
#endif
};

#endif