/*
  ==============================================================================

    SamplerVoice.h
    Created: 29 Aug 2020 8:21:46pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Components/HeaderComponent.h"

class MySamplerVoice : public SamplerVoice
{
public:
	
	// MySamplerVoice(MySamplerAudioProcessor& p, int sampleRate) : processor(p)
	// {
	// 	SynthesiserVoice::setCurrentPlaybackSampleRate(sampleRate);
	// }

private:

	bool setSampleRate(int sampleRate)
	{
	}
	
	bool canPlaySound(SynthesiserSound*) override
	{
		return  true;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition) override
	{
		noteOn = true;
	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		noteOn = false;
	}

	void pitchWheelMoved(int newPitchWheelValue) override
	{
	}

	void controllerMoved(int controllerNumber, int newControllerValue) override
	{
	}

	void renderNextBlock(AudioBuffer<float> & outputBuffer, int startSample, int numSamples) override
	{
		// const auto samplesThisTime = juce::jmin(outputBuffer.getNumSamples(),
		// 	processor.loadedFileWaveform.getNumSamples() - processor.lastPlaybackPosition);
		//
		// processor.lastPlaybackPosition += samplesThisTime;

	}

	using SynthesiserVoice::renderNextBlock;
	
	// MySamplerAudioProcessor& processor;

	// bool currentPlayHasEnded;
	// bool mIsNotePlayed;
	bool noteOn;

	JUCE_LEAK_DETECTOR(MySamplerVoice);
};
