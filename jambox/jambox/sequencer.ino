#define sequencer_mode_normal     0
#define sequencer_mode_func       1
#define sequencer_mode_set        2
#define sequencer_mode_play       3

#define sequencer_func_rand_trk   0
#define sequencer_func_rand_osc   1
#define sequencer_func_rand_all   2
#define sequencer_func_clr_trk    3
#define sequencer_func_clr_osc    4
#define sequencer_func_clr_all    5
#define sequencer_func_cpy_trk    6
#define sequencer_func_cpy_osc    7
#define sequencer_func_paste      8
#define sequencer_func_rnd_arp    10

#define sequencer_set_bpm         0
#define sequencer_set_osc1_end    1
#define sequencer_set_osc1_spd    2
#define sequencer_set_osc1_wav    4
#define sequencer_set_osc2_end    7
#define sequencer_set_osc2_spd    8
#define sequencer_set_osc2_wav    10
#define sequencer_set_arp_typ     13
#define sequencer_set_arp_oct     14
#define sequencer_set_arp_spd     15
#define sequencer_set_chord       16

char sequencer_track_names[8][12] = { "Note", "Note Len", "Osc 1 Phase", "Osc 1 Decay", "Osc 1 EFX", "Osc 2 Phase" ,"Osc 2 Decay", "Osc 2 Efx" };
char sequencer_function_names[10][12] = { "Rand Track", "Rand Osc", "Rand All", "Clear Track", "Clear Osc", "Clear All", "Copy Track", "Copy Osc", "Paste", "Rnd Arp" };
char sequencer_setting_names[16][16] = { "BPM", "Osc1 End Type", "Osc1 Step Len", "Osc1 Wave", "Osc2 End Type", "Osc2 Step Len", "Osc2 Wave", "Arp Type", "Arp Octives", 
                                   "Arp Speed", "Arp Step Len", "Chord" };

char sequencer_set_osc_end_vals[2][8] = { "Loop", "Bounce" };
char sequencer_set_osc_spd_names[6][8] = { "Whole", "Half", "Quarter", "Eigth", "16th", "32nd" };
int sequencer_set_osc_spd_vals[6][8] = { 32, 16, 8, 4, 2, 1 };
char sequencer_set_osc_wav_vals[1][8] = { "Saw" };
char sequencer_set_arp_typ_vals[4][8] = { "Down", "Up", "Random", "Track" };
char sequencer_set_arp_spd_vals[4][8] = { "Down", "Up", "Random", "Track" };
char sequencer_set_chord_vals[1][8] = { "None" };

int seqMode = 0;
int currentTrack = 0;
int currentStep = 0;
int currentFuncion = 0;
int currentSetting = 0;
int currentFunctionVal = 0;
int currentSettingVal = 0;
int index; 
int index2; 
int bpm;
int bpm32ndMSDelay;
bool blinkToggle = false;

