/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Alex Breuer (breuer AT mytum.de, http://www5.in.tum.de/wiki/index.php/Dipl.-Math._Alexander_Breuer)
 * @author Sebastian Rettenberger (sebastian.rettenberger @ tum.de, http://www5.in.tum.de/wiki/index.php/Sebastian_Rettenberger)
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
 * C++/Fortran-interoperability.
 **/

#include <cstddef>
#include <cstring>

#include "Interoperability.h"
#include "time_stepping/TimeManager.h"
#include "SeisSol.h"
#include <Initializer/CellLocalMatrices.h>
#include <Model/Setup.h>

seissol::Interoperability e_interoperability;

/*
 * C bindings
 */
extern "C" {
  // fortran to c
  void c_interoperability_setDomain( void *i_domain ) {
    e_interoperability.setDomain( i_domain );
  }

  void c_interoperability_setTimeStepWidth( int    i_meshId,
                                            double i_timeStepWidth ) {
    e_interoperability.setTimeStepWidth( i_meshId,
                                         i_timeStepWidth );
  }

  void c_interoperability_initializeClusteredLts( int i_clustering ) {
    e_interoperability.initializeClusteredLts( i_clustering );
  }
  
  void c_interoperability_setupNRFPointSources(char* nrfFileName)
  {
#ifdef USE_NETCDF
    e_interoperability.setupNRFPointSources(nrfFileName);
#endif
  }
  
  void c_interoperability_setupFSRMPointSources( double*  momentTensor,
                                                 int      numberOfSources,
                                                 double*  centres,
                                                 double*  strikes,
                                                 double*  dips,
                                                 double*  rakes,
                                                 double*  onsets,
                                                 double*  areas,
                                                 double   timestep,
                                                 int      numberOfSamples,
                                                 double*  timeHistories )
  {
    e_interoperability.setupFSRMPointSources( momentTensor, 
                                              numberOfSources,
                                              centres,
                                              strikes,
                                              dips,
                                              rakes,
                                              onsets,
                                              areas,
                                              timestep,
                                              numberOfSamples,
                                              timeHistories );
  }

  void c_interoperability_addReceiver( int i_receiverId,
                                       int i_meshId ) {
    e_interoperability.addReceiver( i_receiverId,
                                    i_meshId );
  }

  void c_interoperability_setReceiverSampling( double i_receiverSampling ) {
    e_interoperability.setReceiverSampling( i_receiverSampling );
  }

  void c_interoperability_enableDynamicRupture() {
    e_interoperability.enableDynamicRupture();
  }
  
  void c_interoperability_setMaterial( int    i_meshId,
                                       int    i_side,
                                       double* i_materialVal,
                                       int    i_numMaterialVals ) {
    e_interoperability.setMaterial(i_meshId, i_side, i_materialVal, i_numMaterialVals);
  }

#ifdef USE_PLASTICITY
 void c_interoperability_setInitialLoading( int    *i_meshId,
                                            double *i_initialLoading ) {
    e_interoperability.setInitialLoading( i_meshId, i_initialLoading );
  }
#endif
  
  void c_interoperability_initializeCellLocalMatrices() {
    e_interoperability.initializeCellLocalMatrices();
  }

  void c_interoperability_synchronizeCellLocalData() {
    e_interoperability.synchronizeCellLocalData();
  }

  void c_interoperability_synchronizeCopyLayerDofs() {
    e_interoperability.synchronizeCopyLayerDofs();
  }

  void c_interoperability_enableWaveFieldOutput( double i_waveFieldInterval, const char* i_waveFieldFilename ) {
    e_interoperability.enableWaveFieldOutput( i_waveFieldInterval, i_waveFieldFilename );
  }

  void c_interoperability_enableCheckPointing( double i_checkPointInterval,
		  const char* i_checkPointFilename, const char* i_checkPointBackend ) {
    e_interoperability.enableCheckPointing( i_checkPointInterval,
    		i_checkPointFilename, i_checkPointBackend );
  }

  void c_interoperability_initializeIO( double* mu, double* slipRate1, double* slipRate2,
		  double* slip, double* slip1, double* slip2, double* state, double* strength,
		  int numSides, int numBndGP, int refinement) {
	  e_interoperability.initializeIO(mu, slipRate1, slipRate2, slip, slip1, slip2, state, strength,
			  numSides, numBndGP, refinement);
  }

  void c_interoperability_addToDofs( int    i_meshId,
                                     double i_update[NUMBER_OF_DOFS] ) {
    e_interoperability.addToDofs( i_meshId, i_update );
  }

  void c_interoperability_getTimeDerivatives( int    i_meshId,
                                              double  o_timeDerivatives[CONVERGENCE_ORDER][NUMBER_OF_DOFS] ) {
    e_interoperability.getTimeDerivatives( i_meshId,
                                           o_timeDerivatives );
  }

  void c_interoperability_getFaceDerInt( int    i_meshId,
                                         int    i_localFaceId,
                                         double i_timeStepWidth,
                                         double o_timeDerivativesCell[CONVERGENCE_ORDER][NUMBER_OF_DOFS],
                                         double o_timeDerivativesNeighbor[CONVERGENCE_ORDER][NUMBER_OF_DOFS],
                                         double o_timeIntegratedCell[NUMBER_OF_DOFS],
                                         double o_timeIntegratedNeighbor[NUMBER_OF_DOFS] ) {
    e_interoperability.getFaceDerInt( i_meshId,
                                      i_localFaceId,
                                      i_timeStepWidth,
                                      o_timeDerivativesCell,
                                      o_timeDerivativesNeighbor,
                                      o_timeIntegratedCell,
                                      o_timeIntegratedNeighbor );
  }

  void c_interoperability_getDofs( int    i_meshId,
                                   double o_timeDerivatives[NUMBER_OF_DOFS] ) {
    e_interoperability.getDofs( i_meshId, o_timeDerivatives );
  }

  void c_interoperability_getDofsFromDerivatives( int    i_meshId,
                                                  double o_dofs[NUMBER_OF_DOFS] ) {
    e_interoperability.getDofsFromDerivatives( i_meshId, o_dofs );
  }

  void c_interoperability_getNeighborDofsFromDerivatives( int    i_meshId,
                                                          int    i_localFaceId,
                                                          double o_dofs[NUMBER_OF_DOFS] ) {
    e_interoperability.getNeighborDofsFromDerivatives( i_meshId, i_localFaceId, o_dofs );
  }

  void c_interoperability_simulate( double i_finalTime ) {
    e_interoperability.simulate( i_finalTime );
  }

  void c_interoperability_finalizeIO() {
	  e_interoperability.finalizeIO();
  }

  // c to fortran
  extern void f_interoperability_computeSource(  void   *i_domain,
                                                 int    *i_meshId,
                                                 double *i_fullUpdateTime,
                                                 double *i_timeStepWidth );

  extern void f_interoperability_setDynamicRuptureTimeStep( void *i_domain,
		   	   	   	   	   	   	   	   	   	   	   	   	    int  *i_timeStep );

  extern void f_interoperability_getDynamicRuptureTimeStep( void *i_domain,
		  	  	  	  	  	  	  	  	  	  	  	  	    int  *i_timeStep );

  extern void f_interoperability_computeDynamicRupture( void   *i_domain,
                                                        double *i_fullUpdateTime,
                                                        double *i_timeStepWidth );

  extern void f_interoperability_computePlasticity( void    *i_domain,
                                                    double  *i_timestep,
													int    numberOfAlignedBasisFunctions,
                                                    double (*i_initialLoading)[NUMBER_OF_BASIS_FUNCTIONS],
                                                    double  *io_dofs );


  extern void f_interoperability_writeReceivers( void   *i_domain,
                                                 double *i_fullUpdateTime,
                                                 double *i_timeStepWidth,
                                                 double *i_receiverTime,
                                                 int    *i_numberOfReceivers, 
                                                 int    *i_receiverIds );
                                                 
  extern void f_interoperability_computeMInvJInvPhisAtSources( void*    i_domain,
                                                               double   i_x,
                                                               double   i_y,
                                                               double   i_z,
                                                               int      i_elem,
                                                               double*  o_mInvJInvPhisAtSources );
}

/*
 * C++ functions
 */
seissol::Interoperability::Interoperability() :
  m_domain(NULL) // reset domain pointer
{
}

seissol::Interoperability::~Interoperability()
{
}

void seissol::Interoperability::setDomain( void* i_domain ) {
  assert( i_domain != NULL );

  m_domain = i_domain;
}

void seissol::Interoperability::setTimeStepWidth( int    i_meshId,
                                                  double i_timeStepWidth ) {
  seissol::SeisSol::main.getLtsLayout().setTimeStepWidth( (i_meshId)-1, i_timeStepWidth );
}

void seissol::Interoperability::initializeClusteredLts( int i_clustering ) {
  // assert a valid clustering
  assert( i_clustering > 0 );

  // either derive a GTS or LTS layout
  if( i_clustering == 1 ) {
    seissol::SeisSol::main.getLtsLayout().deriveLayout( single, 1);
  }
  else {
    seissol::SeisSol::main.getLtsLayout().deriveLayout( multiRate, i_clustering );
  }

  // get cell information & mappings
  seissol::SeisSol::main.getLtsLayout().getCellInformation( m_numberOfMeshCells,
                                                            m_numberOfLtsCells,
                                                            m_numberOfCopyInteriorCells,
                                                            m_cellInformation,
                                                            m_meshToLts,
                                                            m_meshToCopyInterior,
                                                            m_meshToClusters,
                                                            m_ltsToMesh,
                                                            m_copyInteriorToMesh );

  // get the mesh structure
  seissol::SeisSol::main.getLtsLayout().getMeshStructure( m_meshStructure );

  // get time stepping
  seissol::SeisSol::main.getLtsLayout().getCrossClusterTimeStepping( m_timeStepping );

  // add clusters
  seissol::SeisSol::main.timeManager().addClusters( m_timeStepping,
                                                    m_meshStructure,
                                                    m_cellInformation,
                                                    m_meshToClusters );

  // get backward coupling
  seissol::SeisSol::main.timeManager().getRawData( m_globalData,
                                                   m_cellData,
                                                   m_dofs,
                                                   m_buffers,
                                                   m_derivatives,
                                                   m_faceNeighbors );
}

#ifdef USE_NETCDF
void seissol::Interoperability::setupNRFPointSources( char const* fileName )
{
  SeisSol::main.sourceTermManager().loadSourcesFromNRF(
    fileName,
    seissol::SeisSol::main.meshReader(),
    m_cellData->material,
    m_meshToClusters,
    m_meshToCopyInterior,
    m_copyInteriorToMesh,
    m_meshStructure,
    m_timeStepping.numberOfLocalClusters,
    seissol::SeisSol::main.timeManager()
  );
}
#endif

void seissol::Interoperability::setupFSRMPointSources( double const* momentTensor,
                                                       int           numberOfSources,
                                                       double const* centres,
                                                       double const* strikes,
                                                       double const* dips,
                                                       double const* rakes,
                                                       double const* onsets,
                                                       double const* areas,
                                                       double        timestep,
                                                       int           numberOfSamples,
                                                       double const* timeHistories )
{
  SeisSol::main.sourceTermManager().loadSourcesFromFSRM(
    momentTensor, 
    numberOfSources,
    centres,
    strikes,
    dips,
    rakes,
    onsets,
    areas,
    timestep,
    numberOfSamples,
    timeHistories,
    seissol::SeisSol::main.meshReader(),
    m_cellData->material,
    m_meshToClusters,
    m_meshToCopyInterior,
    m_copyInteriorToMesh,
    m_meshStructure,
    m_timeStepping.numberOfLocalClusters,
    seissol::SeisSol::main.timeManager()
  );
}

void seissol::Interoperability::addReceiver( int i_receiverId,
                                             int i_meshId ) {
  seissol::SeisSol::main.timeManager().addReceiver( i_receiverId, i_meshId );
}

void seissol::Interoperability::setReceiverSampling( double i_receiverSampling ) {
  assert( i_receiverSampling > 0 );

  seissol::SeisSol::main.timeManager().setReceiverSampling( i_receiverSampling );
}

void seissol::Interoperability::enableDynamicRupture() {
  seissol::SeisSol::main.timeManager().enableDynamicRupture();
}

void seissol::Interoperability::setMaterial(int i_meshId, int i_side, double* i_materialVal, int i_numMaterialVals)
{
  unsigned int copyInteriorId = m_meshToCopyInterior[i_meshId - 1];
  int side = i_side - 1;
  seissol::model::Material* material;
  
  if (side < 0) {
    material = &m_cellData->material[copyInteriorId].local;
  } else {
    assert(side < 4);
    material = &m_cellData->material[copyInteriorId].neighbor[side];
  }
  
  seissol::model::setMaterial(i_materialVal, i_numMaterialVals, material);
}

#ifdef USE_PLASTICITY
void seissol::Interoperability::setInitialLoading( int* i_meshId, double *i_initialLoading ) {\
  unsigned int l_copyInteriorId = m_meshToCopyInterior[(*i_meshId) - 1];

  for( unsigned int l_stress = 0; l_stress < 6; l_stress++ ) {
    for( unsigned int l_basis = 0; l_basis < NUMBER_OF_BASIS_FUNCTIONS; l_basis++ ) {
      m_cellData->plasticity[l_copyInteriorId].initialLoading[l_stress][l_basis] = i_initialLoading[ l_stress*NUMBER_OF_BASIS_FUNCTIONS + l_basis ];
    }
  }
}
#endif

void seissol::Interoperability::initializeCellLocalMatrices()
{
  // \todo Move this to some common initialization place
  seissol::initializers::initializeCellLocalMatrices( seissol::SeisSol::main.meshReader(),
                                                      m_copyInteriorToMesh,
                                                      m_meshToLts,
                                                      m_numberOfCopyInteriorCells,
                                                      m_cellInformation,
                                                      m_cellData );
}

void seissol::Interoperability::synchronizeCellLocalData() {
  // iterate over the mesh and set all redundant data
#ifdef _OPENMP
  #pragma omp parallel for schedule(static)
#endif
  for( unsigned int l_cell = 0; l_cell < m_numberOfCopyInteriorCells; l_cell++ ) {
    unsigned sourceId = m_meshToCopyInterior[m_copyInteriorToMesh[l_cell]];
    m_cellData->material[l_cell].local = m_cellData->material[sourceId].local;
    for (unsigned side = 0; side < 4; ++side) {
      m_cellData->material[l_cell].neighbor[side] = m_cellData->material[sourceId].neighbor[side];
    }

#ifdef USE_PLASTICITY
    // sync initial loading
    for( unsigned int l_quantity = 0; l_quantity < 6; l_quantity++ ) {
      for( unsigned int l_basis = 0; l_basis < NUMBER_OF_BASIS_FUNCTIONS; l_basis++ ) {
        m_cellData->plasticity[l_cell].initialLoading[l_quantity][l_basis] = m_cellData->plasticity[sourceId].initialLoading[l_quantity][l_basis];
      }
    }
#endif

  }
}

void seissol::Interoperability::synchronizeCopyLayerDofs() {
  unsigned int l_offset = 0;

  for( unsigned int l_cluster = 0; l_cluster < m_timeStepping.numberOfLocalClusters; l_cluster++ ) {
    // sync DOFs
#ifdef _OPENMP
  #pragma omp parallel for schedule(static)
#endif
    for( unsigned int l_cell = 0; l_cell < m_meshStructure[l_cluster].numberOfCopyCells; l_cell++ ) {
      for( int l_dof = 0; l_dof < NUMBER_OF_ALIGNED_DOFS; l_dof++ ) {
        m_dofs[l_cell+l_offset][l_dof] = m_dofs[ m_meshToCopyInterior[m_copyInteriorToMesh[l_cell+l_offset]] ][l_dof];
      }
    }

    // update offset
    l_offset += m_meshStructure[l_cluster].numberOfCopyCells + m_meshStructure[l_cluster].numberOfInteriorCells;
  }
}

void seissol::Interoperability::enableWaveFieldOutput( double i_waveFieldInterval, const char *i_waveFieldFilename ) {
  seissol::SeisSol::main.simulator().setWaveFieldInterval( i_waveFieldInterval );
  seissol::SeisSol::main.waveFieldWriter().enable();
  seissol::SeisSol::main.waveFieldWriter().setFilename( i_waveFieldFilename );
}

void seissol::Interoperability::enableCheckPointing( double i_checkPointInterval,
		const char *i_checkPointFilename, const char *i_checkPointBackend ) {
  seissol::SeisSol::main.simulator().setCheckPointInterval( i_checkPointInterval );
  if (strcmp(i_checkPointBackend, "posix") == 0)
	  seissol::SeisSol::main.checkPointManager().setBackend(checkpoint::POSIX);
  else if (strcmp(i_checkPointBackend, "hdf5") == 0)
	  seissol::SeisSol::main.checkPointManager().setBackend(checkpoint::HDF5);
  else if (strcmp(i_checkPointBackend, "mpio") == 0)
	  seissol::SeisSol::main.checkPointManager().setBackend(checkpoint::MPIO);
  else if (strcmp(i_checkPointBackend, "mpio_async") == 0)
	  seissol::SeisSol::main.checkPointManager().setBackend(checkpoint::MPIO_ASYNC);
  else if (strcmp(i_checkPointBackend, "sionlib") == 0)
	  seissol::SeisSol::main.checkPointManager().setBackend(checkpoint::SIONLIB);
  else
	  logError() << "Unknown checkpoint backend";
  	  seissol::SeisSol::main.checkPointManager().setFilename( i_checkPointFilename );
}

void seissol::Interoperability::initializeIO(
		  double* mu, double* slipRate1, double* slipRate2,
		  double* slip, double* slip1, double* slip2, double* state, double* strength,
		  int numSides, int numBndGP, int refinement)
{
	  // Initialize checkpointing
	  double currentTime;
	  int waveFieldTimeStep = 0;
	  int faultTimeStep;
	  bool hasCheckpoint = seissol::SeisSol::main.checkPointManager().init(reinterpret_cast<real*>(m_dofs),
				  m_numberOfCopyInteriorCells * NUMBER_OF_ALIGNED_DOFS,
				  mu, slipRate1, slipRate2, slip, slip1, slip2,
				  state, strength, numSides, numBndGP,
				  currentTime, waveFieldTimeStep, faultTimeStep);
	  if (hasCheckpoint) {
		  seissol::SeisSol::main.simulator().setCurrentTime(currentTime);
		  f_interoperability_setDynamicRuptureTimeStep(m_domain, &faultTimeStep);
	  }

	  // Initialize wave field output
	  seissol::SeisSol::main.waveFieldWriter().init(
			  NUMBER_OF_QUANTITIES, CONVERGENCE_ORDER,
              NUMBER_OF_ALIGNED_BASIS_FUNCTIONS,
			  seissol::SeisSol::main.meshReader(),
			  reinterpret_cast<const double*>(m_dofs), m_meshToCopyInterior,
			  refinement, waveFieldTimeStep,
			  seissol::SeisSol::main.timeManager().getTimeTolerance());

	  // I/O initialization is the last step that requires the mesh reader
	  // (at least at the moment ...)
	  seissol::SeisSol::main.freeMeshReader();
}

void seissol::Interoperability::getDynamicRuptureTimeStep(int &o_timeStep)
{
	f_interoperability_getDynamicRuptureTimeStep(m_domain, &o_timeStep);
}

void seissol::Interoperability::addToDofs( int    i_meshId,
                                           double i_update[NUMBER_OF_DOFS] ) {
  seissol::kernels::addToAlignedDofs( i_update, m_dofs[ m_meshToCopyInterior[(i_meshId)-1] ] );
}

void seissol::Interoperability::getTimeDerivatives( int    i_meshId,
                                                    double  o_timeDerivatives[CONVERGENCE_ORDER][NUMBER_OF_DOFS] ) {
  real l_timeIntegrated[NUMBER_OF_ALIGNED_DOFS] __attribute__((aligned(ALIGNMENT)));
  real l_timeDerivatives[NUMBER_OF_ALIGNED_DERS] __attribute__((aligned(ALIGNMENT)));

  m_timeKernel.computeAder( 0,
                            m_globalData->stiffnessMatricesTransposed,
                            m_dofs[ m_meshToCopyInterior[(i_meshId)-1] ],
                            m_cellData->localIntegration[ m_meshToCopyInterior[ (i_meshId)-1] ].starMatrices,
#ifdef REQUIRE_SOURCE_MATRIX
                            m_cellData->localIntegration[ m_meshToCopyInterior[ (i_meshId)-1] ].sourceMatrix,
#endif
                            l_timeIntegrated,
                            l_timeDerivatives );

// We cannot use derivative compression with a source matrix
#ifdef REQUIRE_SOURCE_MATRIX
  for (unsigned order = 0; order < CONVERGENCE_ORDER; ++order) {
    seissol::kernels::copySubMatrix( &l_timeDerivatives[order * NUMBER_OF_ALIGNED_DOFS],
                                     NUMBER_OF_BASIS_FUNCTIONS,
                                     NUMBER_OF_QUANTITIES,
                                     NUMBER_OF_ALIGNED_BASIS_FUNCTIONS,
                                     o_timeDerivatives[order],
                                     NUMBER_OF_BASIS_FUNCTIONS,
                                     NUMBER_OF_QUANTITIES,
                                     NUMBER_OF_BASIS_FUNCTIONS );
  }
#else
  seissol::kernels::convertAlignedCompressedTimeDerivatives( l_timeDerivatives,
                                                             o_timeDerivatives );
#endif
}

void seissol::Interoperability::getFaceDerInt( int    i_meshId,
                                               int    i_localFaceId,
                                               double i_timeStepWidth,
                                               double o_timeDerivativesCell[CONVERGENCE_ORDER][NUMBER_OF_DOFS],
                                               double o_timeDerivativesNeighbor[CONVERGENCE_ORDER][NUMBER_OF_DOFS],
                                               double o_timeIntegratedCell[NUMBER_OF_DOFS],
                                               double o_timeIntegratedNeighbor[NUMBER_OF_DOFS] ) {
  // assert that the cell provides derivatives
  assert( (m_cellInformation[ m_meshToLts[ (i_meshId)-1 ] ].ltsSetup >> 9)%2 == 1 );

  // get cells derivatives
  seissol::kernels::convertAlignedCompressedTimeDerivatives( m_derivatives[ m_meshToLts[ (i_meshId)-1 ] ],
                                                             o_timeDerivativesCell );

  // get neighbors derivatives
  seissol::kernels::convertAlignedCompressedTimeDerivatives( m_faceNeighbors[ m_meshToCopyInterior[ (i_meshId)-1 ] ][ (i_localFaceId)-1 ],
                                                             o_timeDerivativesNeighbor );

  real l_timeIntegrated[NUMBER_OF_ALIGNED_DOFS] __attribute__((aligned(ALIGNMENT)));

  // compute time integrated DOFs of the cell
  m_timeKernel.computeIntegral( 0,
                                0,
                                i_timeStepWidth,
                                m_derivatives[ m_meshToLts[ (i_meshId)-1 ] ],
                                l_timeIntegrated );

  seissol::kernels::convertAlignedDofs( l_timeIntegrated, o_timeIntegratedCell );

  // compute time integrated dofs of the neighbor

  m_timeKernel.computeIntegral( 0,
                                0,
                                i_timeStepWidth,
                                m_faceNeighbors[ m_meshToCopyInterior[ (i_meshId)-1 ] ][ (i_localFaceId)-1 ],
                                l_timeIntegrated );

  seissol::kernels::convertAlignedDofs( l_timeIntegrated, o_timeIntegratedNeighbor );
}

void seissol::Interoperability::getDofs( int    i_meshId,
                                         double o_dofs[NUMBER_OF_DOFS] ) {
  unsigned int l_cellId = m_meshToCopyInterior[ (i_meshId)-1 ];

  seissol::kernels::convertAlignedDofs( m_dofs[l_cellId], o_dofs );
}

void seissol::Interoperability::getDofsFromDerivatives( int    i_meshId,
                                                        double o_dofs[NUMBER_OF_DOFS] ) {
  // assert that the cell provides derivatives
  assert( (m_cellInformation[ m_meshToLts[ (i_meshId)-1 ] ].ltsSetup >> 9)%2 == 1 );

  // get DOFs from 0th derivatives
  seissol::kernels::convertAlignedDofs( m_derivatives[ m_meshToLts[ (i_meshId)-1 ] ], o_dofs );
}

void seissol::Interoperability::getNeighborDofsFromDerivatives( int    i_meshId,
                                                                int    i_localFaceId,
                                                                double  o_dofs[NUMBER_OF_DOFS] ) {

  // get DOFs from 0th neighbors derivatives
  seissol::kernels::convertAlignedDofs(  m_faceNeighbors[ m_meshToCopyInterior[ (i_meshId)-1 ] ][ (i_localFaceId)-1 ],
                                         o_dofs );
}

void seissol::Interoperability::simulate( double i_finalTime ) {
  seissol::SeisSol::main.simulator().setFinalTime( i_finalTime );

 seissol::SeisSol::main.simulator().simulate();
}

void seissol::Interoperability::finalizeIO()
{
	seissol::SeisSol::main.waveFieldWriter().close();
	seissol::SeisSol::main.checkPointManager().close();
}

void seissol::Interoperability::writeReceivers( double i_fullUpdateTime,
                                                double i_timeStepWidth,
                                                double i_receiverTime,
                                                std::vector< int > &i_receiverIds ) {
  assert( i_receiverIds.size() > 0 );

  // get number of elements
  int l_numberOfReceivers = i_receiverIds.size();

  // get a pointer to the raw data
  int *l_receiverIds = &i_receiverIds.front();

  // call the fortran routine
  f_interoperability_writeReceivers( m_domain,
                                    &i_fullUpdateTime,
                                    &i_timeStepWidth,
                                    &i_receiverTime,
                                    &l_numberOfReceivers, 
                                     l_receiverIds );
}

void seissol::Interoperability::computeDynamicRupture( double i_fullUpdateTime,
                                                       double i_timeStepWidth ) {
  f_interoperability_computeDynamicRupture(  m_domain,
                                            &i_fullUpdateTime,
                                            &i_timeStepWidth );
}

#ifdef USE_PLASTICITY
void seissol::Interoperability::computePlasticity(  double i_timeStep,
                                                    double (*i_initialLoading)[NUMBER_OF_BASIS_FUNCTIONS],
                                                    double *io_dofs ) {
  // call fortran routine
  f_interoperability_computePlasticity(  m_domain,
                                        &i_timeStep,
										 NUMBER_OF_ALIGNED_BASIS_FUNCTIONS,
                                         i_initialLoading,
                                         io_dofs );
}
#endif

void seissol::Interoperability::computeMInvJInvPhisAtSources(double x, double y, double z, unsigned element, real mInvJInvPhisAtSources[NUMBER_OF_ALIGNED_BASIS_FUNCTIONS])
{
  double f_mInvJInvPhisAtSources[NUMBER_OF_BASIS_FUNCTIONS];
  
  int elem = static_cast<int>(element);
  f_interoperability_computeMInvJInvPhisAtSources(m_domain, x, y, z, elem, f_mInvJInvPhisAtSources);
  
  memset(mInvJInvPhisAtSources, 0, NUMBER_OF_ALIGNED_BASIS_FUNCTIONS * sizeof(real));
  for (unsigned bf = 0; bf < NUMBER_OF_BASIS_FUNCTIONS; ++bf) {
    mInvJInvPhisAtSources[bf] = f_mInvJInvPhisAtSources[bf];
  }
}
