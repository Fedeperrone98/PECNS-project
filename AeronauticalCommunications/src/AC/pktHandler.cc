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

#include "pktHandler.h"
#include <algorithm>

using namespace inet;
using namespace std;

Define_Module(PktHandler);

void PktHandler::initialize(int stage)
{
    if(stage == 24){
        // Reference to ac mobility module
        mobility = reinterpret_cast<LinearMobility*> ( getModuleByPath("^.mobility") );

        // Total number of BSs, to compute distance_BS
        int N_BS = par("N_BS").intValue();

        // Get position of the BSs
        cModule *net = getParentModule()->getParentModule();
        for (int i = 0; i < N_BS; i++) {
            cModule *BS_wrapper = net->getSubmodule("bs", i);
            inet::StaticGridMobility* bs_mobility = reinterpret_cast<StaticGridMobility*> (BS_wrapper->getSubmodule("mobility"));
            Coord bsPos = bs_mobility->getCurrentPosition();
            BS_position.push_back(bsPos);
        }

        // Get new serving BS
        handover();

        // Init timers
        communication_timer = new cMessage("communication_timer");
        //handover_timer = new cMessage("handover_timer");

        // Init pkt queue
        pkt_queue = new cQueue();

        // Register signals to compute statistics
        waiting_time = registerSignal("waitingTime");
        response_time = registerSignal("responseTime");
        packets_in_queue = registerSignal("packetsInQueue");
        distance_AC_BS = registerSignal("distance_AC_BS");
        service_time = registerSignal("serviceTime");
    }
}


void PktHandler::handleMessage(cMessage *msg)
{
    // Message handler:
    // Communication or handover packet received
    if (msg->isName("communication_pkt") || msg->isName("handover_pkt")) {

       // Insert received packet into the pkt_queue
       pkt_queue->insert(msg);

       // Emit statistic: number of packets in queue --> Work Conserving
       // (is "-1" because the first packet in queue is in service)
       emit(packets_in_queue, (double) (pkt_queue->getLength() - 1));

       // if it is the first packet in queue
       if(pkt_queue->getLength() == 1){
           // Set timer after simTime + service time s_AC
           scheduleAt(simTime() + s_AC, communication_timer);

           // Emit statistic: distance between AC and BS
           emit(distance_AC_BS, dist_AC_to_BS);
           // Emit statistic: service time of AC
           emit(service_time, s_AC);
       }

       EV<< "pktHandler: s_AC = " << s_AC << endl;

    }
    // Timer triggered
    else if (msg->isSelfMessage()) {
        // Pop packet from the queue
        cMessage *pkt = check_and_cast<cMessage*>(pkt_queue->pop());

        // Emit statistics: waiting time and response time
        simtime_t arrivalTime = pkt->getArrivalTime();
        emit(waiting_time, (simTime() - arrivalTime - s_AC).dbl());
        emit(response_time, (simTime() - arrivalTime).dbl());

        // Check packet type:
        // Communication packet --> forward through output gate
        if (pkt->isName("communication_pkt")) {
            send(pkt, "out", id_closestBS);
        }
        // Handover packet --> change serving BS
        else {
            handover();
            delete(pkt);
        }

        // Serve the next packet
        if(!pkt_queue->isEmpty()) {
            // Set timer after simTime + service time s_AC
            scheduleAt(simTime() + s_AC, communication_timer);

            // Emit statistic: number of packets in queue --> Work Conserving
            // (is "-1" because the first packet in queue is in service)
            if (pkt_queue->getLength() > 1) {
                emit(packets_in_queue, (double) (pkt_queue->getLength() - 1));
            }

            // Emit statistic: distance between AC and BS
            emit(distance_AC_BS, dist_AC_to_BS);
            // Emit statistic: service time of AC
            emit(service_time, s_AC);
        }
    }
}

// Clear timer and queue
void PktHandler::finish()
{
    cancelAndDelete(communication_timer);

    pkt_queue->clear();
    delete pkt_queue;
}

// Handover operation --> change serving BS
void PktHandler::handover()
{
    // Extract distance of AC
    Coord acPos = mobility->getCurrentPosition();

    int N_BS = par("N_BS").intValue();
    double T = par("T").doubleValue();

    double minDistance = std::numeric_limits<double>::max();  // init a max value
    // Compute distances
    for (int i = 0; i < N_BS; ++i) {
        double distance = BS_position[i].distance(acPos);
        EV << "pktHandler (handover): distance to BS_" << i << " = " << distance << endl;
        if (distance < minDistance) {
            minDistance = distance;
            id_closestBS = i;
        }
    }

    // Update distance between AC and serving BS, and its service time s_AC
    dist_AC_to_BS = minDistance;
    if(par("validation").boolValue()){
        s_AC = T * dist_AC_to_BS;
    }else{
        s_AC = T * pow(dist_AC_to_BS, 2);
    }

    EV << "pktHandler (handover): dist_AC_to_BS = " << dist_AC_to_BS << endl;
    EV << "pktHandler (handover): id_closestBS = " << id_closestBS << endl;

}
