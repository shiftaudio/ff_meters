/*
 ==============================================================================
 Copyright (c) 2017 Filmstro Ltd. / Foleys Finest Audio UG - Daniel Walz
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors
    may be used to endorse or promote products derived from this software without
    specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================

    ff_meters_LevelMeter.h
    Created: 5 Apr 2016 9:49:54am
    Author:  Daniel Walz

 ==============================================================================
*/

#ifndef LEVELMETER_H_INCLUDED
#define LEVELMETER_H_INCLUDED


//==============================================================================
/*
 This class is used to display a level reading. It supports max and RMS levels.
 You can also set a reduction value to display, the definition of that value is up to you.
*/
class LevelMeter    : public juce::Component, private juce::Timer
{
public:

    enum MeterFlags {
        Default         = 0x0000,
        Horizontal      = 0x0001,
        Vintage         = 0x0002,
        SingleChannel   = 0x0004,
        HasBorder       = 0x0008,
        Reduction       = 0x0010,
        Minimal         = 0x0020
    };

    enum ColourIds {
        lmTextColour = 0x2200001,
        lmTextDeactiveColour,
        lmTextClipColour,
        lmTicksColour,
        lmOutlineColour,
        lmBackgroundColour,
        lmBackgroundClipColour,
        lmMeterForegroundColour,
        lmMeterOutlineColour,
        lmMeterBackgroundColour,
        lmMeterMaxNormalColour,
        lmMeterMaxWarnColour,
        lmMeterMaxOverColour,
        lmMeterGradientLowColour,
        lmMeterGradientMidColour,
        lmMeterGradientMaxColour,
        lmMeterReductionColour
    };

    class LookAndFeelMethods {
    public:
        virtual ~LookAndFeelMethods() {}

        /** Define your default colours in this callback */
        virtual void setupDefaultMeterColours () = 0;

        /** Override this to change the inner rectangle in case you want to paint a border e.g. */
        virtual juce::Rectangle<float> getMeterInnerBounds (const juce::Rectangle<float> bounds,
                                                            const MeterFlags meterType) const = 0;

        /** Override this callback to define the placement of a meter channel. */
        virtual juce::Rectangle<float> getMeterBounds (const juce::Rectangle<float> bounds,
                                                       const MeterFlags meterType,
                                                       const int numChannels,
                                                       const int channel) const = 0;

        /** Override this callback to define the placement of the actual meter bar. */
        virtual juce::Rectangle<float> getMeterBarBounds (const juce::Rectangle<float> bounds,
                                                          const MeterFlags meterType) const = 0;

        /** Override this callback to define the placement of the tickmarks.
         To disable this feature return an empty rectangle. */
        virtual juce::Rectangle<float> getMeterTickmarksBounds (const juce::Rectangle<float> bounds,
                                                                const MeterFlags meterType) const = 0;

        /** Override this callback to define the placement of the clip indicator light.
         To disable this feature return an empty rectangle. */
        virtual juce::Rectangle<float> getMeterClipIndicatorBounds (const juce::Rectangle<float> bounds,
                                                                    const MeterFlags meterType) const = 0;


        /** Override this to draw background and if wanted a frame. If the frame takes space away, 
         it should return the reduced bounds */
        virtual juce::Rectangle<float> drawBackground (juce::Graphics&,
                                                       const MeterFlags meterType,
                                                       const juce::Rectangle<float> bounds) = 0;

        /** This is called to draw the actual numbers and bars on top of the static background */
        virtual void drawMeterBars (juce::Graphics&,
                                    const MeterFlags meterType,
                                    const juce::Rectangle<float> bounds,
                                    const LevelMeterSource* source,
                                    const int selectedChannel=-1) = 0;

        /** This draws the static background of the whole level meter group with all channels */
        virtual void drawMeterBarsBackground (juce::Graphics&,
                                              const MeterFlags meterType,
                                              const juce::Rectangle<float> bounds,
                                              const int numChannels) = 0;

        /** This draws a group of informations representing one channel */
        virtual void drawMeterChannel (juce::Graphics&,
                                       const MeterFlags meterType,
                                       const juce::Rectangle<float> bounds,
                                       const LevelMeterSource* source,
                                       const int selectedChannel) = 0;

