/*
  ==============================================================================

    ConfirmationDialog.h
    Created: 27 Aug 2020 9:34:11pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

	class ConfirmationDialog : public juce::Component
	{
	public:

		ConfirmationDialog(){}
		
		// void reset(std::function<void()> acceptFunction, std::function<void()> closeFunction) const
		// {
		// 	addAndMakeVisible(closeButton);
		// 	addAndMakeVisible(acceptButton);
		//
		// 	closeButton.onClick = closeFunction;
		// 	acceptButton.onClick = [this, acceptFunction, closeFunction]()
		// 	{
		// 		acceptFunction();
		// 		closeFunction();
		// 	};
		// }

	private:
		TextButton closeButton{ "Close" };
		TextButton acceptButton{ "Continue" };
	};
