/* 
 *            Copyright 2009-2012 The VOTCA Development Team
 *                       (http://www.votca.org)
 *
 *      Licensed under the Apache License, Version 2.0 (the "License")
 *
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __VOTCA_CTP_ORBITALS_H
#define	__VOTCA_CTP_ORBITALS_H

#include <votca/ctp/basisset.h>
#include <votca/ctp/qmatom.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <votca/tools/globals.h>
#include <votca/tools/property.h>
#include <votca/tools/vec.h>

// Text archive that defines boost::archive::text_oarchive
// and boost::archive::text_iarchive
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

// XML archive that defines boost::archive::xml_oarchive
// and boost::archive::xml_iarchive
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

// XML archive which uses wide characters (use for UTF-8 output ),
// defines boost::archive::xml_woarchive
// and boost::archive::xml_wiarchive
#include <boost/archive/xml_woarchive.hpp>
#include <boost/archive/xml_wiarchive.hpp>

// Binary archive that defines boost::archive::binary_oarchive
// and boost::archive::binary_iarchive
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/version.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

static const double                 _conv_Hrt_eV = 27.21138386;

namespace ub = boost::numeric::ublas;
    
namespace votca { namespace ctp {
    
 
/**
 * \brief container for molecular orbitals
 * 
<<<<<<< local
 * The Orbitals class stores orbital id, energy, MO coefficients, basis set
 *     
=======
 * The Orbitals class stores orbital id, energy, MO coefficients
>>>>>>> other
 */
class Orbitals 
{
public:   

    Orbitals();
   ~Orbitals();

    bool           hasBasisSetSize() { return _has_basis_set_size; }
    int            getBasisSetSize() { return (_has_basis_set_size) ? _basis_set_size : 0; }
    void           setBasisSetSize( const int &basis_set_size );
    
    int            getNumberOfLevels() { return (_has_occupied_levels && _has_unoccupied_levels) ? ( _occupied_levels + _unoccupied_levels ) : 0; }
    void           setNumberOfLevels( const int &occupied_levels, const int &unoccupied_levels );
    
    bool           hasNumberOfElectrons() { return _has_number_of_electrons; }
    int            getNumberOfElectrons() { return (_has_number_of_electrons) ? _number_of_electrons : 0; } ;
    void           setNumberOfElectrons( const int &electrons );
    

    ub::symmetric_matrix<double>* getOverlap() { return &_overlap; }
    ub::matrix<double>* getOrbitals() { return &_mo_coefficients; }
    ub::vector<double>* getEnergies() { return &_mo_energies; }
    ub::matrix<double>* getIntegrals() { return _integrals; }
    void setIntegrals( ub::matrix<double>* integrals ) { _has_integrals = true;  _integrals = integrals; }

    /// Atomic orbitals
    const ub::symmetric_matrix<double> &AOOverlap() const { return _overlap; }
    // Allows to change the matrix: useful to fill in the matrix
    ub::symmetric_matrix<double> &AOOverlap() { return _overlap; }

    /// Molecular Orbital coefficients
    const ub::matrix<double> &MOCoefficients() const { return _mo_coefficients; }
    /// Allows to change the matrix: useful to fill in the matrix
    ub::matrix<double> &MOCoefficients() { return _mo_coefficients; }
    
    /// Molecular Orbital energies
    const ub::vector<double> &MOEnergies() const { return _mo_energies; }
    /// Allows to change the matrix: useful to fill in the matrix
    ub::vector<double> &MOEnergies() { return _mo_energies; }

    /// Does not allow to change the matrix: useful for fast access    
    const ub::matrix<double> &MOCouplings() const { return _mo_couplings; }
    // Allows to change the matrix: useful to fill in the values
    ub::matrix<double> &MOCouplings() { return _mo_couplings; }

 
    double getEnergy( int level) { return (_has_mo_energies) ? _conv_Hrt_eV*_mo_energies[level-1] : 0; }
    
    std::vector<int>* getDegeneracy( int level, double _energy_difference );
    std::vector< QMAtom* >* getAtoms() { return &_atoms; }
    
    bool hasSelfEnergy() { return _has_self_energy; }
    double getSelfEnergy() { return (_has_self_energy) ? _self_energy : 0; }

    bool hasQMEnergy() { return _has_qm_energy; }
    double getQMEnergy() { return (_has_qm_energy) ? _qm_energy : 0; }
    
