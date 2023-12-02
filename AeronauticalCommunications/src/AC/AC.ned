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
import inet.mobility.contract.IMobility;

module AC
{
    parameters:
        int N_BS = default(4);
        @display("bgb=327,106");
    gates:
        output out[]; // to base stations
    submodules:
        pktGenerator: PktGenerator {
            @display("p=59,41");
        }
        pktHandler: PktHandler {
            @display("p=216,41");
        }
        mobility: <> like IMobility {
            @display("p=135.3,77.549995");
        }
    connections:
        pktGenerator.out --> pktHandler.in;
        for i=0..N_BS-1 {
            pktHandler.out++ --> out[i];
        }
}
