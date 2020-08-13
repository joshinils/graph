#pragma once

#include <climits> // UINT_MAX
#include <cstddef> // size_t

// size_t but named differently
struct Index
{
    // ungültiger Sonderwert für Indizes
    const static Index KEIN_INDEX;

    size_t idx;
    Index(size_t i = 0)
        : idx(i)
    { }
    Index(int i = 0)
        : idx(i)
    { }

    Index& operator=(const Index& other)
    {
        this->idx = other.idx;
        return *this;
    }

    Index& operator=(const size_t& other)
    {
        this->idx = other;
        return *this;
    }

    operator size_t() const { return idx; }

    Index& operator++()
    {
        idx++;
        return *this;
    }

    Index operator++(int)
    {
        Index tmp(*this);
        operator++();
        return tmp;
    }

    Index& operator+=(const Index& rhs)
    {
        this->idx += rhs.idx;
        return *this;
    }

    friend Index operator+(Index lhs, const Index& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    bool operator==(Index const& other) const { return this->idx == other.idx; }
    bool operator!=(Index const& other) const { return this->idx != other.idx; }
};


struct knotenIndex : public Index
{ };
struct kantenIndex : public Index
{ };
