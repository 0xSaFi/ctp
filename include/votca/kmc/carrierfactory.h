/*
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __VOTCA_KMC_CARRIERFACTORY_H
#define	__VOTCA_KMC_CARRIERFACTORY_H

#include <votca/tools/objectfactory.h>
#include "carrier.h"

namespace votca { namespace kmc {

using namespace tools;

class CarrierFactory
: public ObjectFactory< std::string, Carrier >
{
private:
    CarrierFactory() {}
public:
    
    static void RegisterAll(void);

    friend CarrierFactory &Carriers();
};

inline CarrierFactory &Carriers()
{
    static CarrierFactory _instance;
    return _instance;
}

}}

#endif	/* __VOTCA_KMC_CARRIERFACTORY_H */

