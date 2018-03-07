#define seq_mode_normal     0
#define seq_mode_func       1
#define seq_mode_set        2
#define seq_mode_play       3

#define seq_func_rand_trk   0
#define seq_func_rand_osc   1
#define seq_func_rand_all   2
#define seq_func_clr_trk    3
#define seq_func_clr_osc    4
#define seq_func_clr_all    5
#define seq_func_cpy_trk    6
#define seq_func_cpy_osc    7
#define seq_func_paste      8
#define seq_func_rnd_arp    10

#define seq_set_bpm         0
#define seq_set_osc1_end    1
#define seq_set_osc1_spd    2
#define seq_set_osc1_vol    3
#define seq_set_osc1_wav    4
#define seq_set_osc1_oct    5
#define seq_set_osc1_sem    6
#define seq_set_osc2_end    7
#define seq_set_osc2_spd    8
#define seq_set_osc2_vol    9
#define seq_set_osc1_wav    10
#define seq_set_osc1_oct    11
#define seq_set_osc1_sem    12
#define seq_set_arp_typ     13
#define seq_set_arp_oct     14
#define seq_set_arp_spd     15
#define seq_set_chord       16

char seq_track_names[8, 12] = { "Osc 1 Not", "Osc 1 Oct", "Osc 1 Len", "Osc 1 Efx", "Osc 1 Not", "Osc 1 Oct" ,"Osc 1 Len", "Osc 1 Efx" };
char seq_function_names[10, 12] = { "Rand Track", "Rand Osc", "Rand All", "Clear Track", "Clear Osc", "Clear All", "Copy Track", "Copy Osc", "Paste", "Rnd Arp" };
char seq_setting_names[16, 12] = { 
    "BPM", "Osc1 End Type", "Osc1 Step Len", "Osc1 Volume", "Osc1 Wave", "Osc1 Semi", "Osc2 End Type", "Osc2 Step Len", 
    "Osc2 Volume", "Osc2 Wave", "Osc2 Semi", "Arp Type", "Arp Octives", "Arp Speed", "Arp Step Len", "Chord" };

char seq_set_osc_end_vals[2][8] = { "Loop", "Bounce" };
char seq_set_osc_spd_vals[6][8] = { "Whole", "Half", "Quarter", "Eigth", "16th", "32nd" };
char seq_set_osc_wav_vals[1][8] = { "Saw" };
char seq_set_arp_typ_vals[4][8] = { "Down", "Up", "Random", "Track" };
char seq_set_arp_spd_vals[4][8] = { "Down", "Up", "Random", "Track" };
char seq_set_chord_vals[1][8] = { "None" };

int seqMode = 0;
int currentTrack = 0;
int currentStep = 0;
int currentFuncion = 0;
int currentSetting = 0;
int currentFunctionVal = 0;
int currentSettingVal = 0;

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

void seq_init() {


}

void seq_update() {

    switch(seqMode) {

        case seq_mode_normal: 
            seq_update_normal_mode();
            break;

        case seq_mode_func: 
            seq_update_function_mode();
            break;

        case seq_mode_set: 
            seq_update_setting_mode();
            break;

        case seq_mode_play:
            seq_update_play_mode();
            break;
    }
}

void seq_update_normal_mode() {

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
        seq_change_mode(seq_mode_func);

    if (input_button7() == HIGH)
        seq_change_mode(seq_mode_set);

    if (input_button8() == HIGH)
        seq_change_mode(seq_mode_play);

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
    if (newVal != steps[currentStep, currentTrack]) {

        // TODO: show new value on oled
        steps[currentStep, currentTrack] = newVal;
    }

    // have they tweaked the function knob?
    if (input_pot4() != currentFunctionVal)
        seq_change_mode(seq_mode_func);

    // have they tweaked the setting knob?
    if (input_pot5() != currentSettingVal)
        seq_change_mode(seq_mode_set);

    // TODO: show steps with any values as dots on selected track
    // TODO: show steps as red on every unselected track
}

void seq_update_play_mode() {

}

