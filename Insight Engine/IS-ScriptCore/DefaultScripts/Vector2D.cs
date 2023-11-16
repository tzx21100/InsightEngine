using System;
using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public struct SimpleVector2D
{
    public float x;
    public float y;
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

    // Addition of two vectors
    public Vector2D Add(Vector2D other)
    {
        return new Vector2D(this.x + other.x, this.y + other.y);
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

}
