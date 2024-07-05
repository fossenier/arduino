// Speaker pin
const int speakerPin = 10;

// Note frequencies (in Hz) for a simplified version of "Clair de Lune"
const int melody[] = {
    392, 392, 440, 392, 523, 494, // G G A G C B
    392, 392, 440, 392, 587, 523  // G G A G D C
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.
const int noteDurations[] = {
    4, 4, 4, 4, 4, 4, // First part
    4, 4, 4, 4, 4, 4  // Second part
};

void setup()
{
    // Initialize the speaker pin as an output
    pinMode(speakerPin, OUTPUT);
}

void loop()
{
    // Play each note in the melody
    for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++)
    {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(speakerPin, melody[thisNote], noteDuration);

        // Pause for the note duration plus 30% for a short gap
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);

        // Stop the tone playing
        noTone(speakerPin);
    }

    // Pause between repeats of the melody
    delay(2000);
}