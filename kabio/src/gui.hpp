/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "jack.hpp"
#include "notedetection.hpp"
#include "midi.hpp"
#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QMutex>


namespace kabio
{
    class GUI : public QWidget
    {
        Q_OBJECT
        unsigned int buffer_size;
        NoteDetection *note_detection;
        Jack *io;
        void audio_process(float *input, unsigned int nsamples, MIDI *out);
        QProgressBar *pitch_view;
        QLabel *note_label;
        void current_note_changed_slot(NoteDetection::Result);
        void set_buffer_size(unsigned int);
        QString buffer_size_slider_display(int);
    public:
        GUI();
        ~GUI() override;
    signals:
        void current_note_changed(NoteDetection::Result);
    };
};
