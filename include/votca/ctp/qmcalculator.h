/*
 *            Copyright 2009-2016 The VOTCA Development Team
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


#ifndef _VOTCA_CTP_QMCALCULATOR_H
#define _VOTCA_CTP_QMCALCULATOR_H


#include <votca/tools/calculator.h>
#include <votca/ctp/topology.h>

namespace CTP = votca::ctp;
namespace TOOLS = votca::tools;

namespace votca { namespace ctp {

class QMCalculator : public Calculator
{
public:

                    QMCalculator() {}
    virtual        ~QMCalculator() {}

    virtual std::string  Identify() = 0;

    virtual void    Initialize(Property *options) = 0;
    virtual bool    EvaluateFrame(CTP::Topology *top) { return true; }
    virtual void    EndEvaluate(CTP::Topology *top) { }

protected:

};

}}

#endif /* _QMCALCULATOR_H */
