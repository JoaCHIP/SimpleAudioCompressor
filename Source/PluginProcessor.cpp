#include "PluginProcessor.h"
#include "PluginEditor.h"

SimpleExampleAudioProcessor::SimpleExampleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
}

SimpleExampleAudioProcessor::~SimpleExampleAudioProcessor()
{
	srand(time(0));
}

const juce::String SimpleExampleAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool SimpleExampleAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SimpleExampleAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SimpleExampleAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SimpleExampleAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int SimpleExampleAudioProcessor::getNumPrograms()
{
	return 1;	// Some hosts don't cope very well if you tell them there are 0 programs,
}

int SimpleExampleAudioProcessor::getCurrentProgram()
{
	return 0;
}

void SimpleExampleAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SimpleExampleAudioProcessor::getProgramName(int index)
{
	return {};
}

void SimpleExampleAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void SimpleExampleAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback initialisation that you need.
}

void SimpleExampleAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleExampleAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void SimpleExampleAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output channels that didn't contain input data.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	// A very simple compressor and soft-clipper
	const auto master_gain = 0.5f;
	const auto attack_decay_milliseconds = 30.0f / static_cast<float>(getSampleRate());
	const auto inverted = 1.0f - attack_decay_milliseconds;
	loudness = 0;

	for (int channel = 0; channel < totalNumInputChannels; channel++)
	{
		auto* channel_data = buffer.getWritePointer(channel);
		for (auto i = 0; i < buffer.getNumSamples(); i++)
		{
			_attenuation[channel] = _attenuation[channel] * inverted + std::abs(channel_data[i]) * attack_decay_milliseconds;
			channel_data[i] = std::sin(std::clamp(channel_data[i] * master_gain / (_attenuation[channel] + 0.1f), -1.570796f, 1.570796f)) * 0.95f;
			loudness += _attenuation[channel];
		}
	}

	auto total_sample_count = buffer.getNumSamples() * totalNumInputChannels;
	loudness = std::sqrt(loudness / total_sample_count) * 1.41f;
}

bool SimpleExampleAudioProcessor::hasEditor() const
{
	return true;
}

juce::AudioProcessorEditor* SimpleExampleAudioProcessor::createEditor()
{
	return new SimpleExampleAudioProcessorEditor(*this);
}

void SimpleExampleAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void SimpleExampleAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SimpleExampleAudioProcessor();
}
