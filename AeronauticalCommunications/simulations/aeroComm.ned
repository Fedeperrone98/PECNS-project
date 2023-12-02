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

import inet.visualizer.integrated.IntegratedVisualizer;
import inet.visualizer.scene.SceneVisualizer;

import aeronauticalcommunications.AC.AC;
import aeronauticalcommunications.BS.BS_wrapper;
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
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=6200.248,3200.128");
        }
        ac[N_AC]: AC {
            @display("p=65,165");
        }
        bs[N_BS]: BS_wrapper {
            @display("p=171,165");
        }
        ct: CT {
            @display("p=258,165");
        }
    connections:
        for i=0..N_BS-1 {
            bs[i].out --> ct.in++;
        }
        for i=0..N_AC-1, for j=0..N_BS-1 {
            ac[i].out++ --> bs[j].in++;
        }
}
