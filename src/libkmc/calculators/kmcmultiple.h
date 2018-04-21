/*
 * Copyright 2009-2013 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *vector<Node*> 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * author: Kordt
 */

#ifndef __VOTCA_KMC_MULTIPLE_H
#define	__VOTCA_KMC_MULTIPLE_H

// #include <votca/kmc/vssmgroup.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
//#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <votca/tools/vec.h>
#include <votca/tools/statement.h>
#include <votca/tools/database.h>
#include <votca/tools/tokenizer.h>
#include <votca/tools/globals.h>
#include <votca/tools/random2.h>
#include <unordered_map>
#include <cmath> // needed for abs(double)
#include "node.h"

using namespace std;

namespace votca { namespace kmc {

static double kB   = 8.617332478E-5; // eV/K
static double hbar = 6.5821192815E-16; // eV*s
//static double eps0 = 8.85418781762E-12/1.602176565E-19; // e**2/eV/m = 8.85418781762E-12 As/Vm
//static double epsr = 3.0; // relative material permittivity
static double Pi   = 3.14159265358979323846;

typedef unordered_map<unsigned long, double> CoulombMap;
typedef CoulombMap::const_iterator CoulombIt;

class Chargecarrier
{
    public:
        int position;
        int id;
        Node *node;
        myvec dr_travelled;
};


void progressbar(double fraction)
{
    int totalbars = 50;
    std::cout << "\r";
    for(double bars=0; bars<double(totalbars); bars++)
    {
        if(bars<=fraction*double(totalbars))
        {
            std::cout << "|";
        }
        else
        {
            std::cout << "-";
        }
    }
    std::cout << "  " << int(fraction*1000)/10. <<" %   ";
    std::cout << std::flush;
    if(fraction*100 == 100)
    {
        std::cout << std::endl;
    }
}


class KMCMultiple : public KMCCalculator 
{
public:
    KMCMultiple() {};
   ~KMCMultiple() {};

    void Initialize(const char *filename, Property *options, const char *outputfile );
    bool EvaluateFrame();

protected:
	    vector<Node*>  LoadGraph();
            vector<double> RunVSSM(vector<Node*> node, double runtime, unsigned int numberofcharges, votca::tools::Random2 *RandomVariable);
            void WriteOcc(vector<double> occP, vector<Node*> node);
            void InitialRates(vector<Node*> node);
         
            void InitBoxSize(vector<Node*> node);


