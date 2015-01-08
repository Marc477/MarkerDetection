#ifndef RGBIMAGE_H_
#define RGBIMAGE_H_

#include <cstring>

template<class _ColorType>
class Image
{
public:

    Image(unsigned int aWidth, unsigned int aHeight);
    Image(const Image& ao_Img);
    Image();
    virtual ~Image();

    const _ColorType    operator()(unsigned int aX, unsigned int aY) const;
    _ColorType&         operator()(unsigned int aX, unsigned int aY);

    operator const      _ColorType*() const;
    operator            _ColorType*();

    void operator=(const Image &aImg);

    void                ResetColor(_ColorType aValue);

    _ColorType          GetPixel(unsigned int aX, unsigned int aY);
    void                SetPixel(unsigned int aX, unsigned int aY, 
                                 const _ColorType &aColor);

    const _ColorType*   GetRasterData() const;
    _ColorType*         GetRasterData();
    void                SetRasterData(const _ColorType *aRaster);

    unsigned int        GetWidth() const;
    void                SetWidth(unsigned int aWidth);

    unsigned int        GetHeight() const;
    void                SetHeight(unsigned int aHeight);

    void                FlipY();
    void                FlipX();

    void                Resize(unsigned int aWidth, unsigned int aHeight);

protected:

    void _ResetRaster(bool ab_ZeroFill = true);

    _ColorType*     mRasterData;

    unsigned int    mWidth;
    unsigned int    mHeight;
};

template<class _ColorType>
inline const _ColorType Image<_ColorType>::operator()(
    unsigned int aXCoord, 
    unsigned int aYCoord) const
{
    return mRasterData[aYCoord * mWidth + aXCoord];
}

template<class _ColorType>
inline _ColorType &Image<_ColorType>::operator()(unsigned int aX,
                                                 unsigned int aY)
{
    return mRasterData[aY * mWidth + aX];
}

template<class _ColorType>
inline Image<_ColorType>::operator const _ColorType*() const
{
    return mRasterData;
}

template<class _ColorType>
inline Image<_ColorType>::operator _ColorType *()
{
    return mRasterData;
}

template<class _ColorType>
inline _ColorType Image<_ColorType>::GetPixel(unsigned int aX,
                                              unsigned int aY)
{
    return mRasterData[aY * mWidth + aX];
}

template<class _ColorType>
inline void Image<_ColorType>::SetPixel(unsigned int aX, 
                                        unsigned int aY, 
                                        const _ColorType &aColor)
{
    mRasterData[aY * mWidth + aX] = aColor;
}

template<class _ColorType>
inline unsigned int Image<_ColorType>::GetWidth() const
{
    return mWidth;
}

template<class _ColorType>
inline void Image<_ColorType>::SetWidth(unsigned int aWidth)
{
    mWidth = aWidth;
    _ResetRaster();
}

template<class _ColorType>
inline unsigned int Image<_ColorType>::GetHeight() const
{
    return mHeight;
}

template<class _ColorType>
inline void Image<_ColorType>::SetHeight(unsigned int aHeight)
{
    mHeight = aHeight;
    _ResetRaster();
}

template<class _ColorType>
inline const _ColorType* Image<_ColorType>::GetRasterData() const
{
    return mRasterData;
}

template<class _ColorType>
inline _ColorType* Image<_ColorType>::GetRasterData()
{
    return mRasterData;
}

template<class _ColorType>
inline void Image<_ColorType>::SetRasterData(const _ColorType *aRaster)
{
    memcpy(mRasterData,aRaster,mWidth*mHeight*sizeof(_ColorType));
}

template<class _ColorType>
inline void Image<_ColorType>::Resize(unsigned int aWidth, 
                                      unsigned int aHeight)
{
    mWidth  = aWidth;
    mHeight = aHeight;
    _ResetRaster();
}

#include "Image.hpp"

#endif //RGBIMAGE_H_
