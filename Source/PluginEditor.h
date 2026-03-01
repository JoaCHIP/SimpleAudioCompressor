#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <chrono>

class SimpleExampleAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	SimpleExampleAudioProcessorEditor(SimpleExampleAudioProcessor&);
	~SimpleExampleAudioProcessorEditor() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to access the processor object that created it.
	SimpleExampleAudioProcessor& audioProcessor;
	std::chrono::steady_clock::time_point lastRedraw;

	static float rnd()
	{
		return static_cast<float>(rand()) / RAND_MAX;
	}

	// UI components
	juce::VBlankAttachment vBlankAttachment
	{
		this, [&]
		{
			using namespace std::chrono;
			auto now = steady_clock::now();
			auto duration = std::chrono::duration_cast<milliseconds>(now - this->lastRedraw).count();
			if (duration > 12)		// 12 ms is appx 80 fps
			{
				this->lastRedraw = now;
				repaint();
			}
		}
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleExampleAudioProcessorEditor)
};
