using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IS
{
    public static class CustomMath
    {
        // Constants
        public const float PI = 3.14159265358979323846f;

        // Convert degrees to radians
        public static float DegreesToRadians(float degrees)
        {
            return degrees * PI / 180.0f;
        }

        // Convert radians to degrees
        public static float RadiansToDegrees(float radians)
        {
            return radians * 180.0f / PI;
        }

        public static float Sin(float radians)
        {
            radians = Mod(radians + PI, 2 * PI) - PI; // Reduce the angle

            // Use a more efficient approximation
            float sine;
            if (radians < 0)
            {
                sine = 1.27323954f * radians + .405284735f * radians * radians;
                if (sine < 0)
                    sine = .225f * (sine * -sine - sine) + sine;
                else
                    sine = .225f * (sine * sine - sine) + sine;
            }
            else
            {
                sine = 1.27323954f * radians - 0.405284735f * radians * radians;
                if (sine < 0)
                    sine = .225f * (sine * -sine - sine) + sine;
                else
                    sine = .225f * (sine * sine - sine) + sine;
            }
            return sine;
        }

        // Improved Cosine function
        public static float Cos(float radians)
        {
            return Sin(radians + PI / 2);
        }

        // Modulus function that handles negative values correctly
        private static float Mod(float a, float b)
        {
            return (a % b + b) % b;
        }

        // Raise a number to a power
        public static float Power(float baseNum, int exponent)
        {
            float result = 1;
            for (int i = 0; i < exponent; i++)
            {
                result *= baseNum;
            }
            return result;
        }

        // Calculate factorial of a number
        public static int Factorial(int number)
        {
            int result = 1;
            for (int i = 1; i <= number; i++)
            {
                result *= i;
            }
            return result;
        }

    }
}
