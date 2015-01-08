
template<class _ColorType>
Image<_ColorType>::Image(unsigned int aWidth, unsigned int aHeight)
:    mWidth(aWidth)
,    mHeight(aHeight)
,    mRasterData(NULL)
{
    _ResetRaster();
}

template <class _ColorType>
Image<_ColorType>::Image(const Image& ao_Img)
:    mWidth(ao_Img.mWidth)
,    mHeight(ao_Img.mHeight)
,    mRasterData(NULL)
{
    _ResetRaster(false);
    SetRasterData(ao_Img.GetRasterData());
}

template<class _ColorType>
Image<_ColorType>::Image()
:    mWidth(1)
,    mHeight(1)
,    mRasterData(NULL)
{
    _ResetRaster();
}

template<class _ColorType>
void Image<_ColorType>::operator=(const Image<_ColorType> &aImg)
{
    if(mWidth != aImg.mWidth || mHeight != aImg.mHeight)
    {
        mWidth = aImg.mWidth;
        mHeight = aImg.mHeight;

        delete[] mRasterData;

        mRasterData = new _ColorType[mWidth*mHeight];
        
    }

    SetRasterData(aImg.mRasterData);
}

template<class _ColorType>
void Image<_ColorType>::_ResetRaster(bool mb_ZeroFill /*= true*/)
{
    delete[] mRasterData;
    mRasterData = new _ColorType[mWidth*mHeight];

    if(mb_ZeroFill)
    {
        memset(mRasterData,0,mWidth*mHeight*sizeof(_ColorType));
    }
}

template<class _ColorType>
void Image<_ColorType>::ResetColor(_ColorType aValue)
{
    for(unsigned int y = 0; y < GetHeight(); ++y)
    {
        for(unsigned int x = 0; x < GetWidth(); ++x)
        {
            SetPixel(x,y,aValue);
        }
    }
}

template<class _ColorType>
void Image<_ColorType>::FlipY()
{
    _ColorType TmpColor;
    _ColorType* TmpLineBuffer = new _ColorType[mWidth];


    for(unsigned int y = 0; y < mHeight/2; ++y)
    {
        memcpy( TmpLineBuffer,
                mRasterData[y * mWidth], 
                mWidth * sizeof(_ColorType));

        memcpy( mRasterData[y * mWidth],
                mRasterData[(mHeight-y-1) * mWidth],
                mWidth * sizeof(_ColorType));

        memcpy( mRasterData[(mHeight-y-1) * mWidth],
                TmpLineBuffer,
                mWidth * sizeof(_ColorType));
    }

    delete[] TmpLineBuffer;
    TmpLineBuffer = NULL;
}

template<class _ColorType>
void Image<_ColorType>::FlipX()
{
    assert(false); //NOT IMPLEMENTED
}

template<class _ColorType>
Image<_ColorType>::~Image()
{
    if(mRasterData)
    {
        delete[] mRasterData;
        mRasterData = NULL;
    }
}
