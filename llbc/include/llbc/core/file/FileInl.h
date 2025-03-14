// The MIT License (MIT)

// Copyright (c) 2013 lailongwei<lailongwei@126.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to 
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
// the Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

__LLBC_NS_BEGIN

inline int LLBC_File::Read(bool &boolVal)
{
    return ReadRawObj<bool>(boolVal);
}

inline int LLBC_File::Read(sint8 &sint8Val)
{
    return ReadRawObj<sint8>(sint8Val);
}

inline int LLBC_File::Read(uint8 &uint8Val)
{
    return ReadRawObj<uint8>(uint8Val);
}

inline int LLBC_File::Read(sint16 &sint16Val)
{
    return ReadRawObj<sint16>(sint16Val);
}

inline int LLBC_File::Read(uint16 &uint16Val)
{
    return ReadRawObj<uint16>(uint16Val);
}

inline int LLBC_File::Read(sint32 &sint32Val)
{
    return ReadRawObj<sint32>(sint32Val);
}

inline int LLBC_File::Read(uint32 &uint32Val)
{
    return ReadRawObj<uint32>(uint32Val);
}

inline int LLBC_File::Read(long &longVal)
{
    return ReadRawObj<long>(longVal);
}

inline int LLBC_File::Read(LLBC_NS ulong &ulongVal)
{
    return ReadRawObj<LLBC_NS ulong>(ulongVal);
}

inline int LLBC_File::Read(sint64 &sint64Val)
{
    return ReadRawObj<sint64>(sint64Val);
}

inline int LLBC_File::Read(uint64 &uint64Val)
{
    return ReadRawObj<uint64>(uint64Val);
}

inline int LLBC_File::Read(float &floatVal)
{
    return ReadRawObj<float>(floatVal);
}

inline int LLBC_File::Read(double &doubleVal)
{
    return ReadRawObj<double>(doubleVal);
}

inline int LLBC_File::Read(ldouble &ldoubleVal)
{
    return ReadRawObj<ldouble>(ldoubleVal);
}

template <typename T>
int LLBC_File::ReadRawObj(T &obj)
{
    return Read((&obj), sizeof(T)) != sizeof(T) ? LLBC_FAILED : LLBC_OK;
}

inline int LLBC_File::Write(const bool &boolVal)
{
    return WriteRawObj<bool>(boolVal);
}

inline int LLBC_File::Write(const sint8 &sint8Val)
{
    return WriteRawObj<sint8>(sint8Val);
}

inline int LLBC_File::Write(const uint8 &uint8Val)
{
    return WriteRawObj<uint8>(uint8Val);
}

inline int LLBC_File::Write(const sint16 &sint16Val)
{
    return WriteRawObj<sint16>(sint16Val);
}

inline int LLBC_File::Write(const uint16 &uint16Val)
{
    return WriteRawObj<uint16>(uint16Val);
}

inline int LLBC_File::Write(const sint32 &sint32Val)
{
    return WriteRawObj<sint32>(sint32Val);
}

inline int LLBC_File::Write(const uint32 &uint32Val)
{
    return WriteRawObj<uint32>(uint32Val);
}

inline int LLBC_File::Write(const sint64 &sint64Val)
{
    return WriteRawObj<sint64>(sint64Val);
}

inline int LLBC_File::Write(const uint64 &uint64Val)
{
    return WriteRawObj<uint64>(uint64Val);
}

inline int LLBC_File::Write(const long &longVal)
{
    return WriteRawObj<long>(longVal);
}

inline int LLBC_File::Write(const LLBC_NS ulong &ulongVal)
{
    return WriteRawObj<LLBC_NS ulong>(ulongVal);
}

inline int LLBC_File::Write(const float &floatVal)
{
    return WriteRawObj<float>(floatVal);
}

inline int LLBC_File::Write(const double &doubleVal)
{
    return WriteRawObj<double>(doubleVal);
}

inline int LLBC_File::Write(const ldouble &ldoubleVal)
{
    return WriteRawObj<ldouble>(ldoubleVal);
}

inline int LLBC_File::Write(const char *cstr)
{
    const auto strSize = cstr ? strlen(cstr) : 0;
    if (strSize == 0)
        return LLBC_OK;

    const sint64 actuallyWrote = Write(cstr, strSize);
    if (actuallyWrote == -1)
    {
        return LLBC_FAILED;
    }
    else if (actuallyWrote != static_cast<sint64>(strSize))
    {
        LLBC_SetLastError(LLBC_ERROR_TRUNCATED);
        return LLBC_FAILED;
    }

    return LLBC_OK;
}

template <typename StrType>
typename std::enable_if<LLBC_IsTemplSpec<StrType, std::basic_string>::value ||
                        LLBC_IsTemplSpec<StrType, LLBC_BasicString>::value ||
                        LLBC_IsTemplSpec<StrType, LLBC_BasicCString>::value, int>::type
LLBC_File::Write(const StrType &str)
{
    const size_t strByteSize = str.size() * sizeof(typename StrType::value_type);
    return Write(str.data(), strByteSize) != static_cast<sint64>(strByteSize) ? LLBC_FAILED : LLBC_OK;
}

template <typename T>
int LLBC_File::WriteRawObj(const T &obj)
{
    sint64 actuallyWrote = Write(&obj, sizeof(T));
    if (actuallyWrote == -1)
    {
        return LLBC_FAILED;
    }
    else if (actuallyWrote != sizeof(T))
    {
        LLBC_SetLastError(LLBC_ERROR_TRUNCATED);
        return LLBC_FAILED;
    }

    return LLBC_OK;
}

__LLBC_NS_END
