/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#include "jack.hpp"
#include <jack/jack.h>
#include <QThread>

namespace kabio
{
    Jack::Jack(char *client_name, unsigned int buffer_size, Callback callback)
    {
        JackStatus status;
        client = jack_client_open(client_name, JackNoStartServer, &status);
        if (status & JackFailure || client == NULL)
            throw tr("Failed to start JACK client");
        input = jack_port_register(client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
        output = jack_port_register(client, "out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
        cb = callback;
        jack_set_process_callback(client, process, this);
        jack_set_buffer_size(client, buffer_size);
    }
    
    Jack::~Jack()
    {
        jack_client_close(client);
    }
    
    int Jack::process(unsigned int nsamples, void* arg)
    {
        if (nsamples == 0)
            return 0;
        Jack* t = (Jack*)arg;
        void *input_buffer = jack_port_get_buffer(t->input, nsamples);
        void *output_buffer = jack_port_get_buffer(t->output, nsamples);
        t->midi.set_buffer(output_buffer);
        t->cb((float*)input_buffer, nsamples, &t->midi);
        return 0;
    }
    
    void Jack::start() {
        jack_activate(client);
    }
    
    unsigned int Jack::sample_rate()
    {
        return jack_get_sample_rate(client);
    }
    
    void Jack::set_buffer_size(unsigned int buffer_size)
    {
        jack_set_buffer_size(client, buffer_size);
    }
};
