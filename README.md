# calcmod - Max/MSP External Object

`calcmod` is a Max/MSP external object that performs modular arithmetic operations in the context of musical temperaments. It's particularly useful for working with microtonal music and different temperament systems.

## Features

- Performs addition with modulo operation
- Handles positive and negative numbers correctly
- Configurable temperament value
- Real-time calculation updates
- Detailed logging for debugging

## Installation

1. Copy the `calcmod.mxe64` file to your Max externals folder:
   ```
   C:\Program Files\Cycling '74\Max 8\resources\packages\max-mxj\java-classes\classes
   ```
2. Restart Max if it's running

## Usage

The object has 5 inlets (from left to right):

0. First sum input (hot inlet)
1. Second sum input
2. First subtraction input (not used in current version)
3. Second subtraction input (not used in current version)
4. Temperament value (default: 12)

### Example Patch

```
[10]  [7]  [0]  [0]  [19]
 |    |    |    |    |
 [calcmod]
     |
    [0]
```

### Operation

1. Set the temperament value first (e.g., 19 for 19-EDO)
2. Input values into the sum inlets
3. The result will be the sum modulo temperament

For example:
- With temperament = 19
- sum_in1 = 10
- sum_in2 = 7
- Result = (10 + 7) mod 19 = 17

## Building from Source

### Prerequisites

- Visual Studio 2022
- CMake
- Max SDK

### Build Steps

1. Clone the repository
2. Create a build directory:
   ```
   mkdir build
   cd build
   ```
3. Configure with CMake:
   ```
   cmake ..
   ```
4. Build:
   ```
   cmake --build . --config Release
   ```

## Debugging

The object includes detailed logging that can be viewed in the Max Console (Window > Max Console). The logs show:

- Input values received on each inlet
- Current state of all variables
- Step-by-step calculation process
- Final result

## License

This project is licensed under the MIT License - see the LICENSE file for details.