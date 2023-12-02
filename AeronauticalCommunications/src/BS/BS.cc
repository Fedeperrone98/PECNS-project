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

Define_Module(BS);

void BS::initialize()
{
    // get useful network parameter
    cModule *network = getParentModule();
    int M = network -> par("M").intValue();
    int T = network -> par("T").intValue();

    // get BS index
    int moduleID = getIndex();

    // set BS coordinate (x, y)
    x = (moduleID % 2) * M;
    y = (moduleID / 2) * M;

    // set distance between BS and CT
    d_to_CT = y + M;

    // set service time
    s_bs = T * d_to_CT;

    EV<< "BS[" << moduleID << "]: s_BS = " << s_bs << endl;

    timer = new cMessage("timer");
    pkt_queue = new cQueue();

    // Signals
    waiting_time = registerSignal("waitingTime");
    response_time = registerSignal("responseTime");
    pkts_in_queue = registerSignal("packetsInQueue");
    service_time = registerSignal("serviceTime");
}

void BS::handleMessage(cMessage *msg)
{
    if(msg->isName("timer")){
        // send the pkt to the front of the pkt queue through out gate
        cMessage *pkt = check_and_cast<cMessage*>(pkt_queue->pop());

        // emit statistics
        simtime_t arrivalTime = pkt->getArrivalTime();
        emit(waiting_time, (simTime() - arrivalTime - s_bs).dbl());
        emit(response_time, (simTime() - arrivalTime).dbl());

        send(pkt, "out");

        if(!pkt_queue->isEmpty()){
            // set timer after simTime + service time
            scheduleAt(simTime() + s_bs, timer);

            // Emit statistic: number of packets in queue --> Work Conserving
            // (is "-1" because the first packet in queue is in service)
            if (pkt_queue->getLength() > 1) {
                emit(pkts_in_queue, (double) (pkt_queue->getLength() - 1));
            }

            // Emit statistics: service time of BS
            emit(service_time, s_bs);

            EV<< "BS[" << getIndex() << "]: s_BS = " << s_bs << endl;
        }

    } else if(msg->isName("communication_pkt")){

        // insert received pkt into the pkt queue
        pkt_queue->insert(msg);

        // Emit statistic: number of packets in queue --> Work Conserving
        // (is "-1" because the first packet in queue is in service)
        emit(pkts_in_queue, (double) (pkt_queue->getLength() - 1));

        // if is the first pkt in queue
        if(pkt_queue->getLength() == 1){
            // set timer after simTime + service time
            scheduleAt(simTime() + s_bs, timer);

            // Emit statistics: service time of BS
            emit(service_time, s_bs);

            EV<< "BS[" << getIndex() << "]: s_BS = " << s_bs << endl;
        }

    }else {
        throw cRuntimeError("Unrecognize message type, abort.");
    }
}

void BS::finish(){
    cancelAndDelete(timer);

    pkt_queue->clear();
    delete pkt_queue;
}
