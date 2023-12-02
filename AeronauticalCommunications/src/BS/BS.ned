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

package aeronauticalcommunications.BS;

//
// TODO auto-generated module
//
simple BS
{
    parameters:
        //signals for statistics        
        @signal[waitingTime] (type=double);
        @statistic[bsWaitingTimeStat](source="waitingTime"; record=mean,vector);
        
        @signal[responseTime] (type=double);
        @statistic[bsResponseTimeStat](source="responseTime"; record=mean,vector);
        
        @signal[packetsInQueue] (type=double);
        @statistic[bsPacketsInQueue](source="packetsInQueue"; record=mean,vector);
        
        @signal[serviceTime] (type=double);
        @statistic[bsServiceTime](source="serviceTime"; record=mean,vector);
        
    gates:
        input in[];
        output out;
}
