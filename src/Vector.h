#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>


class CVector
{
	public:
	
		// Constructors
		CVector() : x(0.0f), y(0.0f), z(0.0f) {}
		CVector(float xxx, float yyy, float zzz) : x(xxx), y(yyy), z(zzz) {}
		
		// Copy Constructor
		CVector(const CVector &vec) : x(vec.x), y(vec.y), z(vec.z) {}

		// Overloaded Operators
		CVector& operator =(const CVector &vec);
		CVector operator +(const CVector &vec) const;
		CVector operator -(const CVector &vec) const;
		CVector operator -() const;
		void operator +=(const CVector &vec);
		void operator -=(const CVector &vec);
		bool operator ==(const CVector &vec) const;
		bool operator !=(const CVector &vec) const;

		// '*' will perform the dot product operation
		// between two vectors
		float operator *(const CVector &vec) const;

		// Sets the CVector to the passed in values
		void set(float xxx, float yyy, float zzz);
		void set(const CVector &vec);

		void negate(); // Negates the CVector
		void normalize(); // Normalizes the CVector
		void scale(float amt); // Scales the CVector by the amount passed in
		
		float magnitude() const; // Returns the magnitude of the CVector

		// Cross product
		void crossProduct(const CVector &vec, CVector &result) const;
		CVector crossProduct(const CVector &vec) const;
		
		// Friend function
		// Overloaded scalar multiplication
		friend inline CVector operator *(const CVector &vec, float scalar);

		// Public data
		float x;
		float y;
		float z;
};

// Friend Function
// Overloaded scalar multiplication
CVector operator*(const CVector &vec, float scalar)
{
	return CVector(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

// Checks for two floats to be equal within a certain tolerance
inline bool TEqual(float a, float b, float t = 0.001f)
{
	return ((a > b - t) && (a < b + t));
}


// Typedef
typedef CVector CPos;


#endif