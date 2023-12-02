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

package aeronauticalcommunications.AC;

//
// TODO auto-generated module
//
simple PktHandler
{
    parameters:
        
        // distance random or deterministic?
        bool dRandom = default(false);
        double distance_AC = default(0);

        // Boundaries for AC distance
        // d Interval --> [0, sqrt((x_max)^2 + (y_max)^2)]
        // x_max = M	y_max = ceil(N_BS/2) * M
        double dMin = default(0);
        
        // Signals for statistics
        @signal[waitingTime] (type=double);
        @statistic[acWaitingTimeStat](source="waitingTime"; record=mean,vector);

        @signal[responseTime] (type=double);
        @statistic[acResponseTimeStat](source="responseTime"; record=mean,vector);

        @signal[packetsInQueue] (type=double);
        @statistic[acPacketsInQueue](source="packetsInQueue"; record=mean,vector);
        
        @signal[serviceTime] (type=double);
        @statistic[acServiceTime](source="serviceTime"; record=mean,vector);
    
    gates:
        input in;
        output out[];
}
