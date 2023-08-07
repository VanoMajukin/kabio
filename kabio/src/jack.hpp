/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <jack/jack.h>
#include <QThread>
#include "midi.hpp"
#include <functional>
#include <QObject>
#include <QMutex>

namespace kabio
{
    class Jack : public QObject
    {
        Q_OBJECT
    public:
        typedef std::function<void(float*, unsigned int, MIDI*)> Callback;
        Jack(char *client_name, unsigned int buffer_size, Callback callback);
        ~Jack();
        void start();
        unsigned int sample_rate();
        void set_buffer_size(unsigned int);
    private:
        Callback cb;
        jack_client_t *client;
        jack_port_t *input;
        jack_port_t *output;
        static int process(unsigned int nsamples, void*arg);
        MIDI midi;
    };
};
