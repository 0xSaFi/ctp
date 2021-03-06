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
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <votca/kmc/carrierfactory.h>
#include "carriers/electron.h"
#include "carriers/hole.h"
#include "carriers/energy.h"

//BOOST_CLASS_EXPORT( votca::kmc::Electron )

namespace votca { namespace kmc {

void CarrierFactory::RegisterAll(void)
{
    Carriers().Register<Electron>("electron");
    Carriers().Register<Hole>("hole");
    Carriers().Register<Energy>("energy");
}

std::vector<BNode*> Electron::e_occupiedNodes = {};
std::vector<BNode*> Hole::h_occupiedNodes = {};
std::vector<BNode*> Energy::energy_occupiedNodes = {};

}}
