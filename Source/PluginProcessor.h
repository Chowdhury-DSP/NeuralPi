/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <nlohmann/json.hpp>
#include "RTNeuralLSTM.h"
#include "AmpOSCReceiver.h"
#include "Eq4Band.h"

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define MODEL_ID "model"
#define MODEL_NAME "Model"
#define MASTER_ID "master"
#define MASTER_NAME "Master"
#define BASS_ID "bass"
#define BASS_NAME "Bass"
#define MID_ID "mid"
#define MID_NAME "Mid"
#define TREBLE_ID "treble"
#define TREBLE_NAME "Treble"
#define PRESENCE_ID "presence"
#define PRESENCE_NAME "Presence"

//==============================================================================
/**
*/
class NeuralPiAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    NeuralPiAudioProcessor();
    ~NeuralPiAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    bool compareFunction(juce::File a, juce::File b);
    int getModelIndex(float model_param);
    void loadConfig(File configFile);
    void setupDataDirectories();
    void installTones();

    void set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider);
    
    // Overdrive Pedal
    float convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max);

    // Amp
    /*
    void set_ampDrive(float db_ampCleanDrive);
    void set_ampMaster(float db_ampMaster);
    void set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider);
    */
    float decibelToLinear(float dbValue);

    void addDirectory(const File& file);
    void resetDirectory(const File& file);
    std::vector<File> jsonFiles;
    File currentDirectory = File::getCurrentWorkingDirectory().getFullPathName();
    File userAppDataDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(JucePlugin_Manufacturer).getChildFile(JucePlugin_Name);
    File userAppDataDirectory_tones = userAppDataDirectory.getFullPathName() + "/tones";

    // Pedal/amp states
    int amp_state = 1; // 0 = off, 1 = on
    int custom_tone = 0; // 0 = custom tone loaded, 1 = default channel tone
    File loaded_tone;
    juce::String loaded_tone_name;
    const char* char_filename = "";
    int model_loaded = 0;
    int current_model_index = 0;
    float num_models = 0.0;
    int model_index = 0; // Used in processBlock when converting slider param to model index

    RT_LSTM LSTM;

private:
    var dummyVar;
    Eq4Band eq4band; // Amp EQ

    AudioParameterFloat* gainParam;
    AudioParameterFloat* masterParam;
    AudioParameterFloat* bassParam;
    AudioParameterFloat* midParam;
    AudioParameterFloat* trebleParam;
    AudioParameterFloat* presenceParam;
    AudioParameterFloat* modelParam;

    dsp::IIR::Filter<float> dcBlocker;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralPiAudioProcessor)
};
