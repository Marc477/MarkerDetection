#include "ColorRGB.h"

const ColorRGB ColorRGB::PureRed(1.0f,0.0f,0.0f);
const ColorRGB ColorRGB::PureGreen(0.0f,1.0f,0.0f);
const ColorRGB ColorRGB::PureBlue(0.0f,0.0f,1.0f);
const ColorRGB ColorRGB::White(1.0f,1.0f,1.0f);
const ColorRGB ColorRGB::Black(0.0f,0.0f,0.0f);
const ColorRGB ColorRGB::Invalid(-1.0f,-1.0f,-1.0f);

ColorRGB::ColorRGB()
: mR(0.0f)
, mG(0.0f)
, mB(0.0f)
{}

ColorRGB::ColorRGB(const ColorRGB &aColor)
: mR(aColor.mR)
, mG(aColor.mG)
, mB(aColor.mB)
{}

ColorRGB::ColorRGB(float aR, float aG, float aB)
:    mR(aR)
,    mG(aG)
,    mB(aB)
{}

void ColorRGB::Clamp()
{
    for(int i = 0; i < 3; ++i)
    {
        if(mComponents[i] < 0.0f)
        {
            mComponents[i] = 0.0f;
        }
        else if(mComponents[i] > 1.0f)
        {
            mComponents[i] = 1.0f;
        }
    }
}

ColorRGB& ColorRGB::operator=(const ColorRGB &aRHS)
{
    mR = aRHS.mR;
    mG = aRHS.mG;
    mB = aRHS.mB;

    return *this;
}

ColorRGB ColorRGB::operator+(const ColorRGB &aRHS) const
{
    return ColorRGB(    mR + aRHS.mR,
                        mG + aRHS.mG,
                        mB + aRHS.mB);
}

ColorRGB ColorRGB::operator-(const ColorRGB &aRHS) const
{
    return ColorRGB(    mR - aRHS.mR,
                        mG - aRHS.mG,
                        mB - aRHS.mB);
}

ColorRGB ColorRGB::operator*(const ColorRGB &aRHS) const
{
    return ColorRGB(    mR * aRHS.mR,
                        mG * aRHS.mG,
                        mB * aRHS.mB);
}

ColorRGB ColorRGB::operator*(float aRHS) const
{
    return ColorRGB(    mR * aRHS,
                        mG * aRHS,
                        mB * aRHS);
}

ColorRGB ColorRGB::operator/(const ColorRGB &aRHS) const
{
    return ColorRGB(    mR / aRHS.mR,
                        mG / aRHS.mG,
                        mB / aRHS.mB);
}

ColorRGB ColorRGB::operator/( float aRHS) const
{
    return ColorRGB(    mR / aRHS,
                        mG / aRHS,
                        mB / aRHS);
}

ColorRGB& ColorRGB::operator +=(const ColorRGB &aRHS)
{
    mR += aRHS.mR;
    mG += aRHS.mG;
    mB += aRHS.mB;

    return *this;
}

ColorRGB& ColorRGB::operator -=(const ColorRGB &aRHS)
{
    mR -= aRHS.mR;
    mG -= aRHS.mG;
    mB -= aRHS.mB;

    return *this;
}

ColorRGB& ColorRGB::operator *=(const ColorRGB &aRHS)
{
    mR *= aRHS.mR;
    mG *= aRHS.mG;
    mB *= aRHS.mB;

    return *this;
}

ColorRGB& ColorRGB::operator *=(float aRHS)
{
    mR *= aRHS;
    mG *= aRHS;
    mB *= aRHS;

    return *this;
}

ColorRGB& ColorRGB::operator /=(const ColorRGB &aRHS)
{
    mR /= aRHS.mR;
    mG /= aRHS.mG;
    mB /= aRHS.mB;

    return *this;
}

ColorRGB& ColorRGB::operator /=(float aRHS)
{
    mR /= aRHS;
    mG /= aRHS;
    mB /= aRHS;

    return *this;
}

bool ColorRGB::operator==(const ColorRGB &aRHS) const
{
    return  mR == aRHS.mR &&
            mG == aRHS.mG &&
            mB == aRHS.mB;
}

bool ColorRGB::operator!=(const ColorRGB &aRHS) const
{
    return  mR != aRHS.mR    ||
            mG != aRHS.mG        ||
            mB != aRHS.mB;
}

ColorRGB operator*(float aLHS, ColorRGB aRHS)
{
    return ColorRGB(    aLHS * aRHS.R(),
                        aLHS * aRHS.G(),
                        aLHS * aRHS.B());
}