int steps[8][32] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// these are 32nd note multiplier. Meaning 1 = 32nd notes, 2 = 16th notes, 3 = 8th notes, 4 = quater notes, etc..
int trackSpeeds[8] = { 4, 4, 4, 4, 4, 4, 4, 4 };
int trackStepLimits[8] = { 32, 32, 32, 32, 32, 32, 32, 32 };
int trackPlayHead[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void sequencer_init() {

    bpm = 120;
    bpm32ndMSDelay = get32ndNoteBPMToMSDelay(bpm);
}

void sequencer_update() {

    switch(seqMode) {

        case sequencer_mode_normal: 
            sequencer_update_normal_mode();
            break;

        case sequencer_mode_func: 
            sequencer_update_function_mode();
            break;

        case sequencer_mode_set: 
            sequencer_update_setting_mode();
            break;

        case sequencer_mode_play:
            sequencer_update_play_mode();
            break;
    }
}

void sequencer_update_normal_mode() {

    int newTrack = (int)(input_pot1() / 127);
    int newStep = (int)(input_pot2() / 32);
    int newVal = input_pot3();

    if (input_button1() == HIGH)
        newTrack --;

    if (input_button2() == HIGH)
        newTrack ++;

    if (input_button3() == HIGH)
        newStep --;

    if (input_button4() == HIGH)
        newStep ++;

    if (input_button6() == HIGH)
        sequencer_change_mode(sequencer_mode_func);

    if (input_button7() == HIGH)
        sequencer_change_mode(sequencer_mode_set);

    if (input_button8() == HIGH)
        sequencer_change_mode(sequencer_mode_play);

    // if they are changing tracks, show which on screen
    if (newTrack != currentTrack) {

        // TODO: show selected track on oled
        currentTrack = newTrack;
    }

    // if they are changing steps, show which on screen
    if (newStep != currentStep) {

        // TODO: show selected step on oled
        currentStep = newStep;
    }

    // if they are changing vals, show which on screen
    if (newVal != steps[currentStep][currentTrack]) {

        // TODO: show new value on oled
        steps[currentStep][currentTrack] = newVal;
    }

    // have they tweaked the function knob?
    if (input_pot4() != currentFunctionVal)
        sequencer_change_mode(sequencer_mode_func);

    // have they tweaked the setting knob?
    if (input_pot5() != currentSettingVal)
        sequencer_change_mode(sequencer_mode_set);

    // mark steps with any values on this track
    for (index = 0; index < 32; index ++)
        if (steps[index][currentTrack] != 0)
            matrix_setPixel(index, currentTrack);

    // mark all other tracks showing which step is selected
    for (index = 0; index < 8; index ++)
        if (index != currentTrack)
            matrix_setPixel(currentStep, index);

    delay(33);
}

void sequencer_update_play_mode() {

    for (index = 0; index < 8; index++) {

        // push playhead 
        trackPlayHead[index] ++;
        int limit = trackStepLimits[index] * trackSpeeds[index];
        int displayStep = (int)(trackPlayHead[index] / trackSpeeds[index]);

        // track looping
        if (limit >= trackSpeeds[index] * trackSpeeds[index])
            trackPlayHead[index] = 0;

        // blink current position
        blinkToggle = !blinkToggle;
        if (blinkToggle)
            matrix_setPixel(displayStep, index);

        // draw the rest of the track
        for (index2 = 0; index2 < 32; index2 ++)
            if (steps[index2][index] != 0 && index2 != displayStep)
                matrix_setPixel(index, currentTrack);

        // TODO: make these the correct values
        // set synth values
        switch(index) {

            case 0: 
                synth_SyncPhaseInc = getActualTrackValue(index, steps[index2][index]);
                break;

            case 1:
                // TODO: implement note length 
                break;

            case 2: 
                synth_GrainPhaseInc = getActualTrackValue(index, steps[index2][index]);
                break;

            case 3: 
                synth_GrainDecay = getActualTrackValue(index, steps[index2][index]);
                break;

            case 4: 
                // TODO: implement osc1 efx
                break;

            case 5: 
                synth_Grain2PhaseInc = getActualTrackValue(index, steps[index2][index]);
                break;

            case 6: 
                synth_Grain2Decay = getActualTrackValue(index, steps[index2][index]);
                break;

            case 7: 
                // TODO: implement osc2 efx
                break;
        }
    }

    delay(bpm32ndMSDelay);
}

void sequencer_update_function_mode() {

    /*
    int newFunction = (int)(input_pot5() / 102.3);
    int newVal = input_pot1();

    if (input_button1() == HIGH)
        newFunction --;

    if (input_button2() == HIGH)
        newFunction ++;

    if (input_button3() == HIGH)
        newVal --;

    if (input_button4() == HIGH)
        newVal ++;

    if (input_button5() == HIGH) {

        sequencer_activate_function(currentFuncion);
        sequencer_change_mode(sequencer_mode_normal);
    }

    if (input_button6() == HIGH)
        sequencer_change_mode(sequencer_mode_normal);

    if (input_button7() == HIGH)
        sequencer_change_mode(sequencer_mode_set);

    if (input_button8() == HIGH)
        sequencer_change_mode(sequencer_mode_play);

    // if they are changing tracks, show which on screen
    if (newTrack != currentTrack) {

        // TODO: show selected track on oled
        currentTrack = newTrack;
    }

    // if they are changing steps, show which on screen
    if (newStep != currentStep) {

        // TODO: show selected step on oled
        currentStep = newStep;
    }

    // if they are changing vals, show which on screen
    if (newVal != steps[currentStep][currentTrack]) {

        // TODO: show new value on oled
        steps[currentStep][currentTrack] = newVal;
    }

    // have they tweaked the function knob?
    if (input_pot4() != currentFunctionVal)
        sequencer_change_mode(sequencer_mode_func);

    // have they tweaked the setting knob?
    if (input_pot5() != currentSettingVal)
        sequencer_change_mode(sequencer_mode_set);

    // TODO: show steps with any values as dots on selected track
    // TODO: show steps as red on every unselected track
    */
}

void sequencer_activate_function(int which) {

    // TODO: activate selected function
}

void sequencer_update_setting_mode() {

}

void sequencer_activate_setting(int which) {

    // TODO: activate selected function
}

void sequencer_change_mode(int which) {

    switch(which) {

        case sequencer_mode_normal: 
            currentFunctionVal = input_pot4();
            currentSettingVal = input_pot5();
            sequencer_update_normal_mode();
            break;

        case sequencer_mode_func: 
            sequencer_update_function_mode();
            break;

        case sequencer_mode_set: 
            sequencer_update_setting_mode();
            break;

        case sequencer_mode_play:
            sequencer_update_play_mode();
            break;

        default:
            return;
            break;
    }

    seqMode = which;
}

int getActualSettingValue(int which, int rawValue) {

    switch(which) {

        case sequencer_set_bpm:
            // bpm 60 - 200
            return 60 + (int)(rawValue / 7.307142857142857);
            break;

        case sequencer_set_osc1_end:
            // end type ( loop / bounce )
            return (int)(rawValue / 511.5);
            break;

        case sequencer_set_osc1_spd:
            // step speed ( whole, half, quarter, eigth, sixteenth, 32nd )
            return (int)(rawValue / 170.5);
            break;

        case sequencer_set_osc1_wav:
            // TODO: add waveforms
            return 0;
            break;
        
        case sequencer_set_osc2_end:
            // end type ( loop / bounce )
            return (int)(rawValue / 511.5);
            break;

        case sequencer_set_osc2_spd:
            // step speed ( whole, half, quarter, eigth, sixteenth, 32nd )
            return (int)(rawValue / 170.5);
            break;

        case sequencer_set_osc2_wav:
            // TODO: add waveforms
            return 0;
            break;

        case sequencer_set_arp_typ:
            // arpegio type ( down, up, random, track )
            return (int)(rawValue / 255.75);
            break;

        case sequencer_set_arp_oct:
            // arpegio octaves ( 1 - 6 )
            return (int)(rawValue / 170.5);
            break;

        case sequencer_set_arp_spd:
            // arpegio speed ( whole, half, quarter, eigth, sixteenth, 32nd )
            return (int)(rawValue / 170.5);
            break;

        case sequencer_set_chord:
            // TODO: add chord note locking
            return 0;
            break;
    }
}

int getActualTrackValue(int track, int step) {

    int rawValue = steps[step][track];

    switch(track) {

        case 0:
            // Note 1-12 * 6
            return synth_MapMidi(rawValue);
            break;

        case 1:
            // Note Length
            // TODO: implement note length
            return 0;
            break;

        case 2:
            // osc 1 phase
            return mapPhaseInc(rawValue) / 2;
            break;

        case 3:
            // osc 1 decay
            return rawValue / 8;
            break;

        case 4:
            // osc 1 efx
            // TODO: implement osc 1 efx
            return 0;
            break;

        case 5:
            // osc 2 phase
            return mapPhaseInc(rawValue) / 2;
            break;

        case 6:
            // Osc 2 decay
            return rawValue / 4;
            break;

        case 5:
            // Osc 2 efx
            // TODO: implement osc 2 efx
            return 0;
            break;
    }
}

int get32ndNoteBPMToMSDelay(int bpm) {

    return 60000 / bpm / 8;
}