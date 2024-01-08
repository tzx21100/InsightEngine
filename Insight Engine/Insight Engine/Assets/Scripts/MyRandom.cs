using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

public class MyRandom
{
    private uint _current;

    // Constants for the LCG algorithm
    private const uint a = 1664525;
    private const uint c = 1013904223;

    // Constructor that seeds the generator
    public MyRandom(uint seed)
    {
        _current = seed;
    }

    // Generate the next random integer
    public uint Next()
    {
        _current = a * _current + c;  // Removed % m
        return _current;
    }

    // Generate a random integer between minValue and maxValue
    public uint Next(uint minValue, uint maxValue)
    {
        if (minValue > maxValue)
            throw new ArgumentOutOfRangeException("minValue must be less than or equal to maxValue");

        uint result = Next();

        // Scale the result to the desired range
        return minValue + result % (maxValue - minValue);
    }

    // Generate a random double between 0.0 and 1.0
    public double NextDouble()
    {
        // Divide by 2^32 to get a floating point number in the range [0, 1)
        return (double)Next() / (1L << 32);
    }    
    
    public float NextFloat()
    {
        // Divide by 2^32 to get a floating point number in the range [0, 1)
        return (float)Next() / (1L << 32);
    }
}
