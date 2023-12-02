//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "BS.h"

using namespace inet;

Define_Module(BS);

void BS::initialize()
{

    // Get reference to mobility module
    mobility = reinterpret_cast<StaticGridMobility*> ( getModuleByPath("^.mobility") );

    // set BS coordinate (x, y)
    bsPositions = mobility->getCurrentPosition();

    EV<< "BS[" << getIndex() << "]: (" << bsPositions.getX() << ", " << bsPositions.getY() << ")" << endl;
}

void BS::handleMessage(cMessage *msg)
{
    if(msg->isName("communication_pkt")) {
        send(msg, "out");
    } else {
        throw cRuntimeError("Unrecognize message type, abort.");
    }
}

void BS::finish(){
    cSimpleModule::finish();
}
