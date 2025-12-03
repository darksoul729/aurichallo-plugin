#include "AudioFilePlayer.h"

AudioFilePlayer::AudioFilePlayer()
{
    formatManager.registerBasicFormats();
    
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this]() { playPause(); };
    
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this]() { stop(); };
    
    addAndMakeVisible(fileNameLabel);
    fileNameLabel.setText("Drag & Drop Audio File Here", juce::dontSendNotification);
    fileNameLabel.setJustificationType(juce::Justification::centred);
    fileNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xffd4af37));
    
    addAndMakeVisible(statusLabel);
    statusLabel.setText("No file loaded", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setColour(juce::Label::textColourId, juce::Colour(0xffb494ff));
    
    startTimerHz(30);
}

AudioFilePlayer::~AudioFilePlayer()
{
    stopTimer();
    stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
}

void AudioFilePlayer::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Background
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds.toFloat(), 8.0f);
    
    // Border
    g.setColour(juce::Colour(0xffd4af37));
    g.drawRoundedRectangle(bounds.toFloat().reduced(1), 8.0f, 2.0f);
    
    // Progress bar
    if (totalLength > 0.0 && bounds.getHeight() > 8)
    {
        float progress = juce::jlimit(0.0f, 1.0f, static_cast<float>(currentPosition / totalLength));
        auto progressBounds = bounds.removeFromBottom(8).reduced(4, 2).toFloat();
        
        if (progressBounds.getWidth() > 0 && progressBounds.getHeight() > 0)
        {
            g.setColour(juce::Colour(0xff2a2a2a));
            g.fillRoundedRectangle(progressBounds, 2.0f);
            
            auto filledBounds = progressBounds.removeFromLeft(progressBounds.getWidth() * progress);
            if (filledBounds.getWidth() > 0)
            {
                juce::ColourGradient gradient(
                    juce::Colour(0xffb494ff), filledBounds.getX(), filledBounds.getY(),
                    juce::Colour(0xffffa366), filledBounds.getRight(), filledBounds.getY(), false);
                g.setGradientFill(gradient);
                g.fillRoundedRectangle(filledBounds, 2.0f);
            }
        }
    }
}

void AudioFilePlayer::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    if (bounds.getWidth() <= 0 || bounds.getHeight() <= 0)
        return;
    
    // Labels at top
    fileNameLabel.setBounds(bounds.removeFromTop(25));
    statusLabel.setBounds(bounds.removeFromTop(20));
    
    bounds.removeFromTop(5);
    
    // Buttons at bottom
    auto buttonArea = bounds.removeFromBottom(30);
    int halfWidth = buttonArea.getWidth() / 2;
    if (halfWidth > 0)
    {
        playButton.setBounds(buttonArea.removeFromLeft(halfWidth).reduced(5, 0));
        stopButton.setBounds(buttonArea.reduced(5, 0));
    }
}

bool AudioFilePlayer::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto& file : files)
    {
        if (file.endsWithIgnoreCase(".wav") || 
            file.endsWithIgnoreCase(".mp3") ||
            file.endsWithIgnoreCase(".aif") ||
            file.endsWithIgnoreCase(".aiff") ||
            file.endsWithIgnoreCase(".flac"))
            return true;
    }
    return false;
}

void AudioFilePlayer::filesDropped(const juce::StringArray& files, int, int)
{
    if (files.size() > 0)
    {
        juce::File file(files[0]);
        loadFile(file);
    }
}

void AudioFilePlayer::loadFile(const juce::File& file)
{
    stop();
    
    // Release old source first
    transportSource.setSource(nullptr);
    readerSource.reset();
    
    auto* reader = formatManager.createReaderFor(file);
    
    if (reader != nullptr)
    {
        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        
        currentFileName = file.getFileNameWithoutExtension();
        
        // Safe division - check for zero
        if (reader->sampleRate > 0)
            totalLength = reader->lengthInSamples / reader->sampleRate;
        else
            totalLength = 0.0;
        
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        
        fileNameLabel.setText(currentFileName, juce::dontSendNotification);
        statusLabel.setText("Ready to play", juce::dontSendNotification);
        
        repaint();
    }
    else
    {
        statusLabel.setText("Failed to load file", juce::dontSendNotification);
    }
}

void AudioFilePlayer::playPause()
{
    if (readerSource == nullptr)
    {
        statusLabel.setText("No file loaded", juce::dontSendNotification);
        return;
    }
    
    if (playing)
    {
        playing = false;
        transportSource.stop();
        playButton.setButtonText("Play");
        statusLabel.setText("Paused", juce::dontSendNotification);
    }
    else
    {
        playing = true;
        transportSource.start();
        playButton.setButtonText("Pause");
        statusLabel.setText("Playing...", juce::dontSendNotification);
    }
}

void AudioFilePlayer::stop()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
    playing = false;
    playButton.setButtonText("Play");
    currentPosition = 0.0;
    
    if (readerSource != nullptr)
        statusLabel.setText("Stopped", juce::dontSendNotification);
    
    repaint();
}

void AudioFilePlayer::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    if (sampleRate > 0 && samplesPerBlock > 0)
        transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void AudioFilePlayer::getNextAudioBlock(juce::AudioBuffer<float>& buffer)
{
    if (readerSource == nullptr || !playing || buffer.getNumSamples() <= 0)
    {
        buffer.clear();
        return;
    }
    
    try
    {
        // Create temporary buffer to avoid issues
        juce::AudioBuffer<float> tempBuffer(buffer.getNumChannels(), buffer.getNumSamples());
        tempBuffer.clear();
        
        juce::AudioSourceChannelInfo info;
        info.buffer = &tempBuffer;
        info.startSample = 0;
        info.numSamples = tempBuffer.getNumSamples();
        
        transportSource.getNextAudioBlock(info);
        
        // Copy to output buffer
        for (int ch = 0; ch < juce::jmin(buffer.getNumChannels(), tempBuffer.getNumChannels()); ++ch)
        {
            buffer.copyFrom(ch, 0, tempBuffer, ch, 0, buffer.getNumSamples());
        }
        
        currentPosition = transportSource.getCurrentPosition();
    }
    catch (...)
    {
        buffer.clear();
        playing = false;
    }
}

void AudioFilePlayer::releaseResources()
{
    transportSource.releaseResources();
}

void AudioFilePlayer::timerCallback()
{
    if (playing && readerSource != nullptr)
    {
        currentPosition = transportSource.getCurrentPosition();
        repaint();
        
        // Auto stop at end
        if (totalLength > 0.0 && currentPosition >= totalLength - 0.1)
        {
            stop();
        }
    }
}
