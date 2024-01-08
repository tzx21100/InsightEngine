/* Start Header **************************************************************/
/*!
\file    Main.cs
\author  Matthew Ng, matthewdeen.ng@digipen.edu
\par     Course: CSD2401
\date    27-09-2023
\brief   This C# file defines a set of classes and structures used for demonstration
         purposes, showcasing the integration of C# with C++ functions. It includes
         a Vector3 struct, an InternalCalls class for C++ function declarations,
         and an Entity class with various methods for interaction.

\copyright
All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/
/* End Header ****************************************************************/

using System.Runtime.CompilerServices;

namespace IS
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float X, float Y, float Z)
        {
            x = X;
            y = Y;
            z = Z;
        }
    }


    /// <summary>
    /// A demo class for demonstrating C++ function calls from C# scripts.
    /// </summary>
    public class Entity
    {
        public float FloatVar { get; set; }

        /// <summary>
        /// Constructor for the Entity class.
        /// </summary>
        public Entity()
        {
            Console.WriteLine("ctor!");
            Log("matt", 1010);

            Vector3 pos = new Vector3(10, 5.5f, 2);
            Log(ref pos);
        }

        /// <summary>
        /// Prints a "Hello World" message from C#.
        /// </summary>
        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        /// <summary>
        /// Prints an integer value received as a parameter.
        /// </summary>
        /// <param name="value">The integer value to print.</param>
        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says: {value}");
        }

        /// <summary>
        /// Prints a custom message received as a parameter.
        /// </summary>
        /// <param name="message">The custom message to print.</param>
        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

        /// <summary>
        /// Private method to log a message and an integer parameter using C++ function.
        /// </summary>
        /// <param name="str">The message to log.</param>
        /// <param name="param">The integer parameter to log.</param>
        private void Log(string str, int param)
        {
            InternalCalls.NativeLog(str, param);
        }

        /// <summary>
        /// Private method to log a Vector3 using C++ function.
        /// </summary>
        /// <param name="param">The Vector3 to log.</param>
        private void Log(ref Vector3 param)
        {
            InternalCalls.NativeLogVector(ref param);
        }
    }
}
