/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#include "notedetection.hpp"
#include <aubio/aubio.h>
#include <algorithm>
#include <cmath>
#include <QMutex>

namespace kabio
{
    NoteDetection::NoteDetection(const unsigned int buf_size, const unsigned int hop_factor,
                                 const unsigned int sample_rate, unsigned int note_median_size)
        : sample_rate(sample_rate), hop_factor(hop_factor), note_buffer(note_median_size)
    {
        buffer_size = buf_size;
        pitch_mode = false;
        pitch_detector = new_aubio_pitch("default", buf_size * hop_factor, buf_size, sample_rate);
        onset_detector = new_aubio_onset("default", buf_size * hop_factor, buf_size, sample_rate);
        filter = new_aubio_filter_a_weighting(sample_rate);
    }
    
    NoteDetection::~NoteDetection()
    {
        del_aubio_pitch(pitch_detector);
        del_aubio_onset(onset_detector);
        del_aubio_filter(filter);
    }

    void NoteDetection::set_median_size(unsigned int capacity)
    {
        note_buffer_mutex.lock();
        note_buffer.set_capacity(capacity);
        note_buffer_mutex.unlock();
    }
    
    void NoteDetection::set_buffer_size(unsigned int buf_size)
    {
        buffer_size = buf_size;
        float silence = silence_treshold();
        del_aubio_pitch(pitch_detector);
        del_aubio_onset(onset_detector);
        pitch_detector = new_aubio_pitch("default", buf_size * hop_factor, buf_size, sample_rate);
        onset_detector = new_aubio_onset("default", buf_size * hop_factor, buf_size, sample_rate);
        set_silence_treshold(silence);
    }

    NoteDetection::Result NoteDetection::analyze(const float* input_buffer, unsigned int nsamples)
    {
        fvec_t input_vec = {.length = nsamples, .data = (float*)input_buffer};
        
        aubio_filter_do(filter, &input_vec);
        
        // spl equals 1 if silence, otherwise non positive
        float spl = aubio_level_detection(&input_vec, silence_treshold());
        bool onset_was_found;
        if (spl != 1) {
            float onset;
            fvec_t onset_vec = {.length = 1, .data = &onset};
            aubio_onset_do(onset_detector, &input_vec, &onset_vec);
            onset_was_found = onset == 0;
        }
        
        note_buffer_mutex.lock();
        if (spl == 1) {
            note_buffer.clear();
        }
        if (spl != 1 && onset_was_found) {
            Result current;
            float frequency, note;
            fvec_t pitch_vec = {.length = 1, .data = &frequency};
            
            aubio_pitch_do(pitch_detector, &input_vec, &pitch_vec);
            
            note = 12 * log2(frequency / 440.) + 69;
            if (note > 0) {
                current.note = roundf(note);
                current.pitch = pitch_mode? note - (float)current.note : 0;
                current.velocity = 64;
            
                note_buffer.add(current);
            }
            else {
                note_buffer.clear();
            }
        }
        
        if (note_buffer.size() == note_buffer.get_capacity()) {
            Result for_sort[note_buffer.size()];
            std::copy(note_buffer.begin(), note_buffer.end(), for_sort);
            std::sort(for_sort, for_sort + note_buffer.size(), &compare_results);
            auto result = for_sort[note_buffer.size() / 2];
            note_buffer_mutex.unlock();
            return result;
        }
        note_buffer_mutex.unlock();
        return {.note = 0, .pitch = 0, .velocity = 0};
    }
    
    bool NoteDetection::compare_results(const NoteDetection::Result a, const NoteDetection::Result b)
    {
        return (a.velocity == 0 && b.velocity > 0) || (float)a.note + a.pitch < (float)b.note + b.pitch;
    }
    
    void NoteDetection::set_silence_treshold(float s)
    {
        aubio_pitch_set_silence(pitch_detector, s);
        aubio_onset_set_silence(onset_detector, s);
    }
    
    float NoteDetection::silence_treshold()
    {
        return aubio_pitch_get_silence(pitch_detector);
    }
};
