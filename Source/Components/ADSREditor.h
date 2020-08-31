/*
  ==============================================================================

    ADSREditor.h
    Created: 31 Aug 2020 1:09:56am
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once


#ifndef ADSR_EDITOR_H_INCLUDED
#define ADSR_EDITOR_H_INCLUDED

#include <JuceHeader.h>

class ADSREditor : public Component
{
public:
	ADSREditor();

	~ADSREditor()
	{
	}

	/** Updates the segment positions from ADSR position information,
	 call this when the host, or loading a new patch, has caused the
	 values to change.  */
	void update();

	struct EnvelopeData
	{
		float attackLevel;
		float attackTime;
		float decay;
		float sustain; /** < 0.0 to 1.0. */
		float release;

		void drawDebugInformation(Graphics& g, Rectangle<float> area) const;
	private:
		void show(Graphics& g, const String& title, float value,
		          Rectangle<float>& area) const;
	};

	/** Access the ADSR structure. Call update() if you change the values. */
	EnvelopeData& getData() { return data; }

	/** Several of these are arranged horizontally to make up the envelope. */
	class Segment
		:
		public Component
	{
	public:
		Segment(ADSREditor* owner, Segment* segmentOnLeft)
			:
			owner(owner),
			right(nullptr),
			allowDurationChange(true)
		{
			if (segmentOnLeft != nullptr)
				segmentOnLeft->right = this;

			left = segmentOnLeft;
		}

		void mouseDown(const MouseEvent&) override;
		void mouseDrag(const MouseEvent&) override;
		void paint(Graphics&) override;

		enum ChainDirection
		{
			kLeft,
			kRight,
			kBoth
		};

		/** Returns the level, i.e. the Y amount, for this shape.
		 Values are normalised from 0.0 to 1.0. */
		float getLeftLevel() const { return leftLevel; }
		float getRightLevel() const { return rightLevel; }

		void setLeftLevel(float newLevel, ChainDirection d = kBoth);
		void setRightLevel(float newLevel, ChainDirection d = kBoth);
		void setDuration(float newDuration) { duration = newDuration; }

		void setFixedDuration(float fixedDuration)
		{
			duration = fixedDuration;
			allowDurationChange = false;
		}

		/**
		 Returns the length of this segment.  Values are
		 normalised from 0.0 to 1.0.
		 */
		float getDuration() const { return duration; }

		void setYAxisControls(bool leftSide, bool rightSide);

	private:


		bool leftRightLinked() const { return controllingLeft && controllingRight; }


		ADSREditor* owner;

		Segment* left;
		Segment* right;

		float leftLevel;
		float rightLevel;
		float duration;

		bool controllingLeft;
		bool controllingRight;
		bool allowDurationChange;

		struct MouseDownInfo
		{
			float left;
			float right;
			float duration;
		} mouseDownData;

		const float mouseSensitivity = 200.0f; /**< Increase to reduce senstivity. */
	};


private:
	void resized() override;
	/** Called by the segments when some values have changed. */
	void updateFromSegments();
	/** Updates the bounds of the segment components. */
	void updateSegmentPositions();

	EnvelopeData data;

	enum SegmentOrder
	{
		kAttack,
		kDecay,
		kSustain,
		kRelease
	};

	OwnedArray<Segment> segments;
};

#endif  // ADSR_EDITOR_H_INCLUDED