        /** This draws the static backgrounds representing one channel */
        virtual void drawMeterChannelBackground (juce::Graphics&,
                                                 const MeterFlags meterType,
                                                 const juce::Rectangle<float> bounds) = 0;

        /** This callback draws the actual level bar. The background has an extra callback */
        virtual void drawMeterBar (juce::Graphics&,
                                   const MeterFlags meterType,
                                   const juce::Rectangle<float> bounds,
                                   const float rms, const float peak) = 0;

        /** This draws the background for the actual level bar */
        virtual void drawMeterBarBackground (juce::Graphics&,
                                             const MeterFlags meterType,
                                             const juce::Rectangle<float> bounds) = 0;

        /** This draws the tickmarks for the level scale. It is painted on the static background */
        virtual void drawTickMarks (juce::Graphics&,
                                    const MeterFlags meterType,
                                    const juce::Rectangle<float> bounds) = 0;

        /** This callback draws the clip indicator. The background has an extra callback */
        virtual void drawClipIndicator (juce::Graphics&,
                                        const MeterFlags meterType,
                                        const juce::Rectangle<float> bounds,
                                        const bool hasClipped) = 0;

        /** This draws the background for the clip indicator LED */
        virtual void drawClipIndicatorBackground (juce::Graphics&,
                                                  const MeterFlags meterType,
                                                  const juce::Rectangle<float> bounds) = 0;

        /** Override this callback to define the placement of the max level.
         To disable this feature return an empty rectangle. */
        virtual juce::Rectangle<float> getMeterMaxNumberBounds (const juce::Rectangle<float> bounds,
                                                                const MeterFlags meterType) const = 0;

        /** This callback draws the number of maximum level. The background has an extra callback */
        virtual void drawMaxNumber (juce::Graphics&,
                                    const MeterFlags meterType,
                                    const juce::Rectangle<float> bounds,
                                    const float maxGain) = 0;

        /** This draws the background for the maximum level display */
        virtual void drawMaxNumberBackground (juce::Graphics&,
                                              const MeterFlags meterType,
                                              const juce::Rectangle<float> bounds) = 0;

        /** This is called by the frontend to check, if the clip indicator was clicked (e.g. for reset) */
        virtual int hitTestClipIndicator (const juce::Point<int> position,
                                          const MeterFlags meterType,
                                          const juce::Rectangle<float> bounds,
                                          const LevelMeterSource* source) const = 0;

        /** This is called by the frontend to check, if the maximum level number was clicked (e.g. for reset) */
        virtual int hitTestMaxNumber (const juce::Point<int> position,
                                      const MeterFlags meterType,
                                      const juce::Rectangle<float> bounds,
                                      const LevelMeterSource* source) const = 0;
    };

    LevelMeter (const MeterFlags type = HasBorder);
    ~LevelMeter ();

    void paint (juce::Graphics&) override;

    void resized () override;

    void timerCallback () override;

    void setMeterSource (LevelMeterSource* source);

    void setSelectedChannel (const int c);

    void setRefreshRateHz (const int newRefreshRate);

    void clearClipIndicator (const int channel=-1);

    void clearMaxLevelDisplay (const int channel=-1);

    void mouseDown (const juce::MouseEvent& event) override;

    class Listener {
    public:
        virtual ~Listener () {}
        virtual void clipLightClicked (const int channel) = 0;
        virtual void maxLevelClicked (const int channel)  = 0;
    };

    void addListener (Listener*);

    void removeListener (Listener*);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
    
    juce::WeakReference<LevelMeterSource> source;

    int                                   selectedChannel;

    MeterFlags                            meterType;

    int                                   refreshRate;

    bool                                  useBackgroundImage;

    juce::Image                           backgroundImage;

    bool                                  backgroundNeedsRepaint;

    juce::ListenerList<LevelMeter::Listener> listeners;
};

inline LevelMeter::MeterFlags operator|(LevelMeter::MeterFlags a, LevelMeter::MeterFlags b)
{return static_cast<LevelMeter::MeterFlags>(static_cast<int>(a) | static_cast<int>(b));}

#endif  // LEVELMETER_H_INCLUDED
