#pragma once
#include <JuceHeader.h>

class AudioFilePlayer : public juce::Component,
                        public juce::FileDragAndDropTarget,
                        private juce::Timer
{
public:
    AudioFilePlayer();
    ~AudioFilePlayer() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    // Drag and drop
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    // Audio processing
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void getNextAudioBlock(juce::AudioBuffer<float>& buffer);
    void releaseResources();
    
    bool isPlaying() const { return playing; }
    
private:
    void timerCallback() override;
    void loadFile(const juce::File& file);
    void playPause();
    void stop();
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label fileNameLabel;
    juce::Label statusLabel;
    
    bool playing = false;
    juce::String currentFileName;
    double currentPosition = 0.0;
    double totalLength = 0.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilePlayer)
};
