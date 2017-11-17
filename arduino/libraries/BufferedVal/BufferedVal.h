#ifndef BufferedVal_h
#define BufferedVal_h

template<typename T>
class BufferedVal
{
public:
    constexpr BufferedVal(T val) : last_val{val} {}
    
    BufferedVal& operator=(T new_val)
    {
        last_val = new_val;
        return *this;
    }
    
    bool new_value(T new_val)
    {
        if (new_val != last_val)
        {
            last_val = new_val;
            return true;
        }
        return false;
    }
    
    T val()
    {
        return last_val;
    }
    
private:
    T last_val;
};

#endif