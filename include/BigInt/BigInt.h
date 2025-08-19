#ifndef _YABI_BIGINT_
#define _YABI_BIGINT_

#include <cassert>
#include <cstdint>
#include <string>
#include <iostream>

namespace yabi
{

class BigInt
{
public:
    BigInt()
        : mSize(1), mIsDynamic(false)
    {
    }

    BigInt(uint32_t num)
    {
        if (num < BASE)
        {
            mSize = 1;
            mIsDynamic = false;
            mData.staticData[0] = num;
        }
        else
        {
            mIsDynamic = true;
            mSize = 2;
            mData.dynamicData = new uint32_t[mSize];
            mData.dynamicData[0] = num & MASK;
            mData.dynamicData[1] = num >> BASE_PWR;
        }
    }

    BigInt(const BigInt& other)
        : mSize(other.mSize), mIsDynamic(other.mIsDynamic)
    {
        if (mIsDynamic)
        {
            mData.dynamicData = new uint32_t[mSize];
            for (size_t i = 0; i < mSize; ++i)
            {
                mData.dynamicData[i] = other.mData.dynamicData[i];
            }
        }
        else
        {
            mData.staticData[0] = other.mData.staticData[0];
        }
    }

    BigInt& operator=(const BigInt& other)
    {
        if (this != &other)
        {
            if (mIsDynamic)
            {
                delete[] mData.dynamicData;
            }
            mSize = other.mSize;
            mIsDynamic = other.mIsDynamic;

            if (mIsDynamic)
            {
                mData.dynamicData = new uint32_t[mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    mData.dynamicData[i] = other.mData.dynamicData[i];
                }
            }
            else
            {
                mData.staticData[0] = other.mData.staticData[0];
            }
        }
        return *this;
    }


    BigInt(BigInt&& other) noexcept
        : mSize(other.mSize), mIsDynamic(other.mIsDynamic)
    {
        if (other.mIsDynamic)
        {
            mData.dynamicData = other.mData.dynamicData;
            other.mData.dynamicData = nullptr;
        }
        else
        {
            mData.staticData[0] = other.mData.staticData[0];
        }
    }

    ~BigInt()
    {
        if (mIsDynamic)
        {
            delete[] mData.dynamicData;
        }
    }

    bool convertibleToU32() const
    {
        return !mIsDynamic;
    }

    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator!=(const BigInt& lhs, const BigInt& rhs) { return !(lhs == rhs); }

    BigInt& operator+=(const BigInt& other)
    {
        if (mIsDynamic || other.mIsDynamic)
        {
            assert(false && "Dynamic addition not implemented yet");
        }
        else
        {
            mData.staticData[0] += other.mData.staticData[0];
            if (mData.staticData[0] >= BASE)
            {
                mIsDynamic = true;
                mSize = 2;
                const uint32_t oldValue = mData.staticData[0];
                mData.dynamicData = new uint32_t[mSize];
                mData.dynamicData[0] = oldValue & MASK;
                mData.dynamicData[1] = oldValue >> BASE_PWR;
            }
        }
        return *this;
    }

    BigInt& operator<<=(int32_t shift)
    {
        if (mIsDynamic)
        {
            assert(false && "Dynamic shift not implemented yet");
        }
        else
        {
            mData.staticData[0] <<= shift;
            if (mData.staticData[0] >= BASE)
            {
                mIsDynamic = true;
                mSize = 2;
                const uint32_t oldValue = mData.staticData[0];
                mData.dynamicData = new uint32_t[mSize];
                mData.dynamicData[0] = oldValue & MASK;
                mData.dynamicData[1] = oldValue >> BASE_PWR;
            }
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& obj);

private:
    static constexpr uint32_t BASE_PWR = 30;
    static constexpr uint32_t BASE = 1u << BASE_PWR;
    static constexpr uint32_t MASK = BASE - 1;

    union
    {
        uint32_t staticData[1];
        uint32_t* dynamicData;
    } mData;
    size_t mSize;
    bool mIsDynamic = false;
};

inline bool operator==(const BigInt& lhs, const BigInt& rhs)
{
    if (lhs.mIsDynamic != rhs.mIsDynamic || lhs.mSize != rhs.mSize)
    {
        return false;
    }
    if (lhs.mIsDynamic)
    {
        for (size_t i = 0; i < lhs.mSize; ++i)
        {
            if (lhs.mData.dynamicData[i] != rhs.mData.dynamicData[i])
            {
                return false;
            }
        }
    } else {
        return lhs.mData.staticData[0] == rhs.mData.staticData[0];
    }
    return true;
}

inline BigInt operator+(const BigInt& lhs, const BigInt& rhs)
{
    BigInt result = lhs;
    result += rhs;
    return result;
}

inline std::ostream& operator<<(std::ostream& os, const yabi::BigInt& obj)
{
    if (obj.mIsDynamic)
    {
        for (size_t i = 0; i < obj.mSize; ++i)
        {
            os << obj.mData.dynamicData[i];
            if (i < obj.mSize - 1)
            {
                os << " ";
            }
        }
    }
    else
{
        os << obj.mData.staticData[0];
    }
    return os;
}

} // namespace yabi


#endif // _YABI_BIGINT_
