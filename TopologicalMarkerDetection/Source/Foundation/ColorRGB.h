#ifndef COLORRGB_H_
#define COLORRGB_H_

#include<cassert>
#include <cmath>

class ColorRGB
{
public:

	ColorRGB();
	ColorRGB(const ColorRGB &aColor);
	ColorRGB(float aR, float aG, float aB);

	// Will set color components to 0.0 if they're below 0.0 or 
    // 1.0 if they're above 1.0.
	void    Clamp();

	//Accessors
	operator const float*() const;
	operator float*();

	float   operator[](int i) const;
	float&  operator[](int i);

	float   R() const;
	float&  R();
	float   G() const;
	float&  G();
	float   B() const;
	float&  B();

	float   GetBrightness() const;

	//Assignement operator
	ColorRGB& operator=(const ColorRGB &aRHS);
	
	//Arithmetic operators
	ColorRGB operator+(const ColorRGB &aRHS) const;
	ColorRGB operator-(const ColorRGB &aRHS) const;
	ColorRGB operator*(const ColorRGB &aRHS) const;
	ColorRGB operator*(float aRHS) const;
	ColorRGB operator/(const ColorRGB &aRHS) const;
	ColorRGB operator/(float aRHS) const;

	//Arithmetic updates
	ColorRGB& operator +=(const ColorRGB &aRHS);
	ColorRGB& operator -=(const ColorRGB &aRHS);
	ColorRGB& operator *=(const ColorRGB &aRHS);
	ColorRGB& operator *=(float aRHS);
	ColorRGB& operator /=(const ColorRGB &aRHS);
	ColorRGB& operator /=(float aRHS);
	
	//Comparison operators
	bool operator==(const ColorRGB &aRHS) const;
	bool operator!=(const ColorRGB &aRHS) const;

	//Built-in colors
	static const ColorRGB PureRed;
	static const ColorRGB PureGreen;
	static const ColorRGB PureBlue;
	static const ColorRGB White;
	static const ColorRGB Black;
	static const ColorRGB Invalid;

protected:

	union
	{
		float mComponents[3];
		struct 
		{
			float mR;
			float mG;
			float mB;
		};

	};
};

ColorRGB operator*(float aLHS, ColorRGB aRHS);

inline ColorRGB::operator const float*() const
{
	return mComponents;
}

inline ColorRGB::operator float*()
{
	return mComponents;
}

inline float ColorRGB::operator[](int i) const
{
	assert(i >= 0 && i <= 2);
	return mComponents[i];
}

inline float& ColorRGB::operator[](int i)
{
	assert(i >= 0 && i <= 2);
	return mComponents[i];
}

inline float& ColorRGB::R()
{
	return mR;
}

inline float ColorRGB::R() const
{
	return mR;
}

inline float& ColorRGB::G()
{
	return mG;
}

inline float ColorRGB::G() const
{
	return mG;
}

inline float& ColorRGB::B()
{
	return mB;
}

inline float ColorRGB::B() const
{
	return mB;
}

inline float ColorRGB::GetBrightness() const
{
	//return (mR + mG + mB)/3.0f;
    return 0.3f*mR + 0.59f*mG + 0.11f*mB;
}

#endif //COLORRGB_H_
