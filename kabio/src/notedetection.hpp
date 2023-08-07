/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <aubio/aubio.h>
#include "ringbuffer/ringbuffer.hpp"
#include <QObject>
#include <QMutex>

namespace kabio
{
    class NoteDetection
    {
    public:
        struct Result
        {
            unsigned char note;
            float pitch;
            unsigned char velocity;
        };
        
        NoteDetection(const unsigned int buf_size, const unsigned int hop_factor,
                      const unsigned int sample_rate, unsigned int note_median_size = 1);
        ~NoteDetection();
        Result analyze(const float *buffer, unsigned int nsamples);
        bool pitch_mode;
        
        float silence_treshold();
        void set_silence_treshold(float);
        void set_median_size(unsigned int);
        void set_buffer_size(unsigned int);
    private:
        unsigned int buffer_size;
        unsigned int sample_rate;
        unsigned int hop_factor;
        ringbuffer::RingBuffer<Result> note_buffer;
        QMutex note_buffer_mutex;
        aubio_pitch_t *pitch_detector;
        aubio_onset_t *onset_detector;
        aubio_filter_t *filter;
        
        static bool compare_results(const Result a, const Result b);
    };
};

Q_DECLARE_METATYPE(kabio::NoteDetection::Result)
