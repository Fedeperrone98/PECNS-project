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

#ifndef __AERONAUTICALCOMMUNICATIONS_PKTHANDLER_H_
#define __AERONAUTICALCOMMUNICATIONS_PKTHANDLER_H_

#include <omnetpp.h>
#include <vector>
#include "inet/mobility/single/LinearMobility.h"
#include "inet/mobility/static/StaticGridMobility.h"
#include "AC_packet_m.h"

using namespace omnetpp;
using namespace std;
using namespace inet;


class PktHandler : public cSimpleModule
{
  private:

    inet::LinearMobility* mobility;

    std::vector<inet::Coord> BS_position;   // position of the BSs

    int id_closestBS;                       // id of the closest BS
    double dist_AC_to_BS;                   // distance between AC and closest BS, to compute s_ac
    double s_AC;                            // service time for AC

    cMessage* communication_timer;          // timer message with time s_ac to trigger send of communication packet
    cQueue* pkt_queue;                      // packet queue for communication and handover packets

    // Signals for statistics
    simsignal_t waiting_time;          // To compute waiting time in queue
    simsignal_t response_time;         // To compute response time of the packet
    simsignal_t packets_in_queue;      // To compute the number of packets in queue
    simsignal_t arrival_time;          // To compute the packet arrival time
    simsignal_t distance_AC_BS;        // To compute distance between AC and BS
    simsignal_t service_time;          // To compute mean service time of AC

  protected:
    virtual void initialize(int stage);
    virtual int numInitStages()const { return 25; }
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    void handover();
    void updateDistance_and_ServiceTime();
};

#endif
