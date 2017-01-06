/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2016 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/


/**
    A simple Program to set the colours of individual LEDs.
*/
struct BitmapLEDProgram   : public LEDGrid::Program
{
    BitmapLEDProgram (LEDGrid&);

    /** Set the colour of the LED at coordinates {x, y}. */
    void setLED (uint32 x, uint32 y, LEDColour);

private:
    juce::String getLittleFootProgram() override;
    uint32 getHeapSize() override;
};
