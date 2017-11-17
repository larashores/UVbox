#ifndef PushButton_h
#define PushButton_h

class PushButton
{
public:
    constexpr PushButton(int pin) : pin{pin} {}
    
    bool pressed(long unsigned int delay)
    { 
        long unsigned int cur_time {millis()};
        if (digitalRead(pin) && ((cur_time - last_time) > delay))
        {
            last_time = millis();
            return true;
        }
        return false; 
    }
    
    bool pressed()
    { 
        if (digitalRead(pin))
        {
            last_time = millis();
            return true;
        }
        return false;
    }
    
    
private:
    long unsigned int last_time {};
    const int pin;
};

#endif