    // for GW-BSE
    ub::symmetric_matrix<double>* getVxc() { return &_vxc; }
    bool hasQMpackage() { return _has_qm_package; }
    string getQMpackage() { return _qm_package; }
    bool hasQPpert() { return _has_QPpert; }
    ub::matrix<double>* getQPpertEnergies() {return  &_QPpert_energies ;}
    bool hasQPdiag() { return _has_QPdiag; }
    std::vector<double>* getQPdiagEnergies() {return  &_QPdiag_energies ;} 
    ub::matrix<double>* getQPdiagCoefficients() {return  &_QPdiag_coefficients ;}
    std::vector<int>* getQPLevelsIndexList() {return &_QP_levels_index;}
    ub::matrix<int>* getBSELevelsIndexList() { return &_BSE_levels_indices; }
    bool hasBSESinglets() {return _has_BSE_singlets;}
    std::vector<double>* getBSESingletEnergies() {return &_BSE_singlet_energies;}
    ub::matrix<double>* getBSESingletCoefficients() {return &_BSE_singlet_coefficients;}
    bool hasBSETriplets() {return _has_BSE_triplets;}
    std::vector<double>* getBSETripletEnergies() {return &_BSE_triplet_energies;}
    ub::matrix<double>* getBSETripletCoefficients() {return &_BSE_triplet_coefficients; }   
    

    
    // returns indeces of a re-sorted in a descending order vector of energies
    void SortEnergies( std::vector<int>* index );
    
    /** Adds a QM atom to the atom list */
    QMAtom* AddAtom (std::string _type, 
                     double _x, double _y, double _z, 
                     double _charge = 0, bool _from_environment = false)
    {
        QMAtom* pAtom = new QMAtom(_type, _x, _y, _z, _charge, _from_environment);
        _atoms.push_back( pAtom );
        return pAtom;
    }
    
    void setStorage( bool _store_orbitals, bool _store_overlap,  bool _store_integrals ) {
        _has_mo_coefficients = _store_orbitals;
        _has_overlap = _store_overlap;
        _has_integrals = _store_integrals;
    } 
        
    void WritePDB( FILE *out );
    
    // reduces number of virtual orbitals to factor*number_of_occupied_orbitals
    void Trim( int factor );

    // reduces number of virtual orbitals to [HOMO-degG:LUMO+degL]
    void Trim( int degH, int degL );

    /** Loads orbitals from a file
     * Returns true if successful and does not throw an exception.
     * If exception is required, please use the << overload.
     */
    bool Load(string file_name);    
    
private:
    

    bool                                _has_basis_set_size;
    int                                     _basis_set_size;   
    
    bool                                _has_occupied_levels;
    int                                     _occupied_levels;
    
    bool                                _has_unoccupied_levels;
    int                                     _unoccupied_levels;
    
    bool                                _has_number_of_electrons;
    int                                     _number_of_electrons;
    
    bool                                _has_level_degeneracy;
    std::map<int, std::vector<int> >        _level_degeneracy;
    
    bool                                _has_mo_energies;
    ub::vector<double>                      _mo_energies; 
    
    bool                                _has_mo_coefficients;
    ub::matrix<double>                      _mo_coefficients;
    
    bool                                _has_overlap;
    ub::symmetric_matrix<double>            _overlap;
    
    bool                                _has_vxc;
    ub::symmetric_matrix<double>            _vxc;
    
    bool                                _has_charges;
    bool                                _has_atoms;
    std::vector< QMAtom* >                  _atoms;   

    bool                                _has_qm_energy;
    double                                  _qm_energy;
    
    bool                                _has_self_energy;
    double                                  _self_energy;
    
    bool                                _has_integrals;
    ub::matrix<double>*                 _integrals;
    ub::matrix<double>                  _mo_couplings;
   
    bool                                _has_basis_set;
    BasisSet                            _basis_set;
    
    // new variables for GW-BSE storage
    bool                                _has_qm_package;
    string                              _qm_package;
    // perturbative quasiparticle energies
    bool                                _has_QPpert;
    std::vector<int>                    _QP_levels_index;
    ub::matrix<double>                  _QPpert_energies;
    // quasiparticle energies and coefficients after diagonalization
    bool                                _has_QPdiag;
    std::vector<double>                 _QPdiag_energies;
    ub::matrix<double>                  _QPdiag_coefficients;
    // excitons
    ub::matrix<int>                     _BSE_levels_indices;
    bool                                _has_BSE_singlets;
    std::vector<double>                 _BSE_singlet_energies;
    ub::matrix<double>                  _BSE_singlet_coefficients;
    bool                                _has_BSE_triplets;
    std::vector<double>                 _BSE_triplet_energies;
    ub::matrix<double>                  _BSE_triplet_coefficients;    
    

private:

