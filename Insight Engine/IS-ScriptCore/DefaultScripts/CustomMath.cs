using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IS
{
    public static class CustomMath
    {
        public static float min(float x, float y) { 
            if(x < y) return x; return y;
        }        
        public static float max(float x, float y) { 
            if(x > y) return x; return y;
        }
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

        public static float Atan2(float y, float x)
        {
            if (x > 0) return Atan(y / x);
            if (y >= 0 && x < 0) return Atan(y / x) + PI;
            if (y < 0 && x < 0) return Atan(y / x) - PI;
            if (y > 0 && x == 0) return PI / 2;
            if (y < 0 && x == 0) return -PI / 2;
            return 0; // Undefined, ideally should handle as an error
        }

        // Simple Atan approximation - Can be improved
        private static float Atan(float z)
        {
            // Coefficients for a polynomial approximation
            const float a1 = 0.9998660f;
            const float a2 = -0.3302995f;
            const float a3 = 0.1801410f;
            const float a4 = -0.0851330f;
            const float a5 = 0.0208351f;

            float az = Abs(z);
            bool large = az > 1.0f;

            if (large)
                az = 1.0f / az;

            float az2 = az * az;
            float az3 = az2 * az;
            float az4 = az3 * az;
            float az5 = az4 * az;

            float atanApprox = a1 * az - a2 * az2 + a3 * az3 - a4 * az4 + a5 * az5;

            if (large)
                atanApprox = PI / 2 - atanApprox;

            return z < 0 ? -atanApprox : atanApprox;
        }

        public static float Abs(float value)
        {
            if (value < 0)
                return -value;
            else
                return value;
        }

        public static float AngleBetweenPoints(Vector2D point1, Vector2D point2)
        {
            float deltaY = point2.y - point1.y;
            float deltaX = point2.x - point1.x;
            return CustomMath.Atan2(deltaY, deltaX);
        }

        public static float ToDegrees(float radians)
        {
            return radians * 180.0f / PI;
        }

    }
}
