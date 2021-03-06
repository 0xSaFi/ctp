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

#ifndef _VOTCA_CTP_QMANALYZE_H
#define _VOTCA_CTP_QMANALYZE_H

#include <stdio.h>

#include <votca/ctp/logger.h>
// #include <votca/ctp/mbgft.h>
// #include <votca/ctp/qmpackagefactory.h>

namespace votca { namespace ctp {
    using namespace std;
    
class QMAnalyze : public QMTool
{
public:

    QMAnalyze() { };
   ~QMAnalyze() { };

    string Identify() { return "qmanalyze"; }

    void   Initialize(Property *options);
    bool   Evaluate();


private:
    
    string      _orbfile;
    string      _output_file;
    
    Logger      _log;
    
    void CheckContent(  Orbitals& _orbitals );

};

void QMAnalyze::Initialize(Property* options) {

            // update options with the VOTCASHARE defaults   
    UpdateWithDefaults( options );
 

            string key = "options." + Identify();
            // _jobfile = options->get(key + ".file").as<string>();

            // key = "options." + Identify();
 
       
           // orbitals file or pure DFT output
           _orbfile      = options->get(key + ".input").as<string> ();
           _output_file  = options->get(key + ".output").as<string> ();

            
    
  
    // get the path to the shared folders with xml files
    char *votca_share = getenv("VOTCASHARE");    
    if(votca_share == NULL) throw std::runtime_error("VOTCASHARE not set, cannot open help files.");
    // string xmlFile = string(getenv("VOTCASHARE")) + string("/ctp/qmpackages/") + _package + string("_idft_pair.xml");
    // load_property_from_xml( _package_options, xmlFile );    

    // register all QM packages (Gaussian, TURBOMOLE, etc)
    // QMPackageFactory::RegisterAll();
    
    
    
    
    
}

bool QMAnalyze::Evaluate() {

    _log.setReportLevel( logDEBUG );
    _log.setMultithreading( true );
    
    _log.setPreface(logINFO,    "\n... ...");
    _log.setPreface(logERROR,   "\n... ...");
    _log.setPreface(logWARNING, "\n... ...");
    _log.setPreface(logDEBUG,   "\n... ..."); 

    CTP_LOG(logDEBUG, _log) << "Analyzing serialized QM data in " << _orbfile << flush;

    Orbitals _orbitals;
    // load the QM data from serialized orbitals object

    std::ifstream ifs( (_orbfile).c_str());
    CTP_LOG(logDEBUG, _log) << " Loading QM data from " << _orbfile << flush;
    boost::archive::binary_iarchive ia(ifs);
    ia >> _orbitals;
    ifs.close();
    
    // output info about contents of serialized data
    CheckContent( _orbitals );
    
    
    
    
    // CTP_LOG(logDEBUG, _log) << "Written text data to " << _output_file << flush;
    
    
    return true;
}




void QMAnalyze::CheckContent( Orbitals& _orbitals ){


   
    CTP_LOG(logDEBUG, _log) << "===== Summary of serialized content ===== " << flush;
    CTP_LOG(logDEBUG, _log) << "   Information about DFT:" << flush;
    
          
    // DFT atoms
    if ( _orbitals.hasQMAtoms() ) {
        CTP_LOG(logDEBUG, _log) << "      atoms:                  " << _orbitals.QMAtoms().size() << flush;
    } else {
        CTP_LOG(logDEBUG, _log) << "      atoms:                  not stored "<< flush;
    } 
    
    // QM package
    if ( _orbitals.hasQMpackage() ){
        CTP_LOG(logDEBUG, _log) << "      QM package:             " << _orbitals.getQMpackage() << flush;
    
    } else {
        CTP_LOG(logDEBUG, _log) << "      QM package:             not stored " << flush;
    }
    
    
    // DFT basis set
    if ( _orbitals.hasDFTbasis() ) {
        CTP_LOG(logDEBUG, _log) << "      basis set:              " << _orbitals.getDFTbasis() << flush;
    } else {
        CTP_LOG(logDEBUG, _log) << "      basis set:              not stored "<< flush;
    }

    // DFT basis set size
    if ( _orbitals.hasBasisSetSize() ) {
        CTP_LOG(logDEBUG, _log) << "      basis set size:         " << _orbitals.getBasisSetSize() << flush;
   
    } else {
        CTP_LOG(logDEBUG, _log) << "      basis set size:          not stored "<< flush;
    }

    // DFT number of electrons
    if ( _orbitals.hasNumberOfElectrons() ) {
        CTP_LOG(logDEBUG, _log) << "      number of electrons:    " << _orbitals.getNumberOfElectrons() << flush;
    } else {
         CTP_LOG(logDEBUG, _log) << "      number of electrons:    not stored "<< flush;
    }    
    
    // DFT number of levels
    if ( _orbitals.hasNumberOfLevels() ) {
        CTP_LOG(logDEBUG, _log) << "      number of levels:       " << _orbitals.getNumberOfLevels() << flush;
    } else {
         CTP_LOG(logDEBUG, _log) << "      number of levels:       not stored "<< flush;
    }    

    // DFT orbital energies
    if ( _orbitals.hasMOEnergies() ) {
        CTP_LOG(logDEBUG, _log) << "      MO energies:            " << _orbitals.getEnergies()->size() << flush;
    } else {
         CTP_LOG(logDEBUG, _log) << "      MO energies:            not stored "<< flush;
    }    

    // DFT orbital coefficients
    if ( _orbitals.hasMOCoefficients() ) {
        CTP_LOG(logDEBUG, _log) << "      MO coefficients:        " << _orbitals.MOCoefficients().size1() << " x " << _orbitals.MOCoefficients().size2() << flush;
    } else {
        CTP_LOG(logDEBUG, _log) << "      MO coefficients:        not stored "<< flush;
    }  
    
    // DFT AO overlap matrix
    if ( _orbitals.hasAOOverlap() ) {
        CTP_LOG(logDEBUG, _log) << "      AO overlap matrix:      " << _orbitals.getOverlap()->size1()  << " x " << _orbitals.getOverlap()->size2() << flush;
    } else {
        CTP_LOG(logDEBUG, _log) << "      AO overlap matrix:      not stored "<< flush;
    }    
    
    // DFT AO XC matrix
    if ( _orbitals.hasAOVxc() ) {
        CTP_LOG(logDEBUG, _log) << "      AO XC matrix:           " << _orbitals.AOVxc().size1()  << " x " << _orbitals.AOVxc().size2() << flush;
    } else {
        CTP_LOG(logDEBUG, _log) << "      AO XC matrix:           not stored "<< flush;
    }    

    // QM total energy
    if ( _orbitals.hasQMEnergy() ){
        CTP_LOG(logDEBUG, _log) << "      QM energy:              " << _orbitals.getQMEnergy() << flush;
    } else{
        CTP_LOG(logDEBUG, _log) << "      QM energy:              not stored " << flush;
    }
    
    // MM self-energy 
    if ( _orbitals.hasSelfEnergy() ){
        CTP_LOG(logDEBUG, _log) << "      MM self energy:         " << _orbitals.getSelfEnergy() << flush;
    } else{
        CTP_LOG(logDEBUG, _log) << "      MM self energy:         not stored " << flush;
    }    
    
    // DFT transfer integrals
    if ( _orbitals.hasMOCouplings() ) {
        CTP_LOG(logDEBUG, _log) << "      DFT transfer integrals: " << _orbitals.MOCouplings().size1() << " x " << _orbitals.MOCouplings().size2() << flush;

    } else {
         CTP_LOG(logDEBUG, _log) << "      DFT transfer integrals: not stored "<< flush;
    }    
    
    
    

   
    
    
    
}

}}


#endif