    /**
    * @param _energy_difference [ev] Two levels are degenerate if their energy is smaller than this value
    * @return A map with key as a level and a vector which is a list of close lying orbitals
    */    
    bool CheckDegeneracy( double _energy_difference );
    
    // Allow serialization to access non-public data members
    friend class boost::serialization::access;
    
    //Allow Gaussian object to access non-public data members
    friend class Gaussian;
    friend class Turbomole;
    friend class NWChem;
    friend class GW;
    
    // serialization itself (template implementation stays in the header)
    template<typename Archive> 
    void serialize(Archive& ar, const unsigned int version) {

       ar & _has_basis_set_size;
       ar & _has_occupied_levels;
       ar & _has_unoccupied_levels;
       ar & _has_number_of_electrons;
       ar & _has_level_degeneracy;
       ar & _has_mo_energies;
       ar & _has_mo_coefficients;
       ar & _has_overlap;
       ar & _has_atoms;
       ar & _has_qm_energy;
       ar & _has_self_energy;
       ar & _has_integrals;
       ar & _has_qm_package;
       
       //cout << "\nLoaded 1\n" << flush;
       if ( _has_basis_set_size ) { ar & _basis_set_size; }
       if ( _has_occupied_levels ) { ar & _occupied_levels; }
       if ( _has_unoccupied_levels ) { ar & _unoccupied_levels; }
       if ( _has_number_of_electrons ) { ar & _number_of_electrons; }
       if ( _has_level_degeneracy ) { ar & _level_degeneracy; }
       if ( _has_mo_energies ) { ar & _mo_energies; }
       if ( _has_mo_coefficients ) { ar & _mo_coefficients; }
       if ( _has_overlap ) { 
           // symmetric matrix does not serialize by default
            if (Archive::is_saving::value) {
                unsigned int size = _overlap.size1();
                ar & size;
             }

            // copy the values back if loading
            if (Archive::is_loading::value) {
                unsigned int size;
                ar & size;
                _overlap.resize(size);
             }
            
           for (unsigned int i = 0; i < _overlap.size1(); ++i)
                for (unsigned int j = 0; j <= i; ++j)
                    ar & _overlap(i, j); 
       }

       if ( _has_atoms ) { ar & _atoms; }
       if ( _has_qm_energy ) { ar & _qm_energy; }
       if ( _has_qm_package ) { ar & _qm_package; }
       if ( _has_self_energy ) { ar & _self_energy; }     
       if ( _has_integrals ) { ar & _integrals; } 

       //cout << "\nLoaded 2\n" << flush;
       // GW-BSE storage
       if(version > 0)  {
            ar & _has_vxc;               
            ar & _has_QPpert;            
            ar & _has_QPdiag;            
            ar & _has_BSE_singlets;      
            ar & _has_BSE_triplets;      
            if ( _has_QPpert ) { ar & _QP_levels_index; ar & _QPpert_energies; }
            if ( _has_QPdiag ) { ar & _QPdiag_energies; ar & _QPdiag_coefficients; }
            if ( _has_BSE_singlets || _has_BSE_triplets ) { ar & _BSE_levels_indices; }
            if ( _has_BSE_singlets ) { ar & _BSE_singlet_energies; ar & _BSE_singlet_coefficients; }
            if ( _has_BSE_triplets ) { ar & _BSE_triplet_energies; ar & _BSE_triplet_coefficients; }
            
            //cout << "\nLoaded 3\n" << flush;
            if ( _has_vxc ) { 
               // symmetric matrix does not serialize by default
                if (Archive::is_saving::value) {
                    unsigned int size = _vxc.size1();
                    ar & size;
                 }

                // copy the values back if loading
                if (Archive::is_loading::value) {
                    unsigned int size;
                    ar & size;
                    _vxc.resize(size);
                 }

               for (unsigned int i = 0; i < _vxc.size1(); ++i)
                    for (unsigned int j = 0; j <= i; ++j)
                        ar & _vxc(i, j); 
            }
       //cout << "\nLoaded 4\n" << flush;
            
       } // end version 1: GW-BSE storage
    }// end of serialization
};

}}

BOOST_CLASS_VERSION(votca::ctp::Orbitals, 1)
        
#endif	/* __VOTCA_CTP_ORBITALS_H */

