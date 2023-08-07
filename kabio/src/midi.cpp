/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#include "midi.hpp"
#include "notedetection.hpp"
#include <cmath>
#include <jack/midiport.h>
#include <stdio.h>
#include <QtCore>

namespace kabio
{
    MIDI::MIDI()
    {
    }
    
    void MIDI::set_buffer(void *buffer)
    {
        this->buffer = buffer;
        jack_midi_clear_buffer(buffer);
        midi_offset = 0;
    }
    void MIDI::send_note(NoteDetection::Result cur_note)
    {
        if (prev_note.velocity > 0 && (cur_note.velocity == 0 || cur_note.note != prev_note.note)) {
            up(prev_note.note);
        }
        if (cur_note.velocity > 0 && (prev_note.velocity == 0 || cur_note.note != prev_note.note))
            down(cur_note.note, cur_note.velocity);
        if (cur_note.velocity > 0 && cur_note.pitch != prev_note.pitch)
        {   
            pitch(cur_note.pitch);
        }
        prev_note = cur_note;
    }
    
    void kabio::MIDI::down(unsigned char note, unsigned char velocity)
    {
        unsigned char *event = jack_midi_event_reserve(buffer, midi_offset, 3);
        event[0] = 0x90;
        event[1] = note & 0x7F;
        event[2] = velocity;
    }
    
    void kabio::MIDI::up(unsigned char note)
    {
        
        unsigned char *event = jack_midi_event_reserve(buffer, midi_offset, 3);
        event[0] = 0x80;
        event[1] = note & 0x7F;
        event[2] = 64;
    }
    
    void kabio::MIDI::pitch(float p)
    {
        unsigned short midi_pitch = p * 0x1000 + 0x2000;
        unsigned char *event = jack_midi_event_reserve(buffer, midi_offset, 3);
        event[0] = 0xE0;
        event[1] = midi_pitch & 0x7F;
        event[2] = (midi_pitch >> 7) & 0x7F;
    }
};
