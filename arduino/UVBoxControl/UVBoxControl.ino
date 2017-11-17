#include <TFT.h>  // Arduino LCD library
#include <SPI.h> 

#include "BufferedVal.h"
#include "PushButton.h"
#include "RGB.h"
#include "StringWriter.h"

// SCK - Pin 13, 11-SDA(MOSI) (Used by SPI.h)
const int pin_cs{10};
const int pin_dc{9}; // Also named A0
const int pin_rst{8};

const int pin_time{A0};
const int pin_percent1{A1};
const int pin_percent2{A2};
const int pin_button{7};
const int pin_top_pwm{6};
const int pin_bottom_pwm{5};

const int resolution{1023};     // Resolution of analogRead

const Color black{0, 0, 0};
const Color white{255, 255, 255};
const Color light_blue{0, 0, 128};
const Color red{255, 0, 0};

// Use this to interface to screen
TFT TFTscreen{pin_cs, pin_dc, pin_rst};

// Preset all of the locations and some of the strings that need to be written to
StringWriter bake_time{TFTscreen, white, 2, 5, 7, "Bake Time:\n"};
StringWriter time_writer{TFTscreen, white, 5,  8, 27};
StringWriter top{TFTscreen, white, 2, 5, 78, "Top\n"};
StringWriter top_percentage{TFTscreen, white, 3,  5, 98};
StringWriter bottom{TFTscreen, white, 2, 85, 78, "Bottom\n"};
StringWriter bottom_percentage{TFTscreen, white, 3, 85, 98};

// These hold their previous value so when updating them it knows if they've been changed
BufferedVal<long> seconds{-1};
BufferedVal<int> percent_top{-1};
BufferedVal<int> percent_bottom{-1};

// Creates a button on pin 7 and can only accept input after a delay fromt the last button press
PushButton button{pin_button};

char string_buffer[10];
  
/**
 * Runs begin in the LCD library, draws a black bacground
 */
void setup()
{
    TFTscreen.begin();
    delay(100);
    TFTscreen.background(black.blue, black.green, black.red);
    bake_time.write();
    top.write();
    bottom.write();
}

void loop()
{
    if (button.pressed(2000))
    {
        countdown();
    }
  
    int input1{get_average_in(pin_time)};
    int input2{get_average_in(pin_percent1)};
    int input3{get_average_in(pin_percent2)};

    if (seconds.new_value(input1 * (300.1 / resolution)))
    {
        time_writer.write(time_string(seconds.val()));
    }

    if (percent_top.new_value(input2 * (100.0 / resolution)))
    {
        top_percentage.write(percentage_string(percent_top.val()));
    }
    
    if (percent_bottom.new_value(input3 * (100.0  / resolution)))
    {
        bottom_percentage.write(percentage_string(percent_bottom.val()));
    }
    delay(250);
}

char* time_string(int total)
{
  int minutes{total / 60};
  int seconds{total % 60};

  sprintf(string_buffer, "%02d:%02d", minutes, seconds);
  return string_buffer;
}

char* percentage_string(int total)
{
  sprintf(string_buffer, "%02d%%", total);
  return string_buffer;
}

int get_average_in(int pin)
{
    const int num_reads{30};
    int total{0};
    for (int i=0; i<num_reads; i++)
    {
        total += analogRead(pin);
    }
    return total / num_reads;
}
void countdown()
{
    TFTscreen.background(light_blue.blue, light_blue.green, light_blue.red);
    write_all();
    analogWrite(pin_top_pwm, static_cast<int>(255 * .01 * percent_top.val()));
    analogWrite(pin_bottom_pwm, static_cast<int>(255 * .01 * percent_bottom.val()));
    unsigned long countdown_length{static_cast<unsigned long>(seconds.val() * 1000)};
    unsigned long start_time{millis()};
    unsigned long ellapsed{0};
    BufferedVal<unsigned long> time_left {static_cast<unsigned long>(seconds.val())};
    seconds = 0;
    while (true)
    {
        ellapsed = millis() - start_time;
        long difference {static_cast<long>(countdown_length-ellapsed)};
        if (difference <= 0)
        {
            break;
        }
        if (button.pressed(1000))
        {
            TFTscreen.background(black.blue, black.green, black.red);
            digitalWrite(pin_top_pwm, 0);
            digitalWrite(pin_bottom_pwm, 0);
            write_all();
            return;
        }
        if (time_left.new_value(ceil(static_cast<double>(difference) / 1000.0)))
        {
            time_writer.write(time_string(time_left.val()), light_blue);
        }
        delay(250); 
    }
    digitalWrite(pin_top_pwm, 0);
    digitalWrite(pin_bottom_pwm, 0);
    time_writer.write(time_string(0), light_blue);
    flash_screen();
}

void flash_screen()
{
    bool draw_red{true};
    while (true)
    {
        draw_red ? TFTscreen.background(red.green, red.blue, red.red) : TFTscreen.background(black.blue, black.green, black.red);
        draw_red = not draw_red;
        write_all();
        if (button.pressed())
        {
            if (draw_red == false)
            {
                TFTscreen.background(black.blue, black.green, black.red);
                write_all();
            }
            break;
        }
        delay(250); 
    }
}

void write_all()
{
        bake_time.write();
        time_writer.write();
        top.write();
        top_percentage.write();
        bottom.write();
        bottom_percentage.write();
}

