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

#include "pktGenerator.h"

Define_Module(PktGenerator);

void PktGenerator::initialize()
{
    timer_k = new cMessage("timer_k");
    timer_t = new cMessage("timer_t");

    simtime_t interval_k;
    // For the timers of messages
    // k --> randomly or deterministically (DEBUG) distributed   t --> deterministic
    if (par("kRandom").boolValue()) {
        interval_k = computeInterval_k();
    } else {
        interval_k = par("k").doubleValue();
    }
    simtime_t interval_t = par("t").doubleValue();

    // Schedule timers
    scheduleAt(simTime() + interval_k, timer_k);
    scheduleAt(simTime() + interval_t, timer_t);

    EV<< "pktGenerator: k = " << interval_k << endl;
}

void PktGenerator::handleMessage(cMessage *msg)
{

    AC_packet* pkt;

    // Check timer triggered

    // Timer k triggered --> generate communication packet
    if (strcmp(msg->getName(), "timer_k") == 0) {
        // Send communication packet
        pkt = new AC_packet("communication_pkt");
        send(pkt, "out");

        // Schedule a new timer for communication packet

        simtime_t interval_k;
        // DEBUG MODE --> k Random or Deterministic?
        if (par("kRandom").boolValue()) {
            interval_k = computeInterval_k();
        } else {
            interval_k = par("k").doubleValue();
        }

        scheduleAt(simTime() + interval_k, timer_k);

        EV<< "pktGenerator: k = " << interval_k << endl;
    }

    // Timer t triggered --> generate handover packet
    else {
        // Send handover packet
        pkt = new AC_packet("handover_pkt");
        send(pkt, "out");

        // Schedule a new timer for handover packet
        simtime_t interval_t = par("t").doubleValue();
        scheduleAt(simTime() + interval_t, timer_t);
    }
}

// To compute and get interval k
double PktGenerator::computeInterval_k()
{
    // Get distribution for k
    int k_distribution = par("kDistribution").intValue();

    // Compute k
    if (k_distribution == 1) {
        return uniform(par("kMin").doubleValue(), par("kMax").doubleValue(), TIME_RNG);
    } else {
        return exponential(par("kMean").doubleValue(), TIME_RNG);
    }
}

// Clear timers
void PktGenerator::finish() {
    cancelAndDelete(timer_k);
    cancelAndDelete(timer_t);
}
