#define USE_A2DP

#include <Arduino.h>
#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"
#include "AudioLibs/AudioKit.h"

#define sprint Serial.print
#define sprintln Serial.println

// Not using SD part and it hopefully works without
AudioKitStream kit;
BluetoothA2DPSink bt_sink;

bool high_volume = false;

// Write data to AudioKit in callback 
void read_data_stream(const uint8_t *data, uint32_t length)
{
    kit.write(data, length);
}

void vol_mode(bool, int, void*)
{
    high_volume != high_volume;

    kit.setVolume(high_volume ? 75 : 25);
}
void btn_next(bool, int, void*)
{
    bt_sink.next();
}
void btn_prev(bool, int, void*)
{
    bt_sink.previous();
}


void setup() 
{
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);

    // provide a2dp data
    bt_sink.set_stream_reader(read_data_stream, false);
    bt_sink.start("AudioKit");

    //setup output
    auto cfg = kit.defaultConfig(TX_MODE);
    cfg.sample_rate = bt_sink.sample_rate();

    kit.addAction(PIN_KEY4, vol_mode);
    kit.addAction(PIN_KEY5, btn_prev);
    kit.addAction(PIN_KEY6, btn_next);

    kit.setVolume(25);
    kit.begin(cfg);

    // // Update sample rate
    // auto cfg = kit.defaultConfig();
    // cfg.sample_rate = bt_sink.sample_rate();
    // kit.setAudioInfo(cfg);
    sprintln("A2DP started"); 
}

void loop() 
{
    //feed watchdog
    delay(10);

    kit.processActions();
}