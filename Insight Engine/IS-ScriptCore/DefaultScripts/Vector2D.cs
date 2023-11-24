using IS;
using System;
using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public struct SimpleVector2D
{
    public float x;
    public float y;

    public SimpleVector2D(float x, float y)
    {
        this.x = x;
        this.y = y;
    }
}


public class Vector2D
{
    public float x { get; set; }
    public float y { get; set; }


    public Vector2D(float x, float y)
    {
        this.x = x;
        this.y = y;
    }




    // Lerp
    public static float lerp(float start, float end, float t)
    {
        return (1 - t) * start + t * end;
    }

    public static float smoothLerp(float start, float end, float t)
    {
        // Using a smooth step function for smoother interpolation on Y axis.
        t = t * t * (3f - 2f * t);
        return (1 - t) * start + t * end;
    }

    public static Vector2D Lerp(Vector2D start, Vector2D end, float t)
    {
        return new Vector2D(
            lerp(start.x, end.x, t),
            lerp(start.y, end.y, t)
        );
    }

    public static Vector2D LerpGameCam(Vector2D start, Vector2D end, float t)
    {
        return new Vector2D(
            lerp(start.x, end.x, t),  // Linear interpolation for X axis.
            smoothLerp(start.y, end.y, t)  // Smoother interpolation for Y axis.
        );
    }



    // Addition of two vectors
    public Vector2D Add(Vector2D other)
    {
        return new Vector2D(this.x + other.x, this.y + other.y);
    }    
    
    public void Set(float x,float y)
    {
        this.x = x;
        this.y = y;
    }

    // Subtraction of two vectors
    public Vector2D Subtract(Vector2D other)
    {
        return new Vector2D(this.x - other.x, this.y - other.y);
    }

    // Scalar multiplication
    public Vector2D Multiply(float scalar)
    {
        return new Vector2D(this.x * scalar, this.y * scalar);
    }

    // Scalar division
    public Vector2D Divide(float scalar)
    {
        if (scalar == 0) throw new DivideByZeroException("Division by zero is not allowed.");
        return new Vector2D(this.x / scalar, this.y / scalar);
    }

    // Dot product
    public float Dot(Vector2D other)
    {
        return this.x * other.x + this.y * other.y;
    }

    // Magnitude (length) of the vector
    public float Magnitude()
    {
        return (float)Math.Sqrt(this.x * this.x + this.y * this.y);
    }

    // Normalize the vector
    public Vector2D Normalize()
    {
        float magnitude = Magnitude();
        if (magnitude == 0) return new Vector2D(0, 0); // Return zero vector if magnitude is zero
        return Divide(magnitude);
    }

    // Override ToString for easier debugging
    public override string ToString()
    {
        return $"({x}, {y})";
    }

    // Functionalities to convert to SimpleVector2D
    public SimpleVector2D ToSimpleVector2D()
    {
        return new SimpleVector2D { x = this.x, y = this.y };
    }

    public static Vector2D FromSimpleVector2D(SimpleVector2D simpleVec)
    {
        return new Vector2D(simpleVec.x, simpleVec.y);
    }

    // Rotate the vector by an angle in degrees
    public Vector2D Rotate(float angleDegrees)
    {
        float angleRadians = DegreesToRadians(angleDegrees);
        float cos = (float)Math.Cos(angleRadians);
        float sin = (float)Math.Sin(angleRadians);

        return new Vector2D(
            this.x * cos - this.y * sin,
            this.x * sin + this.y * cos
        );
    }

    // Convert degrees to radians
    private static float DegreesToRadians(float degrees)
    {
        return degrees * (float)Math.PI / 180f;
    }

    // Convert radians to degrees (if needed)
    private static float RadiansToDegrees(float radians)
    {
        return radians * 180f / (float)Math.PI;
    }

    public static Vector2D DirectionFromAngle(float angleRadians)
    {
        return new Vector2D(
            CustomMath.Cos(angleRadians),  // x component
            CustomMath.Sin(angleRadians)   // y component
        );
    }

    public Vector2D ApplyForce(float angleRadians, float forceMagnitude)
    {
        Vector2D forceDirection = DirectionFromAngle(angleRadians);
        return forceDirection.Multiply(forceMagnitude); // Scale the direction by the force magnitude
    }

    public static float Distance(Vector2D a, Vector2D b)
    {
        return (float)CustomMath.Sqrt(CustomMath.Abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
    }

}
