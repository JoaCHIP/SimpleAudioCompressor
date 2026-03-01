#include "PluginProcessor.h"
#include "PluginEditor.h"

SimpleExampleAudioProcessorEditor::SimpleExampleAudioProcessorEditor(SimpleExampleAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	setSize(500, 300);
	setResizable(true, true);
	setResizeLimits(200, 150, 4000, 3000);
}

SimpleExampleAudioProcessorEditor::~SimpleExampleAudioProcessorEditor()
{
}

void SimpleExampleAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colour(0x00000010));
	g.setColour(juce::Colour(rnd(), 0.75f, 1.0f, std::clamp(audioProcessor.loudness, 0.0f, 1.0f)));
	constexpr auto square_size = 80.0f;
	auto size = getLocalBounds();
	const auto window_size = std::sqrt(static_cast<float>(size.getWidth() * size.getWidth() + size.getHeight() * size.getHeight()));
	const auto margin = window_size * 0.05f;
	size.expand(-margin, -margin);
	g.fillRect(size.getX() + rnd() * (size.getWidth() - square_size), size.getY() + rnd() * (size.getHeight() - square_size), square_size, square_size);

	g.setColour(juce::Colour(0xA0FFFFFF));
	g.setFont(juce::FontOptions(window_size * 0.04f + 3.0f));
	g.drawFittedText("MAKE IT LOUD", getLocalBounds(), juce::Justification::centred, 1);
}

void SimpleExampleAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any subcomponents in your editor.
}
