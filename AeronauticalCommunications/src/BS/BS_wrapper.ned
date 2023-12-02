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
import inet.mobility.contract.IMobility;

module BS_wrapper
{
    parameters:
        int N_AC = default(1);

    gates:
        input in[];
        output out;
    submodules:
        bs: BS {
            @display("p=295,77");
        }
        mobility: <> like IMobility {
            @display("p=135.3,77.549995");
        }
    connections:
        for i=0..N_AC-1 {
            in[i] --> bs.in++;
        }

        bs.out --> out;
}
