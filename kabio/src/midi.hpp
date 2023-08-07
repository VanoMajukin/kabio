/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <jack/jack.h>
#include "notedetection.hpp"

namespace kabio
{
    class MIDI
    {
        NoteDetection::Result prev_note;
        void *buffer;
        unsigned int midi_offset;
        
        void up(unsigned char);
        void down(unsigned char, unsigned char);
        void pitch(float);
    public:
        MIDI();
        void set_buffer(void *buffer);
        void send_note(NoteDetection::Result note);
    };
};
