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

package aeronauticalcommunications.simulations;

import aeronauticalcommunications.AC.AC;
import aeronauticalcommunications.BS.BS;
import aeronauticalcommunications.CT.CT;


network AeroComm
{
    parameters:
        int N_BS = default(4);
        int N_AC = default(1);

        int M = default(1);
        int T = default(1);

        @display("bgb=351,330");
    submodules:
        ac[N_AC]: AC {
            @display("p=89,221");
        }
        bs[N_BS]: BS {
            @display("p=229,209");
        }
        ct: CT;
    connections:
        for i=0..N_BS-1 {
            bs[i].out --> ct.in++;
        }
        for i=0..N_AC-1, for j=0..N_BS-1 {
            ac[i].out++ --> bs[j].in++;
        }
}
