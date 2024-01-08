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

        public static float clamp(float val,float min, float max) {
            val = CustomMath.max(val, min);
            val = CustomMath.min(val, max);
            return val; 
            
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

            return InternalCalls.MathSin(radians);

/*            radians = Mod(radians + PI, 2 * PI) - PI; // Normalize the angle

            float result = radians - (radians * radians * radians) / 6f
                           + (radians * radians * radians * radians * radians) / 120f
                           - (radians * radians * radians * radians * radians * radians * radians) / 5040f;

            return result;*/
        }

        // Improved Cosine function
        public static float Cos(float radians)
        {
            return InternalCalls.MathCos(radians);
            //return Sin(radians + PI / 2);
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

            return InternalCalls.MathAtan2(y, x);
/*            if (x > 0)
            {
                return Atan(y / x);
            }
            else if (x < 0)
            {
                if (y >= 0) return Atan(y / x) + PI;
                else return Atan(y / x) - PI;
            }
            else // x == 0
            {
                if (y > 0) return PI / 2;
                if (y < 0) return -PI / 2;
                return 0; // x and y are 0, angle is undefined
            }*/
        }


        // Simple Atan approximation - Can be improved
        private static float Atan(float z)
        {
            return InternalCalls.MathAtan(z);
/*
            const float n1 = 0.97239411f;
            const float n2 = -0.19194795f;
            float result = 0.0f;

            bool signChange = false;
            bool invert = false;
            int sp = 0;

            if (z < 0.0f)
            {
                z = -z;
                signChange = true;
            }

            if (z > 1.0f)
            {
                z = 1.0f / z;
                invert = true;
            }

            while (z > 0.28f)
            {
                sp++;
                z = ((z * z) - 1.0f) / (2.0f * z + 1.0f);
            }

            result = z * (n1 + n2 * z * z);

            while (sp > 0)
            {
                result += 0.5235987756f; // PI/6
                sp--;
            }

            if (invert) result = 1.57079632679f - result; // PI/2 - result
            if (signChange) result = -result;

            return result;*/
        }


        public static float Abs(float value)
        {
            if (value < 0)
                return -value;
            else
                return value;
        }        
        
        public static float Normalize(float value)
        {
            if (value < 0)
                return -1;
            else
                return 1;
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

        public static double Power(double baseValue, int exponent)
        {
            double result = 1;
            for (int i = 0; i < exponent; i++)
            {
                result *= baseValue;
            }
            return result;
        }
        public static double Sqrt(double number)
        {
            return InternalCalls.MathSqrt((float)number);

/*            double tolerance = 1e-1; // Precision of the result
            double guess = number / 2; // Initial guess
            while (CustomMath.Abs((float)(number - guess * guess)) > tolerance)
            {
                guess = (guess + number / guess) / 2;
            }
            return guess;*/
        }


    }
}
