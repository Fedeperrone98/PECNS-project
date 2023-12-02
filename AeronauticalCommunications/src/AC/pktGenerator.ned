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


simple PktGenerator
{
    parameters:
        int kDistribution = default(1); //1: uniform || 2: exp
        
        // Handover interval
        double t @unit(s) = default(1s);
        
        // K random or deterministic?
        bool kRandom = default(false);
        double k @unit(s) = default(1s);
        
        // 1: uniform
        double kMin @unit(s) = default(0s);
        double kMax @unit(s) = default(1s);
        
        // 2: exp
        double kMean @unit(s) = default(1s);
    gates:
        output out;
}