void seq_update_function_mode() {

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

        seq_activate_function(currentFuncion);
        seq_change_mode(seq_mode_normal);
    }

    if (input_button6() == HIGH)
        seq_change_mode(seq_mode_normal);

    if (input_button7() == HIGH)
        seq_change_mode(seq_mode_set);

    if (input_button8() == HIGH)
        seq_change_mode(seq_mode_play);

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
    if (newVal != steps[currentStep, currentTrack]) {

        // TODO: show new value on oled
        steps[currentStep, currentTrack] = newVal;
    }

    // have they tweaked the function knob?
    if (input_pot4() != currentFunctionVal)
        seq_change_mode(seq_mode_func);

    // have they tweaked the setting knob?
    if (input_pot5() != currentSettingVal)
        seq_change_mode(seq_mode_set);

    // TODO: show steps with any values as dots on selected track
    // TODO: show steps as red on every unselected track
}

void seq_activate_function(int which) {

    // TODO: activate selected function
}

void seq_update_setting_mode() {

}

void seq_change_mode(int which) {

    switch(which) {

        case seq_mode_normal: 
            currentFunctionVal = input_pot4();
            currentSettingVal = input_pot5();
            seq_update_normal_mode();
            break;

        case seq_mode_func: 
            seq_update_function_mode();
            break;

        case seq_mode_set: 
            seq_update_setting_mode();
            break;

        case seq_mode_play:
            seq_update_play_mode();
            break;

        default:
            return;
            break;
    }

    seqMode = which;
}

int getActualSettingValue(int which, int rawValue) {

    switch(which) {

        case seq_set_bpm:
            // bpm 60 - 200
            return 60 + (int)(rawValue / 7.307142857142857);
            break;

        case seq_set_osc1_end:
            // end type ( loop / bounce )
            return (int)(rawValue / 511.5);
            break;

        case seq_set_osc1_spd:
            // step speed ( whole, half, quarter, eigth, sixteenth, 32nd )
            return (int)(rawValue / 170.5);
            break;

        case seq_set_osc1_vol:
            return rawValue;
            break;

        case seq_set_osc1_wav:
            // TODO: add waveforms
            return 0;
            break;

        case seq_set_osc1_sem:
            // TODO: add semitone adjustment
            return 0;
            break;
        
        case seq_set_osc2_end:
            // end type ( loop / bounce )
            return (int)(rawValue / 511.5);
            break;

        case seq_set_osc2_spd:
            // step speed ( whole, half, quarter, eigth, sixteenth, 32nd )
            return (int)(rawValue / 170.5);
            break;

        case seq_set_osc2_vol:
            return rawValue;
            break;

        case seq_set_osc2_wav:
            // TODO: add waveforms
            return 0;
            break;

        case seq_set_osc2_sem:
            // TODO: add semitone adjustment
            return 0;
            break;

        case seq_set_arp_typ:
            // arpegio type ( down, up, random, track )
            return (int)(rawValue / 255.75);
            break;

        case seq_set_arp_oct:
            // arpegio octaves ( 1 - 6 )
            return (int)(rawValue / 170.5);
            break;

        case seq_set_arp_spd:
            // arpegio speed ( whole, half, quarter, eigth, sixteenth, 32nd )
            return (int)(rawValue / 170.5);
            break;

        case seq_set_chord:
            // TODO: add chord note locking
            return 0;
            break;
    }
}

int getActualTrackValue(int track, int step) {

    int rawValue = steps[step, track];

    switch(track) {

        case 0:
            // Osc 1 note 1-12
            return (int)(rawValue / 85.25);
            break;

        case 1:
            // Osc 1 Octave 1 - 6
            return (int)(rawValue / 170.5);
            break;

        case 2:
            // Osc 1 len 1 - 10 ( 10% / 20% / 30% / 40% / 50% / 60% / 70% / 80% / 90% / 100% )
            return (int)(rawValue / 102.3);
            break;

        case 3:
            // Osc 1 effect 1 - 8 ( arp up, arp down, stutter, bend next, 25% random end, 50% random end, 75% random end, end )
            return (int)(rawValue / 127.875);
            break;

        case 4:
            // Osc 2 note 1-12
            return (int)(rawValue / 85.25);
            break;

        case 5:
            // Osc 2 Octave 1 - 6
            return (int)(rawValue / 170.5);
            break;

        case 6:
            // Osc 2 len 1 - 10 ( 10% / 20% / 30% / 40% / 50% / 60% / 70% / 80% / 90% / 100% )
            return (int)(rawValue / 102.3);
            break;

        case 7:
            // Osc 2 effect 1 - 8 ( arp up, arp down, stutter, bend next, 25% random end, 50% random end, 75% random end, end )
            return (int)(rawValue / 127.875);
            break;
    }
}