            string _injection_name;
            string _injectionmethod;
            int _injectionfree;
            double _runtime;
            int _seed;
            int _numberofcharges;
            int _allowparallel;
            double _fieldX;
            double _fieldY;
            double _fieldZ;
            double _outputtime;
            string _trajectoryfile;
            string _carriertype;
            double _temperature;
            string _filename;
            string _outputfile;
            double _q;
            double _boxsizeX;
            double _boxsizeY;
            double _boxsizeZ;
            string _rates;
};


void KMCMultiple::Initialize(const char *filename, Property *options, const char *outputfile )
{
    	if (options->exists("options.kmcmultiple.runtime")) {
	    _runtime = options->get("options.kmcmultiple.runtime").as<double>();
	}
	else {
	    throw runtime_error("Error in kmcmultiple: total run time is not provided");
        }
    	if (options->exists("options.kmcmultiple.seed")) {
	    _seed = options->get("options.kmcmultiple.seed").as<int>();
	}
	else {
	    throw runtime_error("Error in kmcmultiple: seed is not provided");
        }
        
    	if (options->exists("options.kmcmultiple.numberofcharges")) {
	    _numberofcharges = options->get("options.kmcmultiple.numberofcharges").as<int>();
	}
	else {
	    throw runtime_error("Error in kmcmultiple: number of charges is not provided");
        }

        if (options->exists("options.kmcmultiple.injection")) {
	    _injection_name = options->get("options.kmcmultiple.injection").as<string>();
	}
        else {
	    throw runtime_error("Error in kmcmultiple: injection pattern is not provided");
        }
        
        if (options->exists("options.kmcmultiple.injectionmethod")) {
	    _injectionmethod = options->get("options.kmcmultiple.injectionmethod").as<string>();
	}
        else {
	    cout << "WARNING in kmcmultiple: You did not specify an injection method. It will be set to random injection." << endl;
            _injectionmethod = "random";
        }
        if (_injectionmethod != "random" && _injectionmethod != "equilibrated")
        {
	    cout << "WARNING in kmcmultiple: Unknown injection method. It will be set to random injection." << endl;
            _injectionmethod = "random";
            
        }

        if (options->exists("options.kmcmultiple.fieldX")) {
	    _fieldX = options->get("options.kmcmultiple.fieldX").as<double>();
	}
        else {
            _fieldX = 0;
        }
        if (options->exists("options.kmcmultiple.fieldY")) {
	    _fieldY = options->get("options.kmcmultiple.fieldY").as<double>();
	}
        else {
            _fieldY = 0;
        }
        if (options->exists("options.kmcmultiple.fieldZ")) {
	    _fieldZ = options->get("options.kmcmultiple.fieldZ").as<double>();
	}
        else {
            _fieldZ = 0;
        }
        if (options->exists("options.kmcmultiple.outputtime")) {
	    _outputtime = options->get("options.kmcmultiple.outputtime").as<double>();
	}
        else {
            _outputtime = 0;
        }
        if (options->exists("options.kmcmultiple.trajectoryfile")) {
	    _trajectoryfile = options->get("options.kmcmultiple.trajectoryfile").as<string>();
	}
        else {
            _trajectoryfile = "trajectory.csv";
        }
        if (_trajectoryfile == "") {
            _trajectoryfile = "trajectory.csv";
        }
        if (options->exists("options.kmcmultiple.carriertype")) {
	    _carriertype = options->get("options.kmcmultiple.carriertype").as<string>();
	}
        else {
	    cout << "WARNING in kmcmultiple: You did not specify a charge carrier type. It will be set to electrons." << endl;
            _carriertype = "e";
        }
        if(_carriertype == "electron" || _carriertype == "electrons" || _carriertype == "e"){
            _carriertype = "e";
            cout << "carrier type: electrons" << endl;
            
        }
        else if(_carriertype == "hole" || _carriertype == "holes" || _carriertype == "h"){
            _carriertype = "h";
            cout << "carrier type: holes" << endl;
            
        }
        else {
            _carriertype = "e";
            cout << "Carrier type specification invalid. Setting type to electrons." << endl;
        }


        if (options->exists("options.kmcmultiple.temperature")) {
	    _temperature = options->get("options.kmcmultiple.temperature").as<double>();
	}
        else {
	    cout << "WARNING in kmcmultiple: You did not specify a temperature. If no explicit Coulomb interaction is used, this is not a problem, as the rates are read from the state file and temperature is not needed explicitly in the KMC simulation. Otherwise a default value of 300 K is used." << endl;
            _temperature = 0;
        }
        if (options->exists("options.kmcmultiple.rates")) {
	    _rates = options->get("options.kmcmultiple.rates").as<string>();
	}
        else {
	    cout << "Using rates from statefile." << endl;
            _rates = "statefile";
        }
        if(_rates != "statefile" && _rates != "calculate"){
	    cout << "WARNING in kmcmultiple: Invalid option rates. Valid options are 'statefile' or 'calculate'. Setting it to 'statefile'." << endl;
            _rates = "statefile";
        }
        

        _filename = filename;
        _outputfile = outputfile;

}

vector<Node*> KMCMultiple::LoadGraph()
{
    vector<Node*> node;
    
    // Load nodes
    votca::tools::Database db;
    db.Open( _filename );
    if(votca::tools::globals::verbose) {cout << "LOADING GRAPH" << endl << "database file: " << _filename << endl; }
    votca::tools::Statement *stmt = db.Prepare("SELECT _id-1, name, posX, posY, posZ, UnCnN"+_carriertype+", UcNcC"+_carriertype+",eAnion,eNeutral,eCation,ucCnN"+_carriertype+" FROM segments;");

    int i=0;
    while (stmt->Step() != SQLITE_DONE)
    {
        Node *newNode = new Node();
        node.push_back(newNode);

        int newid = stmt->Column<int>(0);
        string name = stmt->Column<string>(1);
        node[i]->id = newid;
        myvec nodeposition = myvec(stmt->Column<double>(2)*1E-9, stmt->Column<double>(3)*1E-9, stmt->Column<double>(4)*1E-9); // converted from nm to m
        node[i]->position = nodeposition;
        node[i]->reorg_intorig = stmt->Column<double>(5); // UnCnN
        node[i]->reorg_intdest = stmt->Column<double>(6); // UcNcC
        double eAnion = stmt->Column<double>(7);
        //double eNeutral = stmt->Column<double>(8);
        double eCation = stmt->Column<double>(9);
        double internalenergy = stmt->Column<double>(10); // UcCnN
        double siteenergy = 0;
        if(_carriertype == "e")
        {
            siteenergy = eAnion + internalenergy;
        }
        else if(_carriertype == "h")
        {
            siteenergy = eCation + internalenergy;
        }
        node[i]->siteenergy = siteenergy;
        if (votca::tools::wildcmp(_injection_name.c_str(), name.c_str()))
        {
            node[i]->injectable = 1;
        }
        else
        {
            node[i]->injectable = 0;
        }
        i++;
    }
    delete stmt;
    if(votca::tools::globals::verbose) { cout << "segments: " << node.size() << endl; }
    
    // Load pairs and rates
    int numberofpairs = 0;
    stmt = db.Prepare("SELECT seg1-1 AS 'segment1', seg2-1 AS 'segment2', rate12"+_carriertype+" AS 'rate', drX, drY, drZ, Jeff2"+_carriertype+", lO"+_carriertype+" FROM pairs UNION SELECT seg2-1 AS 'segment1', seg1-1 AS 'segment2', rate21"+_carriertype+" AS 'rate', -drX AS 'drX', -drY AS 'drY', -drZ AS 'drZ', Jeff2"+_carriertype+", lO"+_carriertype+" FROM pairs ORDER BY segment1;");
    while (stmt->Step() != SQLITE_DONE)
    {
        int seg1 = stmt->Column<int>(0);
        int seg2 = stmt->Column<int>(1);

        double rate12 = stmt->Column<double>(2);

        myvec dr = myvec(stmt->Column<double>(3)*1E-9, stmt->Column<double>(4)*1E-9, stmt->Column<double>(5)*1E-9); // converted from nm to m
        double Jeff2 = stmt->Column<double>(6);
        double reorg_out = stmt->Column<double>(7); 
        node[seg1]->AddEvent(seg2,rate12,dr,Jeff2,reorg_out);
        numberofpairs ++;
    }    
    delete stmt;

    if(votca::tools::globals::verbose) { cout << "pairs: " << numberofpairs/2 << endl; }
    
    // Calculate initial escape rates !!!THIS SHOULD BE MOVED SO THAT IT'S NOT DONE TWICE IN CASE OF COULOMB INTERACTION!!!
    for(unsigned int i=0; i<node.size(); i++)
    {
        node[i]->InitEscapeRate();
    }
    return node;
}



void ResetForbidden(vector<int> &forbiddenid)
{
    forbiddenid.clear();
}

void AddForbidden(int id, vector<int> &forbiddenid)
{
    forbiddenid.push_back(id);
}

int Forbidden(int id, vector<int> forbiddenlist)
{
    // cout << "forbidden list has " << forbiddenlist.size() << " entries" << endl;
    int forbidden = 0;
    for (unsigned int i=0; i< forbiddenlist.size(); i++)
    {
        if(id == forbiddenlist[i])
        {
            forbidden = 1;
            //cout << "ID " << id << " has been found as element " << i << " (" << forbiddenlist[i]<< ") in the forbidden list." << endl;
            break;
        }
    }
    return forbidden;
}

int ForbiddenEvents(int id, vector<Event> forbiddenevent)
{
    // cout << "forbidden list has " << forbiddenlist.size() << " entries" << endl;
    int forbidden = 0;
    for (unsigned int i=0; i< forbiddenevent.size(); i++)
    {
        if(id == forbiddenevent[i].destination)
        {
            forbidden = 1;
            //cout << "ID " << id << " has been found as element " << i << " (" << forbiddenlist[i]<< ") in the forbidden list." << endl;
            break;
        }
    }
    return forbidden;
}

/*int Surrounded(Node* node, vector<int> forbiddendests)
{
    int surrounded = 1;
    for(unsigned int i=0; i<node->event.size(); i++)
    {
        int thisevent_possible = 1;
        for(unsigned int j=0; j<forbiddendests.size(); j++)
        {
            if(node->event[i].destination == forbiddendests[j])
            {
                thisevent_possible = 0;
                break;
            }
        }
        if(thisevent_possible == 1)
        {
            surrounded = 0;
            break;
        }
    }
    return surrounded;
}*/

void printtime(int seconds_t)
{
    int seconds = seconds_t;
    int minutes = 0;
    int hours = 0;
    while(seconds / 60 >= 1)
    {
        seconds -= 60;
        minutes +=  1;
    }
    while(minutes / 60 >= 1)
    {
        minutes -= 60;
        hours +=  1;
    }
    char buffer [50];
    int n = sprintf(buffer, "%d:%02d:%02d",hours,minutes,seconds);
    printf("%s %i",buffer,n);
}


void KMCMultiple::InitBoxSize(vector<Node*> node)
{
    cout << endl << "Analysing size of the simulation cell." << endl;
    double minX = 0;
    double minY = 0;
    double minZ = 0;
    double maxX = 0;
    double maxY = 0;
    double maxZ = 0;
    double mindX = 777777777777;
    double mindY = 777777777777;
    double mindZ = 777777777777;
    for(unsigned int i = 0; i<node.size(); i++)
    {
        double posX = node[i]->position.x();
        double posY = node[i]->position.y();
        double posZ = node[i]->position.z();
        if(posX < minX) minX = posX;
        if(posY < minY) minY = posY;
        if(posZ < minZ) minZ = posZ;
        if(posX > maxX) maxX = posX;
        if(posY > maxY) maxY = posY;
        if(posZ > maxZ) maxZ = posZ;
        for(unsigned int j = 0; j<node[i]->event.size(); j++)
        {
            if(node[i]->event[j].dr.x() < mindX && node[i]->event[j].dr.x() > 0) mindX = node[i]->event[j].dr.x();
            if(node[i]->event[j].dr.y() < mindY && node[i]->event[j].dr.y() > 0) mindY = node[i]->event[j].dr.y();
            if(node[i]->event[j].dr.z() < mindZ && node[i]->event[j].dr.z() > 0) mindZ = node[i]->event[j].dr.z();
        }
    }
    _boxsizeX = maxX-minX + mindX;
    _boxsizeY = maxY-minY + mindY;
    _boxsizeZ = maxZ-minZ + mindZ;
    cout << "lattice constants (X,Y,Z): " << mindX << ", " << mindY << ", " << mindZ << endl;
    cout << "cell dimensions: " << _boxsizeX << " x " << _boxsizeY << " x " << _boxsizeZ << endl;
    cout << "spatial electron density: " << _numberofcharges/_boxsizeX/_boxsizeY/_boxsizeZ << " m^-3" << endl;
}

void KMCMultiple::InitialRates(vector<Node*> node)
{
    cout << endl <<"Calculating initial Marcus rates." << endl;
    cout << "    Temperature T = " << _temperature << " K." << endl;
    cout << "    Field: (" << _fieldX << ", " << _fieldY << ", " << _fieldZ << ") V/m" << endl;
    if (_carriertype == "e")
    {
        _q = -1.0;
    }
    else if (_carriertype == "h")
    {
        _q = 1.0;
    }
    cout << "    carriertype: " << _carriertype << " (charge: " << _q << " eV)" << endl;
    unsigned int numberofsites = node.size();
    cout << "    Rates for "<< numberofsites << " sites are computed." << endl;
    double maxreldiff = 0;
    int totalnumberofrates = 0;
    for(unsigned int i=0; i<numberofsites; i++)
    {
        unsigned int numberofneighbours = node[i]->event.size();
        for(unsigned int j=0; j<numberofneighbours; j++)
        {
            double dX =  node[i]->event[j].dr.x();
            double dY =  node[i]->event[j].dr.y();
            double dZ =  node[i]->event[j].dr.z();
            double dG_Field = _q * (dX*_fieldX +  dY*_fieldY + dZ*_fieldZ);
            
            double destindex =  node[i]->event[j].destination;
            double reorg = node[i]->reorg_intorig + node[destindex]->reorg_intdest + node[i]->event[j].reorg_out;
            
            double dG_Site = node[destindex]->siteenergy - node[i]->siteenergy;

            double J2 = node[i]->event[j].Jeff2;

            double dG = dG_Site - dG_Field;
            
            double rate = 2*Pi/hbar * J2/sqrt(4*Pi*reorg*kB*_temperature) * exp(-(dG+reorg)*(dG+reorg) / (4*reorg*kB*_temperature));
            
            // calculate relative difference compared to values in the table
            double reldiff = (node[i]->event[j].rate - rate) / node[i]->event[j].rate;
            if (reldiff > maxreldiff) {maxreldiff = reldiff;}
            reldiff = (node[i]->event[j].rate - rate) / rate;
            if (reldiff > maxreldiff) {maxreldiff = reldiff;}
            
            // set rates to calculated values
            node[i]->event[j].rate = rate;
            node[i]->event[j].initialrate = rate;
            
            totalnumberofrates ++;

        }
        
        // Initialise escape rates
        for(unsigned int i=0; i<node.size(); i++)
        {
            node[i]->InitEscapeRate();
        }

    }
    cout << "    " << totalnumberofrates << " rates have been calculated." << endl;
    if(maxreldiff < 0.01)
    {
        cout << "    Good agreement with rates in the state file. Maximal relative difference: " << maxreldiff*100 << " %"<< endl;
    }
    else
    {
        cout << "    WARNING: Rates differ from those in the state file up to " << maxreldiff*100 << " %." << " If the rates in the state file are calculated for a different temperature/field or if they are not Marcus rates, this is fine. Otherwise something might be wrong here." << endl;
    }
}




vector<double> KMCMultiple::RunVSSM(vector<Node*> node, double runtime, unsigned int numberofcharges, votca::tools::Random2 *RandomVariable)
{

    int realtime_start = time(NULL);
    cout << endl << "Algorithm: VSSM for Multiple Charges" << endl;
    cout << "number of charges: " << numberofcharges << endl;
    cout << "number of nodes: " << node.size() << endl;
    string stopcondition;
    unsigned long maxsteps = 0;
    if(runtime > 100)
    {
        stopcondition = "steps";
        maxsteps = runtime;
        cout << "stop condition: " << maxsteps << " steps." << endl;
    }
    else
    {
        stopcondition = "runtime";
        cout << "stop condition: " << runtime << " seconds runtime." << endl;
        cout << "(If you specify runtimes larger than 100 kmcmultiple assumes that you are specifying the number of steps.)" << endl;
    }
    
 
    
    if(numberofcharges > node.size())
    {
        throw runtime_error("ERROR in kmcmultiple: specified number of charges is greater than the number of nodes. This conflicts with single occupation.");
    }
    
    fstream traj;
    char trajfile[100];
    strcpy(trajfile, _trajectoryfile.c_str());
    cout << "Writing trajectory to " << trajfile << "." << endl; 
    traj.open (trajfile, fstream::out);
    if(_outputtime != 0)
    {   
        traj << "'time[s]'\t";
        for(unsigned int i=0; i<numberofcharges; i++)
        {
            traj << "'carrier" << i+1 << "_x'\t";    
            traj << "'carrier" << i+1 << "_y'\t";    
            traj << "'carrier" << i+1 << "_z";    
            if(i<numberofcharges-1)
            {
                traj << "'\t";
            }
        }
        traj << endl;
        
    }
    double outputfrequency = runtime/100;
    double outputstepfrequency = maxsteps/100;
    vector<double> occP(node.size(),0.);

    // Injection
    cout << endl << "injection method: " << _injectionmethod << endl;
    double deltaE = 0;
    double energypercarrier;
    double totalenergy = 0;
    if(_injectionmethod == "equilibrated")
    {
        vector< double > energy;
        for(unsigned int i=0; i<node.size(); i++)
        {
            energy.push_back(node[i]->siteenergy);
        }
        std::sort (energy.begin(), energy.end());
        double fermienergy = 0.5*(energy[_numberofcharges-1]+energy[_numberofcharges]);
                cout << "Energy range in morphology data: [" << energy[0] << ", " << energy[energy.size()-1] << "] eV." << endl;
        cout << "first guess Fermi energy: " << fermienergy << " eV."<< endl;

        cout << "improving guess for fermi energy ";
        
        double probsum;
        double fstepwidth = std::abs(0.01 * fermienergy);
        probsum = 0;
        while(std::abs(probsum - numberofcharges) > 0.1)
        {
            cout << ".";
            totalenergy = 0;
            probsum = 0;
            for(unsigned int i=0; i<energy.size(); i++)
            {
                totalenergy += energy[i] * 1/(exp((energy[i]-fermienergy)/kB/_temperature)+1);
                probsum += 1/(exp((energy[i]-fermienergy)/kB/_temperature)+1);
                //cout << "energy " << energy[i] << " has probability " << 1/(exp((energy[i]-fermienergy)/kB/_temperature)+1) << endl;
            }
            if(std::abs(probsum) > numberofcharges)
            {   // Fermi energy estimate too high
                fermienergy -= fstepwidth;
            }
            else
            {   // Fermi energy estimate too low
                fermienergy += fstepwidth;
            }
            fstepwidth = 0.95 * fstepwidth; // increase precision
        }
        cout << endl << "probsum=" << probsum << "(should be equal to number of charges)." << endl;
        cout << "fermienergy=" << fermienergy << endl;
        cout << "totalenergy=" << totalenergy << endl;
        energypercarrier = totalenergy / probsum; // in theory: probsum=_numberofcharges, but in small systems it can differ significantly
        cout << "Average energy per charge carrier: " << energypercarrier << " eV."<< endl;
        
        double stepwidth = std::abs(fermienergy)/1000;
        int inside = 0;
        while(inside < _numberofcharges)
        {
            inside = 0;
            deltaE += stepwidth;
            for(unsigned int i=0; i<energy.size(); i++)
            {
                if(energy[i] >= energypercarrier-deltaE && energy[i] <= energypercarrier+deltaE)
                {
                    inside += 1;
                }
            }
        }
        cout << inside << " charges in acceptance interval " << energypercarrier << " +/- " << deltaE << "." << endl;
        
    }
    vector< Chargecarrier* > carrier;
    vector<myvec> startposition(numberofcharges,myvec(0,0,0));
    cout << "looking for injectable nodes..." << endl;
    for (unsigned int i = 0; i < numberofcharges; i++)
    {
        Chargecarrier *newCharge = new Chargecarrier;      
        newCharge->id = i;
        newCharge->node = node[RandomVariable->rand_uniform_int(node.size())];
        int ininterval = 1;
        if (_injectionmethod == "equilibrated") {ininterval = 0;}
        while(newCharge->node->occupied == 1 || newCharge->node->injectable != 1 || ininterval != 1)
        {   // maybe already occupied? or maybe not injectable?
            newCharge->node = node[RandomVariable->rand_uniform_int(node.size())];
            if(_injectionmethod == "equilibrated")
            { // check if charge is in the acceptance interval
                if(newCharge->node->siteenergy >= energypercarrier-0*deltaE && newCharge->node->siteenergy <= energypercarrier+2*deltaE && newCharge->node->occupied == 0 && newCharge->node->injectable == 1)
                {
                    ininterval = 1;
                }
                
            }
        }
        // cout << "selected segment " << newCharge->node->id+1 << " which has energy " << newCharge->node->siteenergy << " within the interval [" << energypercarrier-0*deltaE << ", " << energypercarrier+2*deltaE << "]" << endl;
        newCharge->node->occupied = 1;
        newCharge->dr_travelled = myvec(0,0,0);
        startposition[i] = newCharge->node->position;
        cout << "starting position for charge " << i+1 << ": segment " << newCharge->node->id+1 << endl;
        carrier.push_back(newCharge);
    }
    
    if(_injectionmethod == "equilibrated")
    {
        cout << "repositioning charges to obtain an equilibrium configuration..." << endl;
        double realisedenergy = 0;
        //while(std::abs(realisedenergy - totalenergy) > 0.01*(std::abs(realisedenergy)+std::abs(totalenergy)))
        //{
            realisedenergy = 0;
            for (unsigned int j = 0; j < numberofcharges; j++)
            {
                realisedenergy += carrier[j]->node->siteenergy;
            }

            for(unsigned int i=0; i<numberofcharges; i++)
            {
                for(unsigned int k=0; k<node.size(); k++)
                {
                    if(std::abs(realisedenergy-carrier[i]->node->siteenergy+node[k]->siteenergy - totalenergy) < std::abs(realisedenergy - totalenergy) && node[k]->occupied == 0)
                    {    //move charge
                         carrier[i]->node->occupied = 0;
                         realisedenergy = realisedenergy-carrier[i]->node->siteenergy+node[k]->siteenergy;
                         carrier[i]->node = node[k];
                         node[k]->occupied = 1;
                    }        
                }
                
            }
        //}    
        cout << "realised energy: " << realisedenergy/numberofcharges << " eV (aimed for " << energypercarrier << " eV)"<<  endl;
        for(unsigned int i=0; i<numberofcharges; i++)
        {
            startposition[i] = carrier[i]->node->position;
            cout << "starting position for charge " << i+1 << ": segment " << carrier[i]->node->id+1 << endl;
        }
    }
    

    double simtime = 0.;
    unsigned long step = 0;
    double nextoutput = outputfrequency;
    unsigned long nextstepoutput = outputstepfrequency;
    double nexttrajoutput = _outputtime;
    
    progressbar(0.);
    vector<int> forbiddennodes;
    vector<int> forbiddendests;
    while((stopcondition == "runtime" && simtime < runtime) || (stopcondition == "steps" && step < maxsteps))
    {
        double cumulated_rate = 0;

        for(unsigned int i=0; i<numberofcharges; i++)
        {
            cumulated_rate += carrier[i]->node->EscapeRate();
        }
        if(cumulated_rate == 0)
        {   // this should not happen: no possible jumps defined for a node
            throw runtime_error("ERROR in kmcmultiple: Incorrect rates in the database file. All the escape rates for the current setting are 0.");
        }
        // go forward in time
        double dt = 0;
        double rand_u = 1-RandomVariable->rand_uniform();
        while(rand_u == 0)
        {
            cout << "WARNING: encountered 0 as a random variable! New try." << endl;
            rand_u = 1-RandomVariable->rand_uniform();
        }
        dt = -1 / cumulated_rate * log(rand_u);
        simtime += dt;
        if(votca::tools::globals::verbose) {cout << "simtime += " << dt << endl << endl;}
        step += 1;
        
        for(unsigned int i=0; i<numberofcharges; i++)
        {
            carrier[i]->node->occupationtime += dt;
        }

        
        ResetForbidden(forbiddennodes);
        int level1step = 0;
        while(level1step == 0)
        // LEVEL 1
        {
            
            // determine which electron will escape
            Node* do_oldnode;
            Node* do_newnode;
            double escaperate = 0;
            Chargecarrier* do_affectedcarrier;
            
            double u = 1 - RandomVariable->rand_uniform();
            for(unsigned int i=0; i<numberofcharges; i++)
            {
                u -= carrier[i]->node->EscapeRate()/cumulated_rate;
                if(u <= 0)
                {
                    do_oldnode = carrier[i]->node;
                    do_affectedcarrier = carrier[i];
                    escaperate = carrier[i]->node->EscapeRate();
                    break;
                }
               do_oldnode = carrier[i]->node;
               do_affectedcarrier = carrier[i];
               escaperate = carrier[i]->node->EscapeRate();
            }
                
            //double maxprob = 0.;
            //double newprob = 0.;
            myvec dr;
            if(votca::tools::globals::verbose) {cout << "Charge number " << do_affectedcarrier->id+1 << " which is sitting on segment " << do_oldnode->id+1 << " will escape!" << endl ;}
            if(Forbidden(do_oldnode->id, forbiddennodes) == 1) {continue;}
            
            // determine where it will jump to
            ResetForbidden(forbiddendests);
            while(true)
            {
            // LEVEL 2
                if(votca::tools::globals::verbose) {cout << "There are " << do_oldnode->event.size() << " possible jumps for this charge:"; }
                int counter = 0;
                do_newnode = NULL;
                u = 1 - RandomVariable->rand_uniform();
                
                /*for(unsigned int j=0; j<do_oldnode->event.size(); j++)
                {
                    if(votca::tools::globals::verbose) { cout << " " << do_oldnode->event[j].destination+1 ; }
                    u -= do_oldnode->event[j].rate/do_oldnode->EscapeRate();
                    if(u <= 0)
                    {
                        do_newnode = node[do_oldnode->event[j].destination];
                        dr = do_oldnode->event[j].dr;
                        break;
                    }
                    do_newnode = node[do_oldnode->event[j].destination];
                    dr = do_oldnode->event[j].dr;
                }*/
                for(unsigned int j=0; j<do_oldnode->event.size(); j++)
                {
                    //cout << " EVENT: " << do_oldnode->event[j].destination << " IN CODE : " << do_oldnode->event[j].destination+1 <<  "  RATE: " << do_oldnode->event[j].rate+1<< endl;
                    //cout << "Monte Carlo number: " << u << endl;
                    if (ForbiddenEvents(do_oldnode->event[j].destination,do_oldnode->forbiddenevent)==1)
                    {
                        //u -= do_oldnode->event[j].rate/escaperate;
                        //cout << "FORBIDDEN destination: "<< do_oldnode->event[j].destination << endl;
                        //cout << "Escaperate : "<< escaperate << endl;
                    }
                    else
                    {
                        if(votca::tools::globals::verbose) { cout << " " << do_oldnode->event[j].destination+1 ; }
                        u -= do_oldnode->event[j].rate/escaperate;
                    }

                    //cout << "minus : " << do_oldnode->event[j].rate/do_oldnode->EscapeRate() <<" u : "<< u <<endl;
                    if ((u <= 0) )
                    {
                        do_newnode = node[do_oldnode->event[j].destination];
                        dr = do_oldnode->event[j].dr;
                        counter = j;
                        break;
                    }
                    do_newnode = node[do_oldnode->event[j].destination];
                    dr = do_oldnode->event[j].dr;
                    counter = j;
                    //cout << " new NODE: " << do_newnode << endl;
                }

                if(do_newnode == NULL)
                {
                    if(votca::tools::globals::verbose) {cout << endl << "Node " << do_oldnode->id+1  << " is SURROUNDED by forbidden destinations and zero rates. Adding it to the list of forbidden nodes. After that: selection of a new escape node." << endl; }
                    AddForbidden(do_oldnode->id, forbiddennodes);
                    
                    //int nothing=0;
                    break; // select new escape node (ends level 2 but without setting level1step to 1)
                }
                if(votca::tools::globals::verbose) {cout << endl << "Selected jump: " << do_newnode->id+1 << endl; }
                
                // check after the event if this was allowed
                if(Forbidden(do_newnode->id, forbiddendests) == 1)
                {
                    if(votca::tools::globals::verbose) {cout << "Node " << do_newnode->id+1  << " is FORBIDDEN. Now selection new hopping destination." << endl; }
                    continue;
                }

                // if the new segment is unoccupied: jump; if not: add to forbidden list and choose new hopping destination
                /* if(do_newnode->occupied == 1)
                {
                    if(Surrounded(do_oldnode, forbiddendests) == 1)
                    {
                        if(votca::tools::globals::verbose) {cout << "Node " << do_oldnode->id+1  << " is SURROUNDED by forbidden destinations. Adding it to the list of forbidden nodes. After that: selection of a new escape node." << endl; }
                        AddForbidden(do_oldnode->id, forbiddennodes);
                        break; // select new escape node (ends level 2 but without setting level1step to 1)
                    }
                    if(votca::tools::globals::verbose) {cout << "Selected segment: " << do_newnode->id+1 << " is already OCCUPIED. Added to forbidden list." << endl << endl;}
                    AddForbidden(do_newnode->id, forbiddendests);
                    if(votca::tools::globals::verbose) {cout << "Now choosing different hopping destination." << endl; }
                    continue; // select new destination
                }
                else
                {
                    do_newnode->occupied = 1;
                    do_oldnode->occupied = 0;
                    do_affectedcarrier->node = do_newnode;
                    do_affectedcarrier->dr_travelled += dr;
                    level1step = 1;
                    if(votca::tools::globals::verbose) {cout << "Charge has jumped to segment: " << do_newnode->id+1 << "." << endl;}
                    break; // this ends LEVEL 2 , so that the time is updated and the next MC step started
                }*/
                if ((do_newnode->occupied == 1))// && (forbiddendests.size()>do_newnode->event.size()==1))
                {   
                    if(votca::tools::globals::verbose) {cout << "Selected segment: " << do_newnode->id+1 << " is already OCCUPIED. Added to forbidden list." << endl << endl;}
                    
                    //AddForbidden(do_newnode->id, forbiddendests);
                    //EDITED node[seg1]->AddEvent(seg2,rate12,dr,Jeff2,reorg_out);
                    do_oldnode->AddForbiddenEvent(do_newnode->id,do_oldnode->event[counter].rate);
                    
                    //cout<< " 2:Escaperate before: "<< escaperate << "Counter: " << counter << endl;
                    escaperate = (escaperate-do_oldnode->event[counter].rate);
                    do_oldnode->escaperate = escaperate;
                    //cout<< "2: Escaperate after: "<< escaperate << endl;
                    //cout << "List: " << forbiddendests.size() << " element: " << do_newnode->id <<  "Eventlistsize :" << do_oldnode->event.size() << endl;
                    if(votca::tools::globals::verbose) {cout << "Now choosing different hopping destination." << endl; }
                    
                    //ADDED TO CORRECT CODE
                    level1step = 1;
                    break;
                    //continue; // select new destination
                }
                else
                {
                    
                    do_newnode->occupied = 1;
                    do_oldnode->occupied = 0;
                    do_oldnode->escaperate = do_oldnode->initialescaperate;
                    do_oldnode->ClearForbiddenevents();
                    do_affectedcarrier->node = do_newnode;
                    do_affectedcarrier->dr_travelled += dr;
                    level1step = 1;
                    for(unsigned int j=0; j<do_oldnode->event.size(); j++)
                    {
                        if (ForbiddenEvents(do_oldnode->id,(node[do_oldnode->event[j].destination])->forbiddenevent)==1)
                        {
                            // Removes the ForbiddenEvent and adjust the escape rate
                            node[do_oldnode->event[j].destination]->RemoveForbiddenEvent(do_oldnode->id);
                            //node[do_oldnode->event[j].destination]->ClearForbiddenevents();
                            //node[do_oldnode->event[j].destination]->escaperate= node[do_oldnode->event[j].destination]->initialescaperate;
                            
                        }
                    }
                    if(votca::tools::globals::verbose) {cout << "Charge has jumped to segment: " << do_newnode->id+1 << "." << endl;}
                    break; // this ends LEVEL 2 , so that the time is updated and the next MC step started
                }
                if(votca::tools::globals::verbose) {cout << "." << endl;}
            // END LEVEL 2
            }
        // END LEVEL 1
        }    
        
        if(_outputtime != 0 && simtime > nexttrajoutput)       
        {
            nexttrajoutput = simtime + _outputtime;
            traj << simtime << "\t";
            for(unsigned int i=0; i<numberofcharges; i++) 
            {
                traj << startposition[i].getX() + carrier[i]->dr_travelled.getX() << "\t";
                traj << startposition[i].getY() + carrier[i]->dr_travelled.getY() << "\t";
                traj << startposition[i].getZ() + carrier[i]->dr_travelled.getZ();
                if (i<numberofcharges-1) 
                {
                    traj << "\t";
                }
                else
                {
                    traj << endl;
                }
            }
            
        }
        if(stopcondition == "runtime" && simtime > nextoutput)
        {
            nextoutput = simtime + outputfrequency;
            progressbar(simtime/runtime);
            cout << " remaining: ";
            printtime(int((runtime/simtime-1) * (int(time(NULL)) - realtime_start))); 
        }
        else if(stopcondition == "steps" && step > nextstepoutput)
        {
            nextstepoutput = step + outputstepfrequency;
            progressbar(double(step)/double(maxsteps));
            cout << " remaining: ";
            printtime(int((double(maxsteps)/double(step)-1) * (int(time(NULL)) - realtime_start))); 
        }
    }
    progressbar(1.);
    
    if(_outputtime != 0)
    {   
        traj.close();
    }


    // calculate occupation probabilities from occupation times    
    for(unsigned int j=0; j<node.size(); j++)
    {   
        occP[j] = node[j]->occupationtime / simtime;
    }
    

    cout << endl << "finished KMC simulation after " << step << " steps." << endl;
    cout << "simulated time " << simtime << " seconds." << endl;
    cout << "runtime: ";
    printtime(time(NULL) - realtime_start); 
    myvec dr_travelled = myvec (0,0,0);
    cout << endl << "Average velocities (m/s): " << endl;
    for(unsigned int i=0; i<numberofcharges; i++)
    {
        //cout << std::scientific << "    charge " << i+1 << ": " << carrier[i]->dr_travelled/simtime*1e-9 << endl;
        cout << std::scientific << "    charge " << i+1 << ": " << carrier[i]->dr_travelled/simtime << endl;
        dr_travelled += carrier[i]->dr_travelled;
    }
    dr_travelled /= numberofcharges;
    //cout << std::scientific << "  Overall average velocity (m/s): " << dr_travelled/simtime*1e-9 << endl;
    cout << std::scientific << "  Overall average velocity (m/s): " << dr_travelled/simtime << endl;

    cout << endl << "Distances travelled (m): " << endl;
    for(unsigned int i=0; i<numberofcharges; i++)
    {
        cout << std::scientific << "    charge " << i+1 << ": " << carrier[i]->dr_travelled << endl;
    }
    
    // calculate mobilities
    //double absolute_field = sqrt(_fieldX*_fieldX + _fieldY*_fieldY + _fieldZ*_fieldZ);
    
    if (_fieldX*_fieldX + _fieldY*_fieldY + _fieldZ*_fieldZ != 0.0)
    {
        myvec average_mobility = myvec (0.0,0.0,0.0);
        myvec fieldfactors = myvec (0.0, 0.0, 0.0);
        if (_fieldX != 0)
        {
            fieldfactors.setX(1.0E4/_fieldX);
        }
        if (_fieldY != 0)
        {
            fieldfactors.setY(1.0E4/_fieldY);
        }
        if (_fieldZ != 0)
        {
            fieldfactors.setZ(1.0E4/_fieldZ);
        }

        cout << endl << "Mobilities (cm^2/Vs): " << endl;

        for(unsigned int i=0; i<numberofcharges; i++)
        {
            myvec velocity = carrier[i]->dr_travelled/simtime;
            myvec mobility = elementwiseproduct(velocity, fieldfactors);
            average_mobility += mobility;
            cout << std::scientific << "    charge " << i+1 << ": ";
            if (_fieldX != 0)
            {
                cout << std::scientific << "muX=" << mobility.getX() << "   ";
            }
            if (_fieldY != 0)
            {
                cout << std::scientific << "muY=" << mobility.getY() << "   ";
            }
            if (_fieldZ != 0)
            {
                cout << std::scientific << "muZ=" << mobility.getZ() << "   ";
            }
            cout << endl;
        }
        if (numberofcharges != 0){
            cout << std::scientific << "  Overall average mobilities ";
            average_mobility /= numberofcharges;
            if (_fieldX != 0)
            {
                cout << std::scientific << "<muX>=" << average_mobility.getX() << "  ";
            }
            if (_fieldY != 0)
            {
                cout << std::scientific << "<muY>=" << average_mobility.getY() << "  ";
            }
            if (_fieldZ != 0)
            {
                cout << std::scientific << "<muZ>=" << average_mobility.getZ() << "  ";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    return occP;
}


void KMCMultiple::WriteOcc(vector<double> occP, vector<Node*> node)
{
    votca::tools::Database db;
    cout << "Opening for writing " << _filename << endl;
	db.Open(_filename);
	db.Exec("BEGIN;");
	votca::tools::Statement *stmt = db.Prepare("UPDATE segments SET occP"+_carriertype+" = ? WHERE _id = ?;");
	for(unsigned int i=0; i<node.size(); ++i)
        {
	    stmt->Reset();
	    stmt->Bind(1, occP[i]);
	    stmt->Bind(2, node[i]->id+1);
	    stmt->Step();
	}
	db.Exec("END;");
	delete stmt;
}

bool KMCMultiple::EvaluateFrame()
{
    std::cout << "-----------------------------------" << std::endl;      
    std::cout << "      KMC FOR MULTIPLE CHARGES" << std::endl;
    std::cout << "-----------------------------------" << std::endl << std::endl;      
 
    // Initialise random number generator
    if(votca::tools::globals::verbose) { cout << endl << "Initialising random number generator" << endl; }
    srand(_seed); // srand expects any integer in order to initialise the random number generator
    votca::tools::Random2 *RandomVariable = new votca::tools::Random2();
    RandomVariable->init(rand(), rand(), rand(), rand());
    
    vector<Node*> node;
    node = KMCMultiple::LoadGraph();
    vector<double> occP(node.size(),0.);

    occP = KMCMultiple::RunVSSM(node, _runtime, _numberofcharges, RandomVariable);
    
    // write occupation probabilites
    KMCMultiple::WriteOcc(occP, node);
    
    return true;
}

}}


#endif	/* __VOTCA_KMC_MULTIPLE_H */
