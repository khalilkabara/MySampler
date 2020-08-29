/*
  ==============================================================================

    SamplerSound.h
    Created: 29 Aug 2020 8:21:25pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MySamplerSound : public SamplerSound
{
public:


	MySamplerSound(const String& name, AudioFormatReader& source, const BigInteger& midiNotes,
		int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds)
		: SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, 
			attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds)
	{
		
	}

	~MySamplerSound() override{}

	bool appliesToNote(int /*midiNoteNumber*/) override
	{
		return true;
	}

	bool appliesToChannel(int /*midiChannel*/) override
	{
		return true;
	}
	
};
