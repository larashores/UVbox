#include "RGB.h"

class TFT;

class StringWriter
{
public:
    StringWriter(TFT& screen, const Color& color, int size, int x, int y)
        : screen{screen}, size{size}, color{color}, x{x}, y{y} {}
    
    
    StringWriter(TFT& screen, const Color& color, int size, int x, int y, const char* str) 
        : screen{screen}, size{size}, color{color}, x{x}, y{y}
    {
        strcpy(last_string, str);
    }
    
    void write(const char* to_write, const Color& back={0, 0, 0})
    {
        screen.setTextSize(size);
        screen.stroke(back.blue, back.green, back.red);
        screen.text(last_string, x, y);
        
        strcpy(last_string, to_write);
        screen.stroke(color.blue, color.green, color.red);
        screen.text(last_string, x, y);
    }
    
    void write()
    {
        screen.setTextSize(size);
        screen.stroke(color.red, color.green, color.blue);
        screen.text(last_string, x, y);
    }
    
private:
    TFT& screen;
    const int size;
    const Color color;
    const int x;
    const int y;
    char last_string[15] {};